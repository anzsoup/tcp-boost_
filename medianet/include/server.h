#ifndef __SERVER_H__
#define __SERVER_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "session.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    class server
    {
        public:
            server(unsigned short port = 0);
            ~server();
            unsigned short get_listening_port() const;

        protected:
            virtual session* create_new_session();

        private:
            void begin_accept();
            void handle_accept(boost::shared_ptr<session> cl_session, const boost::system::error_code &error);

        private:
            io_service m_ios;
            tcp::acceptor *m_acceptor;
            boost::thread m_thread;
            unsigned short m_listening_port;
    };
}

#endif