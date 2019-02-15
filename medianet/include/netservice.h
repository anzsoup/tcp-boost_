#ifndef __NETSERVICE_H__
#define __NETSERVICE_H__

#include <boost/asio.hpp>
#include <string>
#include "client_listener.h"
//#include "logic_message_entry.h"
#include "session.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    class netservice
    {
        public:
            /**
             * You can determine whether use synced logic thread or not.
             * If use_logicthread is 'true'
             *   -> Create one logic thread.
             *   -> Messages are being queued and processed in one single thread.
             * 
             * If use_logicthread is 'false'
             *   -> Don't create any logic thread.
             *   -> Messages are processed by each IO threads.
             */ 
            netservice(bool use_logicthread = false);

            ~netservice();

            io_service* get_io_service() const;

            /**
             * Start listening connection request.
             * Dynamic port is automatically assigned if port == 0.
             * @param port Listening port.
             * @param backlog The maximum length of the queue of pending connections.
             * @return Assigned port.
             */
            unsigned short start_listen(unsigned short port, int backlog = 0);

            void end_listen();

        private:
            void on_new_client(tcp::socket *cl_socket);
            void begin_receive(session *sess);
            void process_receive(session *sess, const boost::system::error_code& error, size_t bytes_transferred);
    
        private:
            // io_service is noncopyable
            io_service *m_ios;
            //logic_message_entry *m_logic_entry;
            client_listener *m_listener;
    };
}

#endif