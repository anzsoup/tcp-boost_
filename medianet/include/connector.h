#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <boost/asio.hpp>
#include <string>
#include "callbacks.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /**
     * A class hiding connecting operation.
     * @author leejm
     * @date 2019-02-13
     * @version 0.1
     */
    class connector
    {
        public:
            connector(io_service *ios);
            void connect(std::string host, short port, CBK_CONNECTION_HANDLER on_connected);

        private:
            virtual void handle_connect(const boost::system::error_code &error);

        private:
            io_service *m_ios;
            tcp::socket *m_sv_socket;
            CBK_CONNECTION_HANDLER m_on_connected;
    };
}

#endif