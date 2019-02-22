#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "medianet.h"
#include "server_test.h"
#include "client_test.h"

using namespace std;
using namespace medianet;
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
    client_test cl;
    cl.start("127.0.0.1", port);

    int available_length = packet::buffer_length - packet::header_length;
    char line[available_length];
    while (std::cin.getline(line, available_length))
    {
        auto msg = packet::create();
        msg->push_string(std::string(line));
        cl.get_server_session()->send(msg);
    }

    std::cout << "Client test over.\n";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Server : -s, Client : -c <port>\n";
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
            std::cout << "Usage : -c <port>\n";
            return -1;
        }

        auto port = boost::lexical_cast<unsigned short>(std::string(argv[2]));
        run_client(port);
    }
    else
    {
        std::cout << "Server : -s, Client : -c <port>\n";
    }

    return 0;
}