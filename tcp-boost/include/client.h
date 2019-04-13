#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <string>
#include "network_service_interface.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace tcp_boost
{
    /**
     * @brief
     * Can connect to only one host.
     * 
     * @author chickeningot
     */
    class client : public network_service_interface
    {
        public:
            client();
            ~client();
            session* get_server_session();
            io_service& get_io_service();

            /**
             * @brief
             * Connect to given host and become client.
             * 
             * @param host IP or domain name.
             * @param port Port.
             */
            void start(std::string host, unsigned short port);

        protected:
            session* create_new_session(io_service &ios) override;

        private:
            void handle_connect(const boost::system::error_code &error);

        private:
            io_service m_ios;
            boost::thread m_thread;
            session *m_sv_session;
    };
}

#endif