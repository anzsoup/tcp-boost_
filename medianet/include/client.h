#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <string>
#include "session.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    class client
    {
        public:
            client(std::string host, unsigned short port);
            session& get_server_session();
            io_service& get_io_service();

        private:
            void handle_connect(const boost::system::error_code &error);

        private:
            io_service m_ios;
            boost::thread m_thread;
            session m_sv_session;
    };
}

#endif