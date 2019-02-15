#include "session.h"
#include "packet.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    session::session(tcp::socket *socket)
        : m_peer(nullptr),
          m_socket(socket),
          m_state(state::idle),
          m_sending_queue()
    {
        m_buffer = new char[packet::BUFFER_SIZE]();
    }

    session::~session()
    {
        delete m_socket;
        delete[] m_buffer;
        delete m_peer;
    }

    tcp::socket*
    session::get_socket() const
    {
        return m_socket;
    }

    char*
    session::get_buffer() const
    {
        return m_buffer;
    }

    peer*
    session::get_peer()
    {
        return m_peer;
    }

    void
    session::set_peer(peer *pr)
    {
        m_peer = pr;
    }

    void
    session::on_connected()
    {
        m_state = state::connected;
    }

    void
    session::on_receive(size_t bytes_transferred)
    {
        // Assuming that stream is never segmented and arrives at one piece.
        auto msg = new packet(m_buffer, this);
        on_message(msg);
    }

    void
    session::on_message(packet *msg)
    {
        m_peer->on_message(msg);
        delete msg;
    }

    void
    session::close()
    {
        // already closed
        if (m_state == state::closed)
            return;

        m_state = state::closed;

        if (m_socket)
        {
            m_socket->close();
            on_closed();
        }
    }

    void
    session::on_closed()
    {
        m_peer->on_removed();
    }

    void
    session::send(char *data)
    {
        return;
    }
} // medianet