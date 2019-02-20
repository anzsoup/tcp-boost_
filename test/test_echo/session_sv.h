#include "medianet.h"

using namespace medianet;
using namespace boost::asio;
using namespace boost::asio::ip;

class session_sv : public session
{
    public:
        session_sv(io_service *ios, tcp::socket *socket)
            : session(ios, socket)
        {
            
        }

        void on_message(packet *msg) override
        {
            static int i = 1;

            if (i > 10) return; 

            auto echo = msg->pop_int32();
            std::cout << "echo : " + std::to_string(echo) + "\n";

            auto pkt = packet::create(0);
            pkt->push_int32(echo + 1);
            send(pkt);
            ++i;
        }
};