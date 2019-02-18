#ifndef __LOGIC_MESSAGE_ENTRY_H__
#define __LOGIC_MESSAGE_ENTRY_H__

#include <boost/asio.hpp>
#include "network_service.h"

namespace medianet
{
    class logic_message_entry
    {
        public:
            logic_message_entry(network_service *service);
            void start();
        
        private:
            network_service *m_service;
    };
}

#endif