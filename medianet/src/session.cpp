#include <boost/bind.hpp>
#include <iostream>
#include "session.h"
#include "packet.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    session::session(io_service *ios, tcp::socket *socket)
        : m_ios(ios),
          m_peer(nullptr),
          m_socket(socket),
          m_state(state::idle),
          m_mtx_sending(),
          m_cv_sending(),
          m_sending_queue(),
          m_is_sending_stopped(false)
    {
        m_buffer = new char[packet::BUFFER_SIZE]();

        // Start sending thread
        m_sending_thread = boost::thread(boost::bind(&session::sending_job, this));
        m_sending_thread.join();
    }

    session::~session()
    {
        delete m_socket;
        delete[] m_buffer;
        delete m_peer;
    }

    tcp::socket*
    session::get_socket() const
    {
        return m_socket;
    }

    char*
    session::get_buffer() const
    {
        return m_buffer;
    }

    remote_peer*
    session::get_peer()
    {
        return m_peer;
    }

    void
    session::set_peer(remote_peer *pr)
    {
        m_peer = pr;
    }

    void
    session::on_connected()
    {
        m_state = state::connected;
    }

    void
    session::on_receive(size_t bytes_transferred)
    {
        // Assuming that stream is never segmented and arrives at one piece.
        auto msg = new packet(m_buffer);
        on_message(msg);
    }

    void
    session::on_message(packet *msg)
    {
        if (m_peer)
        {
            m_peer->on_message(msg);
        }
        
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
            on_closed();
        }
    }

    void
    session::on_closed()
    {
        // Stop sending thread loop
        m_is_sending_stopped = true;

        if (m_socket)
        {
            m_socket->close();
        }
        if (m_peer)
        {
            m_peer->on_removed();
        }
        
        std::cout << "Session closed." << std::endl;
        delete this;
    }

    void
    session::send(packet *msg)
    {
        msg->record_size();
        if (!m_sending_queue.push(msg))
            std::cout << "Failed to enqueue packet to sending queue." << std::endl;
        // awake sending queue
        m_cv_sending.notify_all();
    }

    void
    session::sending_job()
    {
        while (true)
        {
            m_cv_sending.wait(m_mtx_sending);
            while (!m_sending_queue.empty())
            {
                packet *msg;
                if (m_sending_queue.pop(msg))
                {
                    boost::system::error_code error;

                    // non-blocking synchronous send
                    // It's going to continue writing until all of the data has been transferred.
                    boost::asio::write(*m_socket, buffer(msg->get_buffer(), msg->get_size()), error);
                    // todo : you want pooling?
                    delete msg;

                    if (error)
                    {
                        std::cout << "Failed to send packet. : " << error.message() << std::endl;
                    }
                }
                else
                {
                    std::cout << "Failed to dequeue packet from sending queue." << std::endl;
                }
            }

            if (m_is_sending_stopped)
                break;
        }
    }
}