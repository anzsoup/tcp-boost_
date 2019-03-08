#include <iostream>
#include <boost/shared_ptr.hpp>
#include "medianet.h"

using namespace medianet;
using namespace boost::asio;
using namespace boost::asio::ip;

class session_cl : public session
{
    public:
        static int COUNT;

        session_cl(io_service &ios)
            : session(ios)
        {
            
        }

        void on_message(packet msg) override
        {
            auto str = msg.pop_string();
            std::cout << str + "\n";
            boost::shared_ptr<packet> echo(new packet(msg.get_buffer()));
            send(echo);
        }

    protected:
        void on_created() override
        {
            ++COUNT;
            std::cout << "Current : " + std::to_string(COUNT) + "\n";
        }

        void on_closed() override
        {
            --COUNT;
            std::cout << "Current : " + std::to_string(COUNT) + "\n";
        }
};

int session_cl::COUNT = 0;