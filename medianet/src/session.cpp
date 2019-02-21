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
          m_rcv_packet(),
          m_state(state::connected),
          m_sending_queue()
    {
        
    }

    session::~session()
    {
        close();
    }

    void
    session::start()
    {
        m_ios.post(boost::bind(&session::begin_receive, this));
        on_created();
    }

    tcp::socket&
    session::get_socket()
    {
        return m_socket;
    }

    void
    session::on_created() { }

    void
    session::on_closed() { }
    
    void
    session::on_message(packet msg) { }

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
            m_ios.post(boost::bind(&session::do_close, this));
        }
    }

    void
    session::send(boost::shared_ptr<packet> msg)
    {
        m_ios.post(boost::bind(&session::begin_send, this, msg));
    }

    void
    session::begin_send(boost::shared_ptr<packet> msg)
    {
        bool send_in_progress = !m_sending_queue.empty();
        msg->record_length();
        m_sending_queue.push_back(msg);
        if (!send_in_progress)
        {
            boost::asio::async_write(m_socket,
                boost::asio::buffer(m_sending_queue.front()->get_buffer(), 
                    m_sending_queue.front()->get_length()),
                        boost::bind(&session::handle_send, this, 
                            boost::asio::placeholders::error));
        }
    }

    void
    session::handle_send(const boost::system::error_code &error)
    {
        if (error)
        {
            std::cout << "Failed to send packet. : " + error.message() + "\n";
            std::cout << "Trying re-send.\n";
        }
        else
        {
            m_sending_queue.pop_front();
        }
        
        if (!m_sending_queue.empty())
        {
            boost::asio::async_write(m_socket,
                boost::asio::buffer(m_sending_queue.front()->get_buffer(), 
                        m_sending_queue.front()->get_length()),
                            boost::bind(&session::handle_send, this,
                                boost::asio::placeholders::error));
        }
        else if (m_state == state::reserve_closing)
        {
            do_close();
        }
    }

    void
    session::begin_receive()
    {
        boost::asio::async_read(m_socket,
            boost::asio::buffer(m_rcv_packet.get_buffer(), packet::header_length),
                boost::bind(&session::handle_receive_header, this,
                    boost::asio::placeholders::error));
    }

    void
    session::handle_receive_header(const boost::system::error_code &error)
    {
        if (error == boost::asio::error::eof)
        {
            connection_lost();
        }
        else if (error)
        {
            std::cout << "Failed to receive header. : " + error.message() + "\n";
        }
        else
        {
            std::cout<<"handle_receive_header\n";
            m_rcv_packet.decode_length();
            boost::asio::async_read(m_socket,
                boost::asio::buffer(m_rcv_packet.get_body(), m_rcv_packet.get_body_length()),
                    boost::bind(&session::handle_receive_body, this,
                        boost::asio::placeholders::error));
        }
    }

    void
    session::handle_receive_body(const boost::system::error_code &error)
    {
        if (error == boost::asio::error::eof)
        {
            connection_lost();
        }
        else if (error)
        {
            std::cout << "Failed to receive body. : " + error.message() + "\n";
        }
        else
        {
            std::cout<<"handle_receive_body\n";
            on_message(m_rcv_packet);
            boost::asio::async_read(m_socket,
                boost::asio::buffer(m_rcv_packet.get_buffer(), packet::header_length),
                    boost::bind(&session::handle_receive_header, this,
                        boost::asio::placeholders::error));
        }
    }

    void
    session::connection_lost()
    {
        std::cout << "Connection lost from remote peer.\n";
        do_close();
    }

    void
    session::do_close()
    {
        m_state = state::closed;
        m_socket.cancel();
        m_socket.close();
        on_closed();
        std::cout << "Session closed.\n";
    }
}