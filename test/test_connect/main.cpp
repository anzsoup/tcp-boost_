#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <vector>
#include "medianet.h"
#include "session_cl.h"

using namespace std;
using namespace medianet;
using namespace boost::asio;
using namespace boost::asio::ip;

#define PORT 0

void on_connected(tcp::socket *sv_socket);
void on_client_connected(tcp::socket *cl_socket);

network_service *net;
session *sv;

void run_server()
{
    net = new network_service();
    net->start_listen(PORT, 10, on_client_connected);

    cin.get();

    std::cout << "Server test over.\n";
}

void run_client(short port)
{
    net = new network_service();
    net->connect("127.0.0.1", port, on_connected);

    cin.get();

    std::cout << "Client test over.\n";
}

void on_connected(tcp::socket *sv_socket)
{
    new session(net->get_io_service(), sv_socket);
}

void on_client_connected(tcp::socket *cl_socket)
{
    new session_cl(net->get_io_service(), cl_socket);
}

int main(int argc, char **argv)
{
    string option(argv[1]);
    if (option == "-s")
    {
        run_server();
    }
    else if (option == "-c")
    {
        unsigned short port = PORT;
        if (port == 0 && argc == 3)
        {
            port = boost::lexical_cast<unsigned short>(std::string(argv[2]));
        }
        run_client(port);
    }
}