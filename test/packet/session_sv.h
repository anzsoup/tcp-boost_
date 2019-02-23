#include <string>
#include "medianet.h"

using namespace medianet;
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
            auto msg = packet::create();
            msg->push_byte(80);
            msg->push_bool(true);
            msg->push_int16(100);
            msg->push_int32(1001);
            msg->push_int64(1000001);
            msg->push_single(666.666f);
            msg->push_double(10020.54533);
            msg->push_byte_array(new char[10]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'}, 10);
            msg->push_string("unicode not allowed!!");
            send(msg);
        }
};