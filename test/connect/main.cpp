#include <iostream>
#include <boost/lexical_cast.hpp>
#include "tcp_boost.h"
#include "server_test.h"

using namespace std;
using namespace tcp_boost;
using namespace boost::asio;
using namespace boost::asio::ip;

void run_server()
{
    server_test sv;
    sv.start();

    cin.get();

    std::cout << "Server test over.\n";
}

void run_client(unsigned short port)
{
    client cl;
    cl.start("127.0.0.1", port);

    cin.get();

    std::cout << "Client test over.\n";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "[Usage] Server : -s, Client : -c <port>\n";
        return -1;
    }

    string option(argv[1]);
    if (option == "-s")
    {
        run_server();
    }
    else if (option == "-c")
    {
        if (argc != 3)
        {
            std::cout << "[Usage] -c <port>\n";
            return -1;
        }

        auto port = boost::lexical_cast<unsigned short>(std::string(argv[2]));
        run_client(port);
    }
    else
    {
        std::cout << "[Usage] Server : -s, Client : -c <port>\n";
    }

    return 0;
}