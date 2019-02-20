#include "medianet.h"

using namespace medianet;
using namespace boost::asio;
using namespace boost::asio::ip;

class session_cl : public session
{
    public:
        static int COUNT;

        session_cl(io_service *ios, tcp::socket *socket)
            : session(ios, socket)
        {
            ++COUNT;
            std::cout << "Current : " + std::to_string(COUNT) + "\n";
        }

        void on_message(packet *msg) override
        {
            auto receive = msg->pop_int32();
            std::cout << "receive : " + std::to_string(receive) + "\n";

            auto echo = new packet(*msg);
            send(echo);
        }

        void on_disconnected() override
        {
            --COUNT;
            std::cout << "Current : " + std::to_string(COUNT) + "\n";
        }
};

int session_cl::COUNT = 0;