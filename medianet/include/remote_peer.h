#ifndef __REMOTE_PEER_H__
#define __REMOTE_PEER_H__

#include "packet.h"

namespace medianet
{
    class session;
    
    class remote_peer
    {
        public:
            remote_peer(session *sess);
            virtual void on_message(packet *msg);
            virtual void on_removed();
            virtual void send(packet *msg);
            virtual void disconnect();

        private:
            session *m_session;
    };
}

#endif