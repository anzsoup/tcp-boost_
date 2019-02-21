#include <boost/bind.hpp>
#include <iostream>
#include "server.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    server::server(unsigned short port)
        : m_ios()
    {
        tcp::endpoint endpoint(tcp::v4(), port);

        // open() and bind() is automatically processed by the constructor.
        m_acceptor = new tcp::acceptor(m_ios, endpoint);

        // Disable 'linger' behaviour to avoid problem when reusing the same port.
        m_acceptor->set_option(tcp::acceptor::reuse_address(true));
        
        m_listening_port = m_acceptor->local_endpoint().port();

        // Start client listeneing.
        begin_accept();

        // Start server thread.
        m_thread = boost::thread(boost::bind(&io_service::run, &m_ios));
    }

    server::~server()
    {
        // Maybe cancle(), close() etc will be called automatically.
        delete m_acceptor;
    }

    void
    server::begin_accept()
    {
        auto cl_session = create_new_session();
        m_acceptor->async_accept(cl_session->get_socket(), 
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
            cl_session->start();
        }

        begin_accept();
    }

    session*
    server::create_new_session()
    {
        return new session(m_ios);
    }
}