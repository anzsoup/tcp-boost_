#ifndef __SERVER_H__
#define __SERVER_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "network_service_interface.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace tcp_boost
{
    /**
     * @brief
     * Can accept multiple connections and create one session per each connection.
     * 
     * @author chickeningot
     */
    class server : public network_service_interface
    {
        public:
            server();
            ~server();
            unsigned short get_listening_port() const;
            io_service& get_io_service();

            /**
             * @brief
             * Start client listening and become host.
             * 
             * @param port
             * Assign listening port. If 0(default), than random port in the range of dynamic port domain will be assigned. 
             */
            void start(unsigned short port = 0);

        protected:
            session* create_new_session(io_service &ios) override;

        private:
            void begin_accept();
            void handle_accept(session *cl_session, const boost::system::error_code &error);
            void do_close();

        private:
            io_service m_ios;
            tcp::acceptor *m_acceptor;
            boost::thread m_thread;
            unsigned short m_listening_port;
    };
}

#endif