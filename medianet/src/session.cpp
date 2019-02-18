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
          m_mtx_send(),
          m_sending_queue()
    {
        m_buffer = new char[packet::BUFFER_SIZE]();
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
        auto msg = new packet(m_buffer, this);
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

        if (m_sending_queue.size() > 0)
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
        m_mtx_send.lock();
        {
            msg->record_size();
            m_sending_queue.push(msg);

            if (m_sending_queue.size() > 1)
            {
                // If something remains in queue, then previous sending has not been completed.
                // begin_send() will be called after current sending operation is done.
                return;
            }

            begin_send();
        }
        m_mtx_send.unlock();
    }

    void
    session::begin_send()
    {
        auto *msg = m_sending_queue.front();
        
        async_write(*m_socket, buffer(msg->get_buffer(), msg->get_size()),
            boost::bind(&session::handle_send, this, msg, placeholders::error));
        m_ios->run();
    }

    void
    session::handle_send(packet *msg, const boost::system::error_code& error)
    {
        m_mtx_send.lock();
        {
            if (error)
            {
                std::cout << "Failed to send packet. : " << error.message() << std::endl;
            }
            
            // remove spent packet
            auto *msg = m_sending_queue.front();
            m_sending_queue.pop();
            // todo : you want packet pooling?
            delete msg;

            if (m_sending_queue.empty())
            {
                // Now we have sent all packets. Close the session if reserved.
                if (m_state == state::reserve_closing)
                {
                    on_closed();
                }
            }
            else
            {
                begin_send();
                return;
            }
        }
        m_mtx_send.unlock();
    }
} // medianet