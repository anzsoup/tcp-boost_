#include "remote_peer.h"

namespace medianet
{
    remote_peer::remote_peer(session *sess)
        : m_session(sess)
    {
        m_session->set_peer(this);
    }

    void
    remote_peer::on_message(packet *msg)
    {

    }

    void
    remote_peer::on_removed()
    {
        
    }

    void
    remote_peer::send(packet *msg)
    {
        m_session->send(msg);
    }

    void
    remote_peer::disconnect()
    {
        m_session->close();
    }
} 