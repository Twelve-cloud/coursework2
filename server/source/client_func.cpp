#include "header/winsock_tcp_server.h"
#include "header/mysql_api.h"
#include "header/mysql_api_exception.h"

extern MySqlAPI database;

void getFields(std::string& str, const std::size_t& argc, ...);

void registration(TcpServer::Client& socket, std::string& str);
void autherization(TcpServer::Client& socket, std::string& str);
void consultation(TcpServer::Client& socket, std::string& str);
void closeConsultation(TcpServer::Client& socket, std::string& str);
void deleteRequest(TcpServer::Client& socket, std::string& str);

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);
    database.connect();

    std::string command, str;
    do
    {
        socket.recvData(command, str);
        std::cout << str << std::endl;

        if (command == "REG")
        {
            registration(socket, str);
        }
        else if (command == "ATH")
        {
            autherization(socket, str);
        }
        else if (command == "CNS")
        {
            consultation(socket, str);
        }
        else if (command == "CCN")
        {
            closeConsultation(socket, str);
        }
        else if (command == "RDL")
        {
            deleteRequest(socket, str);
        }

    } while(str != "EXT");

    database.disconnect();
    socket.close();
}

void registration(TcpServer::Client& socket, std::string& str)
{
    try {
        char password[36], login[36];
        getFields(str, 3, login, password);
        database.execQuery("INSERT INTO Account(AccountLogin, AccountPassword) VALUES ('" + std::string(login) + "', '" + std::string(password) + "');");
        socket.sendData("SRG", "");
    }
    catch (const MySqlException::ExecutionQueryFailed error)
    {
        socket.sendData("FRG", "");
    }
}

void autherization(TcpServer::Client& socket, std::string& str)
{
    try {
        char password[36], login[36];
        getFields(str, 3, login, password);
        if (database.isExists("SELECT COUNT(*) FROM Account WHERE (AccountLogin = '" + std::string(login) + "' AND AccountPassword = '" + std::string(password) + "');"))
        {
            socket.sendData("SAU", database.select("SELECT Rolename FROM Account WHERE AccountLogin = '" + std::string(login) + "';", true));
        }
        else
        {
            socket.sendData("FAU", "");
        }
    }
    catch (const MySqlException::ExecutionQueryFailed error)
    {
        socket.sendData("FAU", "");
    }
}

void consultation(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("RCN", str);
    }
}

void closeConsultation(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("RCL", str); // Пользователь отменил запрос
    }
}

void deleteRequest(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("RDL", str); // Консультант удалил запрос из очереди
    }
}
