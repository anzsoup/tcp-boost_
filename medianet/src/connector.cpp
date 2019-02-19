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
    connector::connect(std::string host, short port, CBK_CONNECTION_HANDLER on_connected)
    {
        m_on_connected = on_connected;
        tcp::endpoint ep(address::from_string(host), port);
        m_sv_socket = new tcp::socket(*m_ios);
        m_sv_socket->async_connect(ep, boost::bind(&connector::handle_connect, this, placeholders::error));
        m_ios->poll();
    }

    void
    connector::handle_connect(const boost::system::error_code &error)
    {
        // if error == success
        if (!error)
        {
            std::cout << "Connection succeed" << std::endl;
            m_on_connected(m_sv_socket);
        }
        else
        {
            std::cout << "Connection failed : " << error.message() << std::endl;
        }
    }
} // medianet