#include <iostream>
#include <boost/shared_ptr.hpp>
#include "tcp_boost.h"

using namespace tcp_boost;
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
            auto str1 = msg.pop_string();
            auto str2 = msg.pop_string();
            auto cnt = msg.pop_int32();
            std::cout << str1 + str2 + std::to_string(cnt) + "\n";
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