#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <string>

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /**
     * A class hiding connecting operation.
     * @author leejm
     */
    class connector
    {
        public:
            connector(io_service *ios);
            void connect(std::string host, short port, boost::function<void (tcp::socket*)> handler);

        private:
            void connecting_job(std::string host, short port);

        private:
            io_service *m_ios;
            tcp::socket *m_sv_socket;
            boost::thread m_thread;
            boost::function<void (tcp::socket*)> m_handler;
    };
}

#endif