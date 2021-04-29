#include "header/winsock_tcp_server.h"
#include "header/mysql_api.h"
#include "header/mysql_api_exception.h"

extern MySqlAPI database;

void getFields(std::string& str, const std::size_t& argc, ...);
void registration(TcpServer::Client& socket, std::string& str);

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);
    database.connect();

    std::string command, str;
    do
    {
        socket.recvData(command, str);

        if (command == "REG")
        {
            registration(socket, str);
            socket.sendData("REG TRUE");
        }

    } while(str != "EXT");

    database.disconnect();
    socket.close();
}

void registration(TcpServer::Client& socket, std::string& str)
{
    try {
        char email[36], password[36], login[36];
        getFields(str, 4, login, password, email);
        database.execQuery("INSERT INTO Account(AccountLogin, AccountPassword, Email) VALUES ('" + std::string(login) + "', '" + std::string(password) + "', '" + std::string(email) + "')");
    }
    catch (const MySqlException::ExecutionQueryFailed error)
    {
        socket.sendData("REG FALSE");
        std::cout << error.what();
    }
}
