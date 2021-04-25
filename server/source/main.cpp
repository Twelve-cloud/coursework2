#include "header/winsock_tcp_server.h"
#include "header/streamtable.h"
#include "header/constants.h"
#include "header/mysql_api.h"
#include <iostream>

void get(std::string& str, const std::size_t& argc, ...)
{

    va_list list;
    va_start(list, str);

    for (std::size_t i = 0; i < argc - 1; i++)
    {
        std::string::size_type indexOfTheEndOfTheField = str.find("~~~");

        if (indexOfTheEndOfTheField != std::string::npos)
        {
            std::strcpy(va_arg(list, char*), str.substr(0, indexOfTheEndOfTheField).c_str());
            str.erase(0, indexOfTheEndOfTheField + 3);
        }
        else
        {
            std::strcpy(va_arg(list, char*), str.c_str());
        }
    }

    va_end(list);
}

StreamTable serverTable;
void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void serverMenu(std::size_t menuItem);
MySqlAPI database("localhost", "root", "26091999", "BrokerBase");

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);

    database.connect();
    std::string data;
    socket.recvData(data);

    char login[100], password[100];
    get(data, 3, login, password);
    std::cout << login << " + " << password << std::endl;
    database.execQuery("INSERT INTO Account VALUES (DEFAULT, '" + std::string(login) + "', '" + std::string(password) + "', '+375(44)774-41-44', 'ilyasavin@mail.ru', 'USER')");
    database.select("SELECT * FROM Account");

    database.disconnect();
    socket.close();
}

int main()
{
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
            case 1: _beginthread([](void* server) { reinterpret_cast<TcpServer*>(server) -> start(); }, 0, &server); break;
            case 2: server.showHostInfo(); system("pause"); break;
            case 3: server.getClients(); system("pause"); break;
            case 4: server.showWinsockInfo(); system("pause"); break;
            case 5: server.stop(); break;
            case 6: isEnd = true;
        }

    } while (!isEnd);
    return 0;
}
