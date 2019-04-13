#include <string>
#include "tcp_boost.h"

using namespace tcp_boost;
using namespace boost::asio;
using namespace boost::asio::ip;

class session_sv : public session
{
    public:
        session_sv(io_service &ios)
            : session(ios)
        {
            
        }

        void on_created() override
        {
            serialize_test st;
            auto stream = (char*)&st;
            auto msg = packet::create();
            msg->push_byte_array(stream, sizeof(st));
            send(msg);
        }
};