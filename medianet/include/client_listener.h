#ifndef __CLIENT_LISTENER_H__
#define __CLIENT_LISTENER_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>
#include "callbacks.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /**
     * A class hiding client listening operation.
     */
    class client_listener
    {
        public:
            client_listener(io_service *ios);
            ~client_listener();
            unsigned short start(unsigned short port, int backlog, CBK_CONNECTION_HANDLER on_client_connected);
            void stop();

        private:
            void begin_accept();
            virtual void handle_accept(tcp::socket *cl_socket, const boost::system::error_code &error);

        private:
            io_service *m_ios;
            tcp::acceptor *m_acceptor;
            CBK_CONNECTION_HANDLER m_on_client_connected;
    };
}

#endif