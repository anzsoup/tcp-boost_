#ifndef __PEER_H__
#define __PEER_H__

#include "packet.h"

namespace medianet
{
    class peer
    {
        public:
            virtual void on_message(packet *msg) = 0;
            virtual void on_removed() = 0;
            void send(packet *msg);
            void disconnect();
    };
}

#endif