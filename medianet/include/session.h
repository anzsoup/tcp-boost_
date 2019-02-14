#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>

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

            const short SYS_CLOSE_REQ = -1;
            const short SYS_CLOSE_ACK = -2;

        public:
            session(tcp::socket *socket);
            //peer get_peer() const;
            tcp::socket* get_socket() const;
            state get_state() const;
            // void set_peer(const peer *p);
            // void set_socket(const tcp::socket *socket);
            void on_connected();

        private:
            /** 
             * A flag used to prevent redundant close operation
             * 0 = connected,
             * 1 = disconnected
             */
            int m_isclosed;

            //message_resolver m_resolver;
            //peer *m_peer;
            tcp::socket *m_socket;
            state m_state;
    };
}

#endif