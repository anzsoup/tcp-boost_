#include <iostream>
#include <boost/bind.hpp>
#include "client_listener.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    client_listener::client_listener(io_service *ios)
        : m_ios(ios),
          m_acceptor(nullptr)
    {
        
    }

    client_listener::~client_listener()
    {
        stop();
    }

    unsigned short
    client_listener::start(unsigned short port, int backlog, CBK_CONNECTION_HANDLER on_client_connected)
    {
        if (m_acceptor)
        {
            std::cerr << "Client listening already in progress." << std::endl;
            return port;
        }

        m_on_client_connected = on_client_connected;
        
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
        std::cout << "Begin client listening. Port : " << assigned_port << std::endl;

        begin_accept();
        return assigned_port;
    }

    void
    client_listener::begin_accept()
    {
        auto cl_socket = new tcp::socket(*m_ios);
        m_acceptor->async_accept(*cl_socket, 
            boost::bind(&client_listener::handle_accept, this, cl_socket, placeholders::error));
        m_ios->poll();
    }

    void
    client_listener::handle_accept(tcp::socket *cl_socket, const boost::system::error_code &error)
    {
        if (!error)
        {
            std::cout << "Client connection accepted." << std::endl;
            m_on_client_connected(cl_socket);
        }
        else
        {
            std::cout << "An error occurs while accepting new client : " << error.message() << std::endl;
        }
        
        begin_accept();
    }

    void
    client_listener::stop()
    {
        if (m_acceptor)
        {
            m_acceptor->cancel();
            m_acceptor->close();

            delete m_acceptor;
            m_acceptor = nullptr;

            std::cout << "End client listening." << std::endl;
        }
    }
} // medianet
