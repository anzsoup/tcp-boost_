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
    network_service::connect(std::string host, short port, boost::function<void (tcp::socket*)> handler)
    {
        if (!m_connector)
            m_connector = new connector(m_ios);

        m_connector->connect(host, port, handler);
    }

    unsigned short
    network_service::start_listen(unsigned short port, int backlog, boost::function<void (tcp::socket*)> handler)
    {
        if (!m_listener)
            m_listener = new client_listener(m_ios);
        
        unsigned short assigned_port = m_listener->start(port, backlog, handler);
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
        } 
    }

    io_service*
    network_service::get_io_service() const
    {
        return m_ios;
    }
} // medianet