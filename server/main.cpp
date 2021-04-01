#include "winsock_tcp_server.h"
#include <iostream>

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);

    std::string data;
    socket.recvData(data);
    std::cout << data;

    socket.close();
}

void startServer(void* s)
{
    TcpServer* server = (TcpServer*)s;
    server -> start();
}

int main()
{
    TcpServer server(clientFunc, "127.0.0.1", 3360);

    _beginthread(startServer, 0, (void*)&server);

    system("pause");

    server.getClients();

    return 0;
}
