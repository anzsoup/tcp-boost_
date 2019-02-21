#include <boost/bind.hpp>
#include <iostream>
#include "server.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    server::server(unsigned short port)
        : m_ios(),
          m_acceptor(m_ios, tcp::endpoint(tcp::v4(), port))
    {
        // Disable 'linger' behaviour to avoid problem when reusing the same port.
        m_acceptor.set_option(tcp::acceptor::reuse_address(true));
        
        m_listening_port = m_acceptor.local_endpoint().port();
        
        // Start server thread.
        m_thread = boost::thread(boost::bind(&io_service::run, &m_ios));

        // Start client listeneing.
        m_ios.post(boost::bind(&server::begin_accept, this));

        std::cout << "Start server. Port : " + std::to_string(m_listening_port) +"\n";
    }

    server::~server()
    {
        m_ios.post(boost::bind(&server::do_close, this));
    }

    session*
    server::create_new_session(io_service &ios)
    {
        return new session(ios);
    }

    unsigned short
    server::get_listening_port() const
    {
        return m_listening_port;
    }

    io_service&
    server::get_io_service()
    {
        return m_ios;
    }

    void
    server::begin_accept()
    {
        auto cl_session = create_new_session(m_ios);
        m_acceptor.async_accept(cl_session->get_socket(), 
                boost::bind(&server::handle_accept, this, cl_session, 
                    boost::asio::placeholders::error));
    }

    void
    server::handle_accept(session *cl_session, const boost::system::error_code &error)
    {
        if (error)
        {
            std::cout << "Failed to accept new client. : " + error.message() + "\n";
        }
        else
        {
            std::cout << "New client connected.\n";
            cl_session->start();
        }

        begin_accept();
    }

    void
    server::do_close()
    {
        m_acceptor.close();
    }
}