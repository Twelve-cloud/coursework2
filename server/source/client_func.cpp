#include "header/winsock_tcp_server.h"
#include "header/mysql_api.h"
#include "header/encrypt.h"
#include "header/mysql_api_exception.h"
#include <cmath>

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
void clientRequestedServices(TcpServer::Client& socket);
void getServicePrice(TcpServer::Client& socket, std::string& str);
void clientOrderService(TcpServer::Client& socket, std::string& str);
void brokerGetRequests(TcpServer::Client& socket);
void cancelRequest(TcpServer::Client& socket, std::string& str);
void brokerHandleRequest(TcpServer::Client& socket, std::string& str);


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
        else if (command == "CRS") // client requested company
        {
            clientRequestedServices(socket);
        }
        else if (command == "GSP") // get service price
        {
            getServicePrice(socket, str);
        }
        else if (command == "COS") // client orders service
        {
            clientOrderService(socket, str);
        }
        else if (command == "BGR") // broket get request
        {
            brokerGetRequests(socket);
        }
        else if (command == "CHS") // change service
        {
            changeService(socket, str);
        }
        else if (command == "ADS") // add service
        {
            addService(socket, str);
        }
        else if (command == "DES") // delete service
        {
            deleteService(socket, str);
        }
        else if (command == "CRE") // cancel request
        {
            cancelRequest(socket, str);
        }
        else if (command == "BHR") // broker handle request
        {
            brokerHandleRequest(socket, str);
        }

    } while(command != "EXT"); // exit

    socket.close();
}

void registration(TcpServer::Client& socket, std::string& str)
{
    try {
        char pass[36], login[36];
        getFields(str, 3, login, pass);
        std::string password = pass;
        ENCRYPTION_CYCLE(password, KEY);
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
        char pass[36], login[36];
        getFields(str, 3, login, pass);
        std::string password = pass;
        ENCRYPTION_CYCLE(password, KEY);
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
    char service[32], price[32], company[32];
    getFields(str, 4, service, price, company);
    try
    {
        database.execQuery("INSERT INTO Service(ServiceName, ServicePrice, CompanyName) VALUES ('" + std::string(service) + "', '" + price + "', '" + company + "');");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SAS", ""); // success change service
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FAS", ""); // fail change service
    }
}

void deleteService(TcpServer::Client& socket, std::string& str)
{
    char service[32], price[32], company[32];
    getFields(str, 4, service, price, company);
    try
    {
        database.execQuery("DELETE FROM Service WHERE ServiceName = '" + std::string(service) + "' AND ServicePrice = '" + price + "' AND CompanyName = '" + company + "';");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SDS", ""); // success change service
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FDS", ""); // fail change service
    }
}

void changeService(TcpServer::Client& socket, std::string& str)
{
    char old_service[32], old_price[32], new_service[32], new_price[32], company[32];
    getFields(str, 6, old_service, new_service, old_price, new_price, company);
    try
    {
        database.execQuery("UPDATE Service SET ServiceName = '" + std::string(new_service) + "', ServicePrice = '" + new_price + "' WHERE CompanyName = '" + company + "' AND ServiceName = '" + old_service + "' AND ServicePrice = '" + old_price + "';");
        for (auto& i : socket.getSockets())
        {
            i.second.sendData("SCS", ""); // success change service
        }
    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FCS", ""); // fail change service
    }
}

void getServicesByCompany(TcpServer::Client& socket, std::string& str)
{
    socket.sendData("GBC", database.getAllRows("SELECT ServiceName, ServicePrice FROM Service WHERE CompanyName = '" + str + "';"));
}

void clientRequestedServices(TcpServer::Client& socket)
{
    socket.sendData("CRS", database.getAllRows("SELECT ServiceName FROM Service GROUP BY ServiceName;"));
}

void getServicePrice(TcpServer::Client& socket, std::string& str)
{
    socket.sendData("GSP", str + "~~~" + database.getAllRows("SELECT AVG(ServicePrice) FROM Service WHERE ServiceName = '" + str + "';"));
}

void clientOrderService(TcpServer::Client& socket, std::string& str)
{
    try
    {
        char client[32], service[32];
        getFields(str, 3, client, service);

        database.execQuery("INSERT INTO Basket(AccountID, ServiceName) VALUES ((SELECT ID FROM Account WHERE AccountLogin = '" + std::string(client) + "'), '" + service + "');");

        for (auto& i : socket.getSockets())
        {
            i.second.sendData("HCS", str); // handle client service
        }

    }
    catch (const MySqlException::ExecutionQueryFailed& error)
    {
        socket.sendData("FAR", ""); // fail add request
    }
}

void brokerGetRequests(TcpServer::Client& socket)
{
    socket.sendData("BGR", database.getAllRows("SELECT AccountLogin, ServiceName FROM Account, Basket WHERE Account.ID = Basket.AccountID;"));
}

void cancelRequest(TcpServer::Client& socket, std::string& str)
{
    char client[32], service[32];
    getFields(str, 3, client, service);
    database.execQuery("DELETE FROM Basket WHERE AccountID = (SELECT ID FROM Account WHERE AccountLogin = '" + std::string(client) + "') AND ServiceName = '" + service + "';");

    for (auto& i : socket.getSockets())
    {
        i.second.sendData("SCR", str); // success cancel request
    }
}

void brokerHandleRequest(TcpServer::Client& socket, std::string& str)
{
    char client[32], service[32];
    getFields(str, 3, client, service);

    std::string avarages = database.getAllRows("SELECT AVG(Price), CompanyName FROM PriceHistory WHERE ServiceName = '" + std::string(service) + "' GROUP BY CompanyName ORDER BY CompanyName"); // извлекаем среднее значение и компанию связанную с этим средним
    std::string avgSend = avarages;

    std::vector<double> avgs; // хранятся средние значения

    int i = 0;
    while (avarages != "")
    {
        char avprice[32], companyName[32];
        getFields(avarages, 3, avprice, companyName);
        avgs.push_back((i++, atof(avprice))); // добавляем среднее значение в массив
    }

    std::vector<double> risks(avgs.size()); // храняться риски

    std::string amountChangesOfServicesInCompany = database.getAllRows("SELECT COUNT(*) FROM PriceHistory WHERE ServiceName = '" + std::string(service) + "' GROUP BY CompanyName ORDER BY CompanyName;"); // извлекаем количество записей цен чтобы знать N
    std::vector<int> amountChangesOfService; // хрянится количество изменений цен каждой услуги, нужно для прохода внутреннего цикла и чтобы поделить на N

    while (amountChangesOfServicesInCompany != "")
    {
        char amountOfChanges[32];
        getFields(amountChangesOfServicesInCompany, 2, amountOfChanges);
        amountChangesOfService.push_back(atoi(amountOfChanges)); // добавляем количество изменений в массив
    }

    std::string amountOfServicePrices = database.getAllRows("SELECT Price FROM PriceHistory WHERE ServiceName = '" + std::string(service) + "' ORDER BY CompanyName;"); // извлекаем все цены, нужно для расчета риска
    std::vector<double> servicePrices; // для расчета риска

    while (amountOfServicePrices != "")
    {
        char price[32];
        getFields(amountOfServicePrices, 2, price);
        servicePrices.push_back(atof(price)); // добавляем цены в массив
    }

    int ii = 0;
    for (std::size_t i = 0; i < avgs.size(); i++)
    {
        for (std::size_t j = 0; j < amountChangesOfService[i]; j++)
        {
            risks[i] += std::pow(servicePrices[ii++] - avgs[i], 2); // считаем (удельная эффективность - средняя удельная эффективность) в квадрате
        }
        risks[i] /= (amountChangesOfService[i] - 1) * 1000; // делим на количество N - 1 умноженной на сотку, чтобы значения слишком высокие не были
    }

    std::string risksStr;
    for (std::size_t i = 0; i < risks.size(); i++)
    {
        std::string tmp;
        tmp = std::to_string(risks[i]);
        risksStr += tmp += "~~~";
    }

    socket.sendData("BHR", avgSend + "Risks~~~" + risksStr);
}
