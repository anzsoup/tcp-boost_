#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <boost/asio.hpp>
#include <boost/function.hpp>

namespace medianet
{
    class session;

    typedef boost::function<void (session*)> CBK_SESSION_HANDLER;
    typedef boost::function<void (boost::asio::ip::tcp::socket*)> CBK_CONNECTION_HANDLER;
}

#endif