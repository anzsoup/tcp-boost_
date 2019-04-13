#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/container/deque.hpp>
#include <boost/shared_ptr.hpp>
#include "packet.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace tcp_boost
{
    /** 
     * @brief
     * Represents each connection.
     * 
     * @details
     * You can send message, handle received message, and observe socket status.
     * 
     * @author chickeningot
     */
    class session
    {
        public:
            enum state
            {
                idle,

                connected,

                /** 
                 * @brief
                 * Disconnection reserved.
                 * 
                 * @details
                 * If close() method is called while waiting for sending packets,
                 * it will be disconnected after all remaining packets are sent.
                 */
                reserve_closing,

                closed
            };

        protected:
            /**
             * Be called right after the start() is called.
             */
            virtual void on_created();
            /**
             * Be called right after the socket is closed.
             */
            virtual void on_closed();
            
        public:
            /**
             * Be called when new packet has been received.
             */
            virtual void on_message(packet msg);

        public:
            session(io_service &ios);
            ~session();
            tcp::socket& get_socket();
            state get_state() const;
            void start();
            void close();

            /**
             * @brief
             * Send packet.
             * 
             * @details
             * So many copy operations may occurs if packet is passed as stack object.
             * And parameter packet can't be a raw pointer object since sending operation is done asynchronously.
             * Therefor, it takes packet as form of shared_ptr.
             */
            void send(boost::shared_ptr<packet> msg);

        private:
            void begin_send(boost::shared_ptr<packet> msg);
            void begin_receive();
            void handle_send(const boost::system::error_code &error);
            void handle_receive_header(const boost::system::error_code& error);
            void handle_receive_body(const boost::system::error_code& error);
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