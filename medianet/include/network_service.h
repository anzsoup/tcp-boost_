#ifndef __NETWORK_SERVICE_H__
#define __NETWORK_SERVICE_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <string>
#include "connector.h"
#include "client_listener.h"
#include "session.h"
//#include "logic_message_entry.h"

using namespace boost::asio;
using namespace boost::asio::ip;

namespace medianet
{
    class network_service
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
            network_service(bool use_logicthread = false);

            ~network_service();

            io_service* get_io_service() const;

            /**
             * Start connection asynchronously.
             * @param host Host IP or domain.
             * @param port Port number.
             * @param on_connected_to Result callback.
             */
            void connect(std::string host, short port, boost::function<void (tcp::socket*)> handler);

            /**
             * Start listening connection request.
             * Dynamic port is automatically assigned if port == 0.
             * @param port Listening port.
             * @param backlog The maximum length of the queue of pending connections.
             * @param on_connected_from Result callback.
             * @return Assigned port.
             */
            unsigned short start_listen(unsigned short port, int backlog, boost::function<void (tcp::socket*)> handler);

            void end_listen();

            /**
             * Connection to server completed with no error.
             */
            void on_connected_to(tcp::socket *sv_socket);
            /**
             * Connection from client completed with no error.
             */
            void on_connected_from(tcp::socket *cl_socket);

        private:
            void begin_receive(session *sess);
            void handle_receive(session *sess, const boost::system::error_code& error, size_t bytes_transferred);
    
        private:
            // io_service is noncopyable
            io_service *m_ios;
            //logic_message_entry *m_logic_entry;
            client_listener *m_listener;
            connector *m_connector;
    };
}

#endif