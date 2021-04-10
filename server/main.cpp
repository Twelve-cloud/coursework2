#include "streamtable.h"
#include "winsock_tcp_server.h"
#include <iostream>

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);

    std::string data;
    socket.recvData(data);
    std::cout << data;
    while (1) {}
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

    StreamTable serverTable;

    serverTable.AddCol(50);
    serverTable.MakeBorderExt(true);
    serverTable.SetDelimRow(true, '-');
    serverTable.SetDelimCol(true, '|');



    return 0;
}
