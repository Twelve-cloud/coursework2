#ifndef WINSOCK_TCP_SERVER_H
#define WINSOCK_TCP_SERVER_H

#include "tcp_server_exception.h"
#include <functional>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <memory>
#include <string>
#include <map>

class TcpServer
{
private:
    void winsockInitialization()
    {
        if (WSAStartup(MAKEWORD(2, 2), &initializationParams) != 0) // version 2.2
        {
            throw TcpServerException::WinSockInitializationFailed("WinSock initialization failed");
        }
        else
        {
            std::cout << "Winsock initialization succeeded" << std::endl;
        }
    }
    std::string getHostname() const
    {
        char _hostname[1024];

        if (gethostname(_hostname, sizeof(_hostname)) != 0)
        {
            throw TcpServerException::GettingHostnameFailed("Getting the hostname failed");
        }

        return std::string(_hostname);
    }
    std::unique_ptr<HOSTENT> getHostData() const
    {
        std::unique_ptr<HOSTENT> hstData = std::make_unique<HOSTENT>(*gethostbyname(hostname.c_str()));

        if (hstData == nullptr)
        {
            throw TcpServerException::GettingHostnameDataFailed("Getting hostname data failed");
        }

        return hstData;
    }
    void createSocket()
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // семейство адресов IPv4, спецификация сокета - передача надеждных потоков байтов для протокола TCP, протокол TCP.

        if (serverSocket == INVALID_SOCKET)
        {
            throw TcpServerException::GettingSocketFailed("Getting the socket failed");
        }
    }
    void bindSocket(const std::string& address, const std::size_t& port)
    {
        socketData.sin_family = AF_INET; // указываем семейство адресов, AF_INET - IPv4
        socketData.sin_addr.s_addr  = inet_addr(address.c_str()); // указываем адрес хоста, INADDR_ANY - адрес локального хоста, inet_addr - преобразует c-style строку к сетевому формату
        socketData.sin_port = htons(port); // указываем порт
        // функции htonl(), htons() - преобразуют номер хоста к сетевому формату, причина этому 2 порядка хранения байтов little-endian & big-endian
        // функции ntonl(), ntons() - преобразуют сетевой формат чисел к конерктному номеру хоста

        if (bind(serverSocket, (SOCKADDR*)&socketData, sizeof(socketData)) != 0) // связываем сокет с хостом (socketData содержит данные хоста)
        {
            throw TcpServerException::BindSocketFailed("Bind socket failed");
        }

        std::cout << "Binded with host: " << hostname << std::endl;
    }

public:
    TcpServer(void (*clientFunc)(void*), const std::string& address, const std::size_t& port) : clientsCounter(0), clientFunction(clientFunc)
    {
        winsockInitialization();
        hostname = getHostname();
        hostData = getHostData();
        createSocket();
        bindSocket(address, port);
    }
    ~TcpServer()
    {
        closesocket(serverSocket);
        WSACleanup();
    }


    void showWinsockInfo()
    {
        std::cout << std::setw(30) << "WS Version" << std::setw(30) << initializationParams.wVersion << std::endl;
        std::cout << std::setw(30) << "WS High Version" << std::setw(30) << initializationParams.wHighVersion << std::endl;
        std::cout << std::setw(30) << "WS Description" << std::setw(30) << initializationParams.szDescription << std::endl;
        std::cout << std::setw(30) << "Status" << std::setw(30) << initializationParams.szSystemStatus << std::endl;
    }
    void showHostInfo()
    {
        std::cout << std::setw(30) << "Hostname" << std::setw(30) << hostData -> h_name << std::endl;
        std::cout << std::setw(30) << "Address type" << std::setw(30) << hostData -> h_addrtype << std::endl;
        std::cout << std::setw(30) << "Address length" << std::setw(30) << hostData -> h_length << std::endl;

        for (std::size_t i = 0; hostData -> h_aliases[i] != 0; i++)
        {
            std::cout << std::setw(29) << "Alias #" << i + 1 << std::setw(30) << hostData -> h_aliases[i] << std::endl;
        }

        for (std::size_t i = 0; hostData -> h_addr_list[i] != 0; i++)
        {
            in_addr addr; // структура, представляющая собой адреса интернета, где s_addr - адрес в формате ulong
            addr.s_addr = *(unsigned long*)hostData -> h_addr_list[i];
            std::cout << std::setw(29) << "Address #" << i + 1 << std::setw(30) << inet_ntoa(addr) << std::endl; // inet_ntoa - преобразует сетевой формат к строковому виду
        }
    }
    void getClients()
    {
        for (const auto& client : clientSockets)
        {
            std::cout << "Client: " << client.first << std::endl;
        }
    }
    void start()
    {
        isWork = true;
        listen(serverSocket, INT_MAX);

        while(true)
        {
            SOCKADDR_IN clientSocketData;
            int sizeClientSocketData = sizeof(clientSocketData);

            SOCKET newClientSocket = accept(serverSocket, (SOCKADDR*)&clientSocketData, &sizeClientSocketData);

            if (isWork == false)
                break; //////////////////

            Client clientSocket(*this, newClientSocket, clientSocketData);
            clientSockets.insert(std::make_pair(inet_ntoa(clientSocketData.sin_addr), clientSocket)); // inet_ntoa преобразует in_addr к строковому виду
            ++clientsCounter;

            _beginthread(clientFunction, 0, (void*)&clientSocket);

        }
    }

    void stop() ///////////////
    {
        isWork = false;

        for (auto& i : clientSockets)
        {
            closesocket(i.second.socket);
        }

        clientSockets.clear();
    }

    class Client
    {
    private:
        friend class TcpServer;
        TcpServer& server;
        SOCKET socket = NULL;
        SOCKADDR_IN socketData;
    public:
        Client(TcpServer& _server, SOCKET _socket, SOCKADDR_IN _socketData) : server(_server), socket(_socket), socketData(_socketData) {}

        void sendData(const std::string& sendingStr)
        {
            int size = sendingStr.size();
            if (send(socket, (char*)&size, sizeof(int), 0) == -1)
            {
                throw TcpServerException::SendDataFailed("sending data failed");
            }

            if (send(socket, sendingStr.c_str(), sendingStr.size(), 0) == -1)
            {
                throw TcpServerException::SendDataFailed("sending data failed");
            }
        }
        void recvData(std::string& data)
        {
            int size;
            if (recv(socket, (char*)&size, sizeof(int), 0) == -1)
            {
                throw TcpServerException::RecvDataFailed("receiving data failed");
            }

            char* strData = new char[size];
            if (recv(socket, strData, size, 0) == -1)
            {
                throw TcpServerException::RecvDataFailed("receiving data failed");
            }

            data = strData;
        }
        void close()
        {
            if (socket != NULL)
            {
                closesocket(socket);
                server.clientSockets.erase(std::find_if(server.clientSockets.begin(),
                                                        server.clientSockets.end(),
                                                        [&](std::pair<std::string, Client> element)
                                                        {
                                                            return element.first == inet_ntoa(socketData.sin_addr);
                                                        }));
            }
        }

    };
private:
    WSADATA initializationParams;

    std::string hostname;
    std::unique_ptr<HOSTENT> hostData;

    SOCKET serverSocket;
    SOCKADDR_IN socketData;

    std::multimap<std::string, Client> clientSockets; // хранит по IP сокеты клиентов
    std::size_t clientsCounter;
    void (*clientFunction)(void*);
    bool isWork;
};

#endif // WINSOCK_TCP_SERVER_H
