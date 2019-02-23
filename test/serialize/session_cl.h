#include <iostream>
#include <boost/shared_ptr.hpp>
#include "medianet.h"
#include "serialize_test.h"

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
            auto stream = msg.pop_byte_array();
            serialize_test *st = (serialize_test*)stream;
            std::cout << st->to_string() + "\n";
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