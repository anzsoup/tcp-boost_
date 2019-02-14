#ifndef __CLIENT_LISTENER_H__
#define __CLIENT_LISTENER_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    typedef boost::function<void (tcp::socket*)> CBK_ACCEPT_HANDLER;

    class client_listener
    {
        public:
            client_listener(io_service *ios);
            ~client_listener();
            unsigned short start(unsigned short port, int backlog, CBK_ACCEPT_HANDLER accept_handler);
            void stop();

        private:
            void begin_accept();
            void handle_accept(tcp::socket *cl_socket, const boost::system::error_code &error);

        private:
            io_service *m_ios;
            tcp::acceptor *m_acceptor;
            CBK_ACCEPT_HANDLER m_accept_handler;
    };
}

#endif