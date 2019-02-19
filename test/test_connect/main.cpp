#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <vector>
#include "medianet.h"

using namespace std;
using namespace medianet;

#define PORT 0

void on_connected(session *sess);
void on_client_connected(session *sess);

network_service *net;
session *sv;
vector<session*> *vec_cl;

void run_server()
{
    net = new network_service();
    vec_cl = new vector<session*>();
    net->start_listen(PORT, 10, on_client_connected);

    cin.get();

    for (auto sess : *vec_cl)
        sess->close();
    std::cout << "Server test over." << std::endl;
}

void run_client(short port)
{
    net = new network_service();
    net->connect("127.0.0.1", port, on_connected);

    cin.get();

    sv->close();
    std::cout << "Client test over." << std::endl;
}

void on_connected(session *sess)
{
    sv = sess;
}

void on_client_connected(session *sess)
{
    vec_cl->push_back(sess);
    std::cout << "Current : " << vec_cl->size() << std::endl;
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