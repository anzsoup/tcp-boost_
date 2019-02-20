#include <iostream>
#include <boost/bind.hpp>
#include "connector.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    connector::connector(io_service *ios)
        : m_ios(ios)
    {
    }

    void
    connector::connect(std::string host, short port, boost::function<void (tcp::socket*)> handler)
    {
        m_handler = handler;
        m_thread = boost::thread(boost::bind(&connector::connecting_job, this, host, port));
    }

    void
    connector::connecting_job(std::string host, short port)
    {
        tcp::endpoint ep(address::from_string(host), port);
        m_sv_socket = new tcp::socket(*m_ios);
        boost::system::error_code error;
        m_sv_socket->connect(ep, error);

        if (error)
        {
            std::cout << "Connection failed. : " + error.message() + "\n";
        }
        else
        {
            std::cout << "Connection succeed.\n";
            m_handler(m_sv_socket);
        }
    }
} // medianet