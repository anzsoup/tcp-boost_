#include <boost/bind.hpp>
#include <iostream>
#include "network_service.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    network_service::network_service(bool use_logicthread)
        : m_listener(nullptr),
          m_connector(nullptr)
    {
        m_ios = new io_service();

        // if (use_logicthread)
        // {
        //     m_logic_entry = new logic_message_entry(this);
        //     m_logic_entry->start();
        // }
    }

    network_service::~network_service()
    {
        if (m_connector)
            delete m_connector;
            
        end_listen();

        // if (m_logic_entry)
        // {
        //     m_logic_entry->stop();
        //     delete m_logic_entry;
        // }
    }

    void
    network_service::connect(std::string host, short port, CBK_SESSION_HANDLER on_connected)
    {
        if (!m_connector)
            m_connector = new connector(m_ios);

        m_on_connected = on_connected;

        m_connector->connect(host, port,
            boost::bind(&network_service::on_connected, this, _1));
    }

    unsigned short
    network_service::start_listen(unsigned short port, int backlog, CBK_SESSION_HANDLER on_client_connected)
    {
        if (!m_listener)
            m_listener = new client_listener(m_ios);

        m_on_client_connected = on_client_connected;
        
        unsigned short assigned_port = m_listener->start(port, backlog, 
            boost::bind(&network_service::on_client_connected, this, _1));
        return assigned_port;
    }

    void
    network_service::end_listen()
    {
        if (m_listener)
        {
            m_listener->stop();
            delete m_listener;
            m_listener = nullptr;
            m_on_client_connected.clear();
        } 
    }

    io_service*
    network_service::get_io_service() const
    {
        return m_ios;
    }

    void
    network_service::on_connected(tcp::socket *sv_socket)
    {
        auto sv_session = new session(m_ios, sv_socket);
        m_on_connected(sv_session);
        begin_receive(sv_session);
    }

    void
    network_service::on_client_connected(tcp::socket *cl_socket)
    {
        auto cl_session = new session(m_ios, cl_socket);
        m_on_client_connected(cl_session);
        begin_receive(cl_session);
    }

    void
    network_service::begin_receive(session *sess)
    {
        char *buf = sess->get_buffer();
        tcp::socket *socket = sess->get_socket();
        // socket::async_read_some(or socket::async_receive) may not receive all of the requested number of bytes.
        // I use async_read() instead to guarantee that packets are always received in one piece.
        async_read(*socket, buffer(buf, packet::BUFFER_SIZE),
            boost::bind(&network_service::handle_receive, this, sess, placeholders::error, placeholders::bytes_transferred));
        m_ios->poll();
    }

    void
    network_service::handle_receive(session *sess, const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (bytes_transferred > 0 && !error)
        {
            sess->on_receive(bytes_transferred);
            // Continue to receive.
            begin_receive(sess);
        }
        else
        {
            // It means that connection has been lost from remote peer.
            if (bytes_transferred == 0)
            {
                std::cout << "Connection lost from remote peer." << std::endl;
                sess->close();
            }
        }
    }
} // medianet