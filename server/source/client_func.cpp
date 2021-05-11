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
void acceptRequest(TcpServer::Client& socket, std::string& str);
void closeChat(TcpServer::Client& socket, std::string& str);
void sendMessage(TcpServer::Client& socket, std::string& str);
void getCompanies(TcpServer::Client& socket);
void addCompany(TcpServer::Client& socket, std::string& str);
void deleteCompany(TcpServer::Client& socket, std::string& str);
void changeCompany(TcpServer::Client& socket, std::string& str);
void addService(TcpServer::Client& socket, std::string& str);
void deleteService(TcpServer::Client& socket, std::string& str);
void changeService(TcpServer::Client& socket, std::string& str);
void getServicesByCompany(TcpServer::Client& socket, std::string& str);

void clientFunc(void* clientSocket)
{
    TcpServer::Client socket = *(TcpServer::Client*)(clientSocket);

    std::string command, str;
    do
    {
        socket.recvData(command, str);
        std::cout << str << std::endl;

        if (command == "REG") // registration
        {
            registration(socket, str);
        }
        else if (command == "ATH") // authentification
        {
            autherization(socket, str);
        }
        else if (command == "CNS") // get consulatation
        {
            consultation(socket, str);
        }
        else if (command == "CCN") // close consulation
        {
            closeConsultation(socket, str);
        }
        else if (command == "RDL") // request delete
        {
            deleteRequest(socket, str);
        }
        else if (command  == "RAP") // request accept
        {
            acceptRequest(socket, str);
        }
        else if (command == "CLC") // close chat
        {
            closeChat(socket, str);
        }
        else if (command == "MSD") // message send
        {
            sendMessage(socket, str);
        }
        else if (command == "GTC") // get companies
        {
            getCompanies(socket);
        }
        else if (command == "ADC") // add company
        {
            addCompany(socket, str);
        }
        else if (command == "DLC") // delete company
        {
            deleteCompany(socket, str);
        }
        else if (command == "CHC") // change company
        {
            changeCompany(socket, str);
        }
        else if (command == "ADS") // add service
        {
            addService(socket, str);
        }
        else if (command == "DLS") // delete service
        {
            deleteService(socket, str);
        }
        else if (command == "CHS") // change service
        {
            changeService(socket, str);
        }
        else if (command == "GBC") // get by company
        {
            getServicesByCompany(socket, str);
        }

    } while(command != "EXT"); // exit

    socket.close();
}

void registration(TcpServer::Client& socket, std::string& str)
{
    try {
        char password[36], login[36];
        getFields(str, 3, login, password);
        database.execQuery("INSERT INTO Account(AccountLogin, AccountPassword) VALUES ('" + std::string(login) + "', '" + std::string(password) + "');");
        socket.sendData("SRG", ""); // sucess registration
    }
    catch (const MySqlException::ExecutionQueryFailed error)
    {
        socket.sendData("FRG", ""); // fail registration
    }
}

void autherization(TcpServer::Client& socket, std::string& str)
{
    try {
        char password[36], login[36];
        getFields(str, 3, login, password);
        if (database.isExists("SELECT COUNT(*) FROM Account WHERE (AccountLogin = '" + std::string(login) + "' AND AccountPassword = '" + std::string(password) + "');"))
        {
            if (!database.isExists("SELECT COUNT(*) FROM BanList WHERE AccountID = (SELECT ID FROM Account WHERE AccountLogin = '" +  std::string(login) + "');"))
            {
                socket.sendData("SAU", database.select("SELECT Rolename FROM Account WHERE AccountLogin = '" + std::string(login) + "';", true)); // success authentification
            }
            else
            {
                std::string ended = database.select("SELECT ended FROM BanList WHERE AccountID = (SELECT ID FROM Account WHERE AccountLogin = '" + std::string(login) + "');", true);
                time_t tm = time(NULL);
                char now[64];
                strftime(now, 64, "%Y-%m-%d", localtime(&tm));
                if (ended > now)
                {
                    socket.sendData("BAN", database.select("SELECT ended FROM BanList WHERE AccountID = (SELECT ID FROM Account WHERE AccountLogin = '" + std::string(login) + "');", true)); // ban
                }
                else
                {
                    socket.sendData("SAU", database.select("SELECT Rolename FROM Account WHERE AccountLogin = '" + std::string(login) + "';", true));
                }
            }
        }
        else
        {
            socket.sendData("FAU", ""); // failed authentification
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
        i.second.sendData("RCN", str); // request handle consultation
    }
}

void closeConsultation(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("RCL", str); // request close
    }
}

void deleteRequest(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("RDL", str); // request deleted by queue
    }
}

void acceptRequest(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("CRC", str); // create chat
    }
}

void closeChat(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("CLC", str); // close chat
    }
}

void sendMessage(TcpServer::Client& socket, std::string& str)
{
    for (auto& i : socket.getSockets())
    {
        i.second.sendData("MSD", str); // message send
    }
}

void getCompanies(TcpServer::Client& socket)
{
    socket.sendData("GTC", database.getAllRows("SELECT CompanyName FROM Company"));
}

void addCompany(TcpServer::Client& socket, std::string& str)
{
    try
    {
        database.execQuery("INSERT INTO Company(CompanyName) VALUES ('" + str + "');");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SAC", ""); // success add company
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FAC", ""); // fail add company
    }
}


void deleteCompany(TcpServer::Client& socket, std::string& str)
{
    try
    {
        database.execQuery("DELETE FROM Company WHERE CompanyName = '" + str + "';");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SDC", ""); // success delete company
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FDC", ""); // fail delete company
    }
}

void changeCompany(TcpServer::Client& socket, std::string& str)
{
    char old_company[32], new_company[32];
    getFields(str, 3, old_company, new_company);
    try
    {
        database.execQuery("UPDATE Company SET CompanyName = '" + std::string(new_company) + "' WHERE CompanyName = '" + old_company + "';");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SCC", ""); // success change company
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FCC", ""); // fail change company
    }
}

void addService(TcpServer::Client& socket, std::string& str)
{

}

void deleteService(TcpServer::Client& socket, std::string& str)
{

}

void changeService(TcpServer::Client& socket, std::string& str)
{

}

void getServicesByCompany(TcpServer::Client& socket, std::string& str)
{

}
