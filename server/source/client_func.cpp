#include "header/winsock_tcp_server.h"
#include "header/mysql_api.h"

extern MySqlAPI database;

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);
    database.connect();



    database.disconnect();
    socket.close();
}
