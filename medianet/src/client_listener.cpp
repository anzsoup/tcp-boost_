#include <iostream>
#include <boost/bind.hpp>
#include "client_listener.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    client_listener::client_listener(boost::shared_ptr<io_service> ios)
        : m_ios(ios),
          m_acceptor(nullptr),
          m_cv(),
          m_is_stopped(false)
    {
        
    }

    client_listener::~client_listener()
    {
        stop();
    }

    unsigned short
    client_listener::start(unsigned short port, int backlog, boost::function<void (tcp::socket*)> handler)
    {
        if (m_acceptor)
        {
            std::cerr << "Client listening already in progress.\n";
            return port;
        }

        m_handler = handler;
        
        tcp::endpoint endpoint(tcp::v4(), port);

        // open() and bind() is automatically processed by the constructor.
        m_acceptor = new tcp::acceptor(*m_ios, endpoint);

        // Disable 'linger' behaviour to avoid problem when reusing the same port.
        m_acceptor->set_option(tcp::acceptor::reuse_address(true));
        
        if (backlog > 0)
        {
            m_acceptor->listen(backlog);
        }
        else
        {
            m_acceptor->listen();
        }
        
        unsigned short assigned_port = m_acceptor->local_endpoint().port();
        m_thread = boost::thread(boost::bind(&client_listener::listening_job, this, assigned_port));
        return assigned_port;
    }

    void
    client_listener::listening_job(unsigned short port)
    {
        std::cout << "Start client listening thread. Port : " + std::to_string(port) + "\n";

        while (true)
        {
            m_next_socket = new tcp::socket(*m_ios);
            boost::system::error_code error;
            // Do async to reduce cpu usage.
            m_ios->reset();
            m_acceptor->async_accept(*m_next_socket, 
                boost::bind(&client_listener::handle_accept, this, m_next_socket, 
                    boost::asio::placeholders::error));
            m_ios->run();

            if (m_is_stopped)
                break;
        }

        m_ios->reset();
        std::cout << "End client listening thread.\n";
    }

    void
    client_listener::handle_accept(tcp::socket *cl_socket, const boost::system::error_code &error)
    {
        if (error)
        {
            std::cout << "An error occurs while accepting new client : " + error.message() + "\n";
        }
        else
        {
            std::cout << "Client connection accepted.\n";
            m_handler(cl_socket);
        }
    }

    void
    client_listener::stop()
    {
        if (m_acceptor)
        {
            m_is_stopped = true;
            m_acceptor->cancel();
            m_acceptor->close();

            delete m_acceptor;
            m_acceptor = nullptr;
        }

        if (m_next_socket)
        {
            m_next_socket->cancel();
            delete m_next_socket;
            m_next_socket = nullptr;
        }
    }
} // medianet
