#ifndef __LOGIC_MESSAGE_ENTRY_H__
#define __LOGIC_MESSAGE_ENTRY_H__

#include <boost/asio.hpp>
#include "netservice.h"

namespace medianet
{
    class logic_message_entry
    {
        public:
            logic_message_entry(netservice *service);
            void start();
        
        private:
            netservice *m_service;
    };
}

#endif