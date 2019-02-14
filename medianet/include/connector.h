#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <boost/asio.hpp>
#include <string>
#include "netservice.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    /**
     * A class for connecting operation.
     * @author leejm
     * @date 2019-02-13
     * @version 0.1
     */
    class connector
    {
        public:
            connector(netservice &net);

            /**
             * Start a connection asynchronously.
             * @param host Host IP or domain.
             * @param port Port number.
             */
            void connect(std::string host, short port);

        private:
            void handle_connect(const boost::system::error_code &error);

        private:
            netservice m_net;
            tcp::socket *m_sv_socket;
    };
}

#endif