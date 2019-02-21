#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/container/deque.hpp>
#include <boost/shared_ptr.hpp>
#include "packet.h"

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

            static const int SENDING_QUEUE_SIZE = 20000;

        protected:
            virtual void on_created();
            virtual void on_closed();
            
        public:
            virtual void on_message(packet msg);

        public:
            session(io_service &ios);
            ~session();
            tcp::socket& get_socket();
            state get_state() const;
            void start();
            void close();

            /**
             * Send packet.
             * Passed packet will be destroyed after the sending operation is done.
             */
            void send(boost::shared_ptr<packet> msg);

        private:
            void begin_send(boost::shared_ptr<packet> msg);
            void begin_receive();
            void handle_send(const boost::system::error_code &error);
            void handle_receive_header(const boost::system::error_code& error, size_t bytes_transferred);
            void handle_receive_body(const boost::system::error_code& error, size_t bytes_transferred);
            void connection_lost();
            void do_close();

        private:
            io_service &m_ios;
            tcp::socket m_socket;
            state m_state;
            packet m_rcv_packet;
            boost::container::deque<boost::shared_ptr<packet>> m_sending_queue;
    };
}

#endif