#include <iostream>
#include <string>
#include "medianet.h"
#include "server_test.h"

using namespace std;
using namespace medianet;
using namespace boost::asio;
using namespace boost::asio::ip;

int main(int argc, char **argv)
{
    server_test sv;
    sv.start();

    cin.get();

    std::cout << "Server test over.\n";

    return 0;
}