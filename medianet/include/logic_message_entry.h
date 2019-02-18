#ifndef __LOGIC_MESSAGE_ENTRY_H__
#define __LOGIC_MESSAGE_ENTRY_H__

#include <boost/asio.hpp>
#include <queue>
#include "network_service.h"

namespace medianet
{
    /**
     * Process pakcets in one logic thread.
     */
    class logic_message_entry
    {
        public:
            logic_message_entry(network_service *net);
            void start();
        
        private:
            network_service *m_net;
            std::queue<packet*> m_msg_queue;
    };
}

#endif