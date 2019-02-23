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
            for (int i = 0; i < 100000; ++i)
            {
                auto msg = packet::create();
                msg->push_string("Big brother is watching you! ");
                msg->push_string("Big brother is watching you! ");
                msg->push_int32(i);
                send(msg);
            }
        }
};