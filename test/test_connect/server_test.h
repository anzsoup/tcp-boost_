#include "medianet.h"
#include "session_cl.h"

using namespace medianet;
using namespace boost::asio;

class server_test : public server
{
    public:
        session* create_new_session(io_service &ios)
        {
            return new session_cl(ios);
        }
};