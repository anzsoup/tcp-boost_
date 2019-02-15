#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>
#include <mutex>
#include <queue>
#include "packet.h"
#include "peer.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /** 
     * A unit of socket communication.
     * This is managed by the netservice and you don't need to care about it.
     * @author leejm
     * @date 2019-02-13
     * @version 0.1
     */
    class session
    {
        public:
            enum state
            {
                idle,

                connected,

                /** 
                 * Disconnection reserved.
                 * If disconnect method is called while waiting for sending packets,
                 * it will be disconnected after all remaining packets are sent.
                 */
                reserve_closing,

                closed
            };

        public:
            session(tcp::socket *socket);
            ~session();
            tcp::socket* get_socket() const;
            char* get_buffer() const;
            state get_state() const;
            peer* get_peer();
            void set_peer(peer *pr);
            void on_connected();
            void on_receive(size_t bytes_transferred);
            void on_message(packet *msg);
            void on_closed();
            void close();
            void send(char *data);

        private:
            tcp::socket *m_socket;
            state m_state;
            char *m_buffer;
            peer *m_peer;
            std::mutex m_mtx_send;
            std::queue<char*> m_sending_queue;
    };
}

#endif