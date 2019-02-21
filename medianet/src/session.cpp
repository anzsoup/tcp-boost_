#include <boost/bind.hpp>
#include <iostream>
#include "session.h"
#include "packet.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    session::session(io_service &ios)
        : m_ios(ios),
          m_socket(m_ios),
          m_state(state::connected),
          m_mtx_sending(),
          m_cv_sending(),
          m_flag_sending(false),
          m_sending_queue(),
          m_is_sending_stopped(false),
          m_is_receiving_stopped(false)
    {
        m_buffer = new char[packet::BUFFER_SIZE]();

        // Start sending thread
        m_sending_thread = boost::thread(boost::bind(&session::sending_job, this));
        // Start receiving thread
        m_receiving_thread = boost::thread(boost::bind(&session::receiving_job, this));
    }

    session::~session()
    {
        close();
        delete[] m_buffer;
    }

    tcp::socket
    session::get_socket() const
    {
        return m_socket;
    }

    char*
    session::get_buffer() const
    {
        return m_buffer;
    }

    void
    session::on_message(packet *msg) { }

    void
    session::on_disconnected() { }

    void
    session::handle_message()
    {
        auto msg = new packet(m_buffer);
        on_message(msg);
        // todo : you want packet pooling?
        delete msg;
    }

    void
    session::close()
    {
        // already closed
        if (m_state == state::closed)
            return;

        if (!m_sending_queue.empty())
        {
            m_state = state::reserve_closing;
        }
        else
        {
            m_state = state::closed;
            
            // Stop thread loops
            m_is_sending_stopped = true;
            m_is_receiving_stopped = true;
            m_cv_sending.notify_all();

            if (m_socket)
            {
                m_socket->close();
            }
            
            on_disconnected();
            std::cout << "Session closed.\n";
        }
    }

    void
    session::send(packet *msg)
    {
        msg->record_size();
        if (!m_sending_queue.push(msg))
            std::cout << "Failed to enqueue packet to sending queue.\n";
        // awake sending queue
        m_flag_sending = true;
        m_cv_sending.notify_all();

        bool send_in_progress = !m_sending_queue.empty();
        msg->record_size();
        m_sending_queue.push(msg);
        if (!send_in_progress)
        {
            boost::asio::async_write(m_socket,
                boost::asio::buffer(msg->get_buffer(), packet::BUFFER_SIZE),
                    boost::bind(&session::handle_send, this, boost::asio::placeholders::error));
        }
    }

    void
    session::begin_send()
    {

    }

    void
    session::sending_job()
    {
        std::cout << "Start sending thread.\n";

        while (true)
        {
            // We don't know send() or sending_job() that which one is called first.
            // So we need to control condition variable with one more flag.
            if (!m_flag_sending)
            {
                m_cv_sending.wait(m_mtx_sending);
            }
            
            while (!m_sending_queue.empty())
            {
                packet *msg;
                if (m_sending_queue.pop(msg))
                {
                    boost::system::error_code error;

                    // non-blocking synchronous send
                    // It's going to continue writing until all of the data has been transferred.
                    // boost::asio::write(*m_socket, buffer(msg->get_buffer(), msg->get_size()), error);
                    boost::asio::write(*m_socket, buffer(msg->get_buffer(), packet::BUFFER_SIZE), error);
                    // todo : you want pooling?
                    delete msg;
                    std::cout << "sended\n";

                    if (error)
                    {
                        std::cout << "Failed to send packet. : " + error.message() + "\n";
                    }
                }
                else
                {
                    std::cout << "Failed to dequeue packet from sending queue.\n";
                }
            }

            if (m_is_sending_stopped)
                break;

            m_flag_sending = false;
        }

        std::cout << "End sending thread.\n";
    }

    void
    session::receiving_job()
    {
        std::cout << "Start receiving thread.\n";

        while (true)
        {
            // Do receiving asynchronously to reduce cpu usage.
            // socket::async_read_some(or socket::async_receive) may not receive all of the requested number of bytes.
            // So async_read() is used to guarantee that packets are always received in one piece.
            // And for the convenience, It's sending full size of buffer.
            // If you want send and receive the exact size of buffer to save bandwidth,
            // because there is no way to know the size of packet sent from remote peer,
            // you should use async_read_some() and concatenate reached streams manually which requires difficult algorithm.
            m_ios->reset();
            boost::asio::async_read(*m_socket, buffer(m_buffer, packet::BUFFER_SIZE),
                boost::bind(&session::handle_receive, this, placeholders::error, placeholders::bytes_transferred));
            m_ios->run();

            if (m_is_receiving_stopped)
                break;
        }

        m_ios->reset();
        std::cout << "End receiving thread.\n";
    }

    void
    session::handle_receive(const boost::system::error_code &error, size_t bytes_transferred)
    {
        if (bytes_transferred > 0 && !error)
        {
            // Assuming that stream is never segmented and arrives at one piece.
            handle_message();
        }
        else
        {
            // It means that connection has been lost from remote peer.
            if (bytes_transferred == 0 && m_state != state::closed)
            {
                std::cout << "Connection lost from remote peer.\n";
                close();
            }
        }
    }
}