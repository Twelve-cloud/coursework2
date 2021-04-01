#include "winsock_tcp_server.h"
#include <iostream>

int main()
{
    TcpServer server;

    server.showHostInfo();
    std::cout << std::endl;
    server.showWinsockInfo();
    server.stopServer();

    return 0;
}
