#include <string>
#include "tcp_boost.h"
#include "session_sv.h"

using namespace tcp_boost;
using namespace boost::asio;

class client_test : public client
{
    protected:
        session* create_new_session(io_service &ios) override
        {
            return new session_sv(ios);
        }
};