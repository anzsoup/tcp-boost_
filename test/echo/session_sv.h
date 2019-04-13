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

        void on_message(packet msg) override
        {
            auto echo = msg.pop_string();
            std::cout << echo + "\n";
        }
};