#include "header/winsock_tcp_server.h"
#include "header/streamtable.h"
#include "header/constants.h"
#include "header/mysql_api.h"
#include <iostream>

using namespace Constants;

// ---------------------------------------------------------------
StreamTable serverTable;
StreamTable serverConfigTable;
StreamTable mysqlConfigTable;
StreamTable selectTable;
MySqlAPI database("localhost", "root", "26091999", "BrokerBase");
// ---------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------
void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void serverMenu(std::size_t menuItem);
void clientFunc(void* clientSocket);
void setOutputFormat();
void workWithAccounts();
// -----------------------------------------------------------------------------------------------------------

int main()
{
    TcpServer server(clientFunc, "127.0.0.1", 3360);
    database.connect();

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    setOutputFormat();

    bool isEnd = false;
    std::size_t menuItem = Range::FIRST;

    do
    {
        renderMenu(serverMenu, Range::LAST_MAIN, menuItem);

        switch (menuItem)
        {
            case 1: _beginthread([](void* server) { reinterpret_cast<TcpServer*>(server) -> start(); }, 0, &server); break;
            case 2: workWithAccounts(); break;
            case 3: database.connect(); database.showInfo(); database.disconnect(); system("pause"); break;
            case 4: server.showHostInfo(); system("pause"); break;
            case 5: server.getClients(); system("pause"); break;
            case 6: server.showWinsockInfo(); system("pause"); break;
            case 7: server.stop(); system("pause"); break;
            case 8: isEnd = true;
        }

    } while (!isEnd);

    database.disconnect();

    return 0;
}
