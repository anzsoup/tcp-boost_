#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>
#include <mutex>
#include <queue>
#include "packet.h"
#include "remote_peer.h"

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
            session(io_service *ios, tcp::socket *socket);
            ~session();
            tcp::socket* get_socket() const;
            char* get_buffer() const;
            state get_state() const;
            remote_peer* get_peer();
            void set_peer(remote_peer *pr);
            void on_connected();
            void on_receive(size_t bytes_transferred);
            void on_message(packet *msg);
            void close();

            /**
             * Send packet.
             * Passed packet will be destroyed after the sending operation is done.
             */
            void send(packet *msg);

        private:
            void on_closed();
            void begin_send();
            void handle_send(packet *msg, const boost::system::error_code& error);

        private:
            io_service *m_ios;
            tcp::socket *m_socket;
            state m_state;
            char *m_buffer;
            remote_peer *m_peer;
            std::mutex m_mtx_send;
            std::queue<packet*> m_sending_queue;
    };
}

#endif