#include "tcp_boost.h"
#include "session_cl.h"

using namespace tcp_boost;
using namespace boost::asio;

class server_test : public server
{
    protected:
        session* create_new_session(io_service &ios) override
        {
            return new session_cl(ios);
        }
};