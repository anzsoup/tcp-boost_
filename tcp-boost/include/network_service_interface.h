#ifndef __NETWORK_SERVICE_INTERFACE_H__
#define __NETWORK_SERVICE_INTERFACE_H__

#include <boost/asio.hpp>
#include "session.h"

using namespace boost::asio;

namespace tcp_boost
{
    /**
     * @brief
     * Network services are object that manages boost::asio::io_service and network session.
     * 
     * @author chickeningot
     */
    class network_service_interface
    {
        public:
            virtual io_service& get_io_service() = 0;
        
        protected:
            virtual session* create_new_session(io_service &ios) = 0;
    };
}

#endif