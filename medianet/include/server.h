#ifndef __SERVER_H__
#define __SERVER_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include "session.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    class server
    {
        public:
            server();
            ~server();
            unsigned short get_listening_port() const;
            io_service& get_io_service();
            void start(unsigned short port = 0);

        protected:
            virtual session* create_new_session(io_service &ios);

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