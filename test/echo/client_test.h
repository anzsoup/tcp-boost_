#include <string>
#include "medianet.h"
#include "session_sv.h"

using namespace medianet;
using namespace boost::asio;

class client_test : public client
{
    protected:
        session* create_new_session(io_service &ios) override
        {
            return new session_sv(ios);
        }
};