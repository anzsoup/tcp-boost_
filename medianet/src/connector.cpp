#include <iostream>
#include <boost/bind.hpp>
#include "connector.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    connector::connector(netservice &net)
        : m_net(net)
    {
    }

    void
    connector::connect(std::string host, short port)
    {
        tcp::endpoint ep(address::from_string(host), port);
        auto ios = m_net.get_io_service();
        m_sv_socket = new tcp::socket(*ios);
        m_sv_socket->async_connect(ep, boost::bind(&connector::handle_connect, this, placeholders::error));
        ios->run();
    }

    void
    connector::handle_connect(const boost::system::error_code &error)
    {
        // if error == success
        if (!error)
        {
            std::cout << "Connection success" << std::endl;
            //auto sv_session = new session(m_sv_socket);
            //m_service.on_connection_completed(sv_session);
        }
        else
        {
            std::cout << "Connection failed : " << error.message() << std::endl;
        }
    }
} // medianet