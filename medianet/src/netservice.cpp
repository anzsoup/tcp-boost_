#include <boost/bind.hpp>
#include <iostream>
#include "netservice.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    netservice::netservice(bool use_logicthread)
        : m_listener(nullptr)
    {
        m_ios = new io_service();

        // if (use_logicthread)
        // {
        //     m_logic_entry = new logic_message_entry(this);
        //     m_logic_entry->start();
        // }
    }

    netservice::~netservice()
    {
        end_listen();

        // if (m_logic_entry)
        // {
        //     m_logic_entry->stop();
        //     delete m_logic_entry;
        // }
    }

    unsigned short
    netservice::start_listen(unsigned short port, int backlog)
    {
        if (!m_listener)
            m_listener = new client_listener(m_ios);
        
        unsigned short assigned_port = m_listener->start(port, backlog, 
            boost::bind(&netservice::on_new_client, this, _1));
        return assigned_port;
    }

    void
    netservice::end_listen()
    {
        if (m_listener)
        {
            m_listener->stop();
            delete m_listener;
            m_listener = nullptr;
        } 
    }

    io_service*
    netservice::get_io_service() const
    {
        return m_ios;
    }

    void
    netservice::on_new_client(tcp::socket *cl_socket)
    {
        std::cout << "Client connection accepted." << std::endl;
        auto cl_session = new session(cl_socket);
        //begin_receive(cl_session);
    }
} // medianet