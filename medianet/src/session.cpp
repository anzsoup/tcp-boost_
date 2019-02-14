#include "session.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    session::session(tcp::socket *socket)
        : m_isclosed(0),
          //m_peer(nullptr),
          m_socket(socket),
          m_state(state::idle)
          //m_resolver()
    {

    }

    void
    session::on_connected()
    {
        m_state = state::connected;
        m_isclosed = 0;
    }

    // void
    // session::set_peer(peer *p)
    // {
    //     m_peer = p;
    // }

    // void
    // session::set_socket(tcp::socket *socket)
    // {
    //     m_socket = socket;
    // }
} // medianet