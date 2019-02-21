#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
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

        public:
            virtual void on_message(packet *msg);
            virtual void on_disconnected();

        public:
            session(io_service &ios);
            ~session();
            tcp::socket get_socket() const;
            char* get_buffer() const;
            state get_state() const;
            void start();
            void handle_message();
            void close();

            /**
             * Send packet.
             * Passed packet will be destroyed after the sending operation is done.
             */
            void send(packet *msg);

        private:
            void sending_job();
            void receiving_job();
            void begin_receive();
            void handle_receive(const boost::system::error_code &error, size_t bytes_transferred);

        private:
            io_service m_ios;
            tcp::socket m_socket;
            state m_state;
            char *m_buffer;
            boost::thread m_sending_thread;
            boost::thread m_receiving_thread;
            boost::condition_variable_any m_cv_sending;
            boost::mutex m_mtx_sending;
            bool m_flag_sending;
            boost::lockfree::queue<packet*, boost::lockfree::capacity<SENDING_QUEUE_SIZE>> m_sending_queue;
            bool m_is_sending_stopped;
            bool m_is_receiving_stopped;
    };
}

#endif