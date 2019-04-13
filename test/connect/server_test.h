#include "tcp_boost.h"
#include "session_cl.h"

using namespace tcp_boost;
using namespace boost::asio;

class server_test : public server
{
    public:
        session* create_new_session(io_service &ios)
        {
            return new session_cl(ios);
        }
};