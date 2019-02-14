#include <iostream>
#include <boost/lexical_cast.hpp>
#include "medianet.h"

using namespace std;
using namespace medianet;

#define PORT 0

void run_server()
{
    netservice net;
    net.start_listen(PORT, 10);
}

void run_client(short port)
{
    netservice net;
    connector con(net);
    con.connect("127.0.0.1", port);
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

    cin.get();
}