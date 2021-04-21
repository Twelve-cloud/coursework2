#include "header/winsock_tcp_server.h"
#include "header/streamtable.h"
#include "header/constants.h"
#include "header/mysql_api.h"
#include <iostream>

#include <header/account.h>

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

    Account test;

//    database.execQuery("INSERT INTO AccountRole(rolename) VALUES ('Broker'), ('Consultant'), ('User');");
//    database.execQuery("DELETE FROM AccountRole");
//    database.select("SELECT * FROM AccountRole");
//    system("pause");
//    database.execQuery("INSERT INTO UserAccount(mnum, login, pass) VALUES ('+375(44)771-41-44', 'ilyasavin', '26091999')");
//    database.execQuery("DELETE FROM UserAccount");
//    database.select("SELECT * FROM UserAccount");
//    system("pause");

    char login[1024], password[1024], mnum[1024], anum[1024], role[1024];
    database.select("SELECT * FROM UserAccount", 6, anum, role, mnum, login, password);
    std::cout << "Login: " << login << "\nPassword: " << password << "\nAccount number: " << anum << "\nRole: " << role <<  "\nMobile number: " << mnum << std::endl;
    Account ilya(login, password, mnum);
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
