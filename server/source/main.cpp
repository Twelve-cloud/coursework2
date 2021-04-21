#include "header/winsock_tcp_server.h"
#include "header/streamtable.h"
#include "header/constants.h"
#include "header/mysql_api.h"
#include <iostream>

StreamTable serverTable;
void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void serverMenu(std::size_t menuItem);

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);

    std::string data;
    socket.recvData(data);
    std::cout << data;
    while (1) {}
    socket.close();
    // основная функция для работы с еденицей подключения
}

void startServer(void* server)
{
    reinterpret_cast<TcpServer*>(server) -> start();
}

int main()
{    
    MySqlAPI database("localhost", "root", "26091999", "BrokerBase");

    database.select("SELECT * FROM AccountRole");

    system("pause");
    system("cls");

    database.execQuery("INSERT INTO AccountRole(rolename) VALUES ('Marina')");

    database.select("SELECT * FROM AccountRole");

    system("pause");

    database.execQuery("DELETE FROM AccountRole WHERE rolename = 'Marina'");

    database.select("SELECT * FROM AccountRole");

    system("pause");

    TcpServer server(clientFunc, "127.0.0.1", 3360);

    serverTable.AddCol(50);
    serverTable.MakeBorderExt(true);
    serverTable.SetDelimRow(true, '-');
    serverTable.SetDelimCol(true, '|');

    bool isEnd;
    std::size_t menuItem = Range::FIRST;

    do
    {
        renderMenu(serverMenu, Range::LAST_MAIN, menuItem);

        switch (menuItem)
        {
            case 1: _beginthread(startServer, 0, &server); break;
            case 2: server.showHostInfo(); system("pause"); break;
            case 3: server.getClients(); system("pause"); break;
            case 4: server.showWinsockInfo(); system("pause"); break;
            case 5: server.stop(); break;
            case 6: isEnd = true;
        }

    } while (!isEnd);
    return 0;
}
