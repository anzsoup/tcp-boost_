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
            std::cout << "Packet received.\n";
            char byte = msg.pop_byte();
            bool boolean = msg.pop_bool();
            int16_t int16 = msg.pop_int16();
            int32_t int32 = msg.pop_int32();
            int64_t int64 = msg.pop_int64();
            float single = msg.pop_single();
            double dou = msg.pop_double();
            char *byte_array = msg.pop_byte_array();
            std::string str = msg.pop_string();

            std::cout << std::to_string(byte) + "\n";
            std::cout << std::to_string(boolean) + "\n";
            std::cout << std::to_string(int16) + "\n";
            std::cout << std::to_string(int32) + "\n";
            std::cout << std::to_string(int64) + "\n";
            std::cout << std::to_string(single) + "\n";
            std::cout << std::to_string(dou) + "\n";
            std::cout << std::string(byte_array) + "\n";
            std::cout << str + "\n";
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