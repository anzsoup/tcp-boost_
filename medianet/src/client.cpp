#include <boost/bind.hpp>
#include <iostream>
#include "client.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    client::client(std::string host, unsigned short port)
        : m_ios(),
          m_sv_session(m_ios)
    {
        std::cout << "Connect to \'" + host + ":" + std::to_string(port) + "\'.\n";
        
        tcp::endpoint endpoint(address::from_string(host), port);
        m_sv_session.get_socket().async_connect(endpoint,
            boost::bind(&client::handle_connect, this,
                boost::asio::placeholders::error));

        // Start client thread.
        m_thread = boost::thread(boost::bind(&io_service::run, &m_ios));
    }
    
    session&
    client::get_server_session()
    {
        return m_sv_session;
    }

    io_service&
    client::get_io_service()
    {
        return m_ios;
    }

    void
    client::handle_connect(const boost::system::error_code &error)
    {
        if (error)
        {
            std::cout << "Failed to connect. : " + error.message() + "\n";
        }
        else
        {
            std::cout << "Connected.\n";
            m_sv_session.start();
        }
    }
}