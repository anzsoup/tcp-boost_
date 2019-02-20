#ifndef __CLIENT_LISTENER_H__
#define __CLIENT_LISTENER_H__

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /**
     * A class hiding client listening operation.
     * @author leejm
     */
    class client_listener
    {
        public:
            client_listener(io_service *ios);
            ~client_listener();
            unsigned short start(unsigned short port, int backlog, boost::function<void (tcp::socket*)> handler);
            void stop();

        private:
            void listening_job(unsigned short port);
            void handle_accept(tcp::socket *cl_socket, const boost::system::error_code &error);

        private:
            io_service *m_ios;
            tcp::acceptor *m_acceptor;
            tcp::socket *m_next_socket;
            boost::thread m_thread;
            boost::condition_variable_any m_cv;
            bool m_is_stopped;
            boost::function<void (tcp::socket*)> m_handler;
    };
}

#endif