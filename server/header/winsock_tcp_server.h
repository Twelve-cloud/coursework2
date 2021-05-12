#ifndef WINSOCK_TCP_SERVER_H
#define WINSOCK_TCP_SERVER_H

#include "tcp_server_exception.h"
#include "streamtable.h"
#include <functional>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <memory>
#include <string>
#include <map>

//extern StreamTable serverConfigTable;

class TcpServer
{

    void winsockInitialization()
    {
        if (WSAStartup(MAKEWORD(2, 2), &initializationParams) != 0) // version 2.2
        {
            throw TcpServerException::WinSockInitializationFailed("WinSock initialization failed", WSAGetLastError());
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
            throw TcpServerException::GettingHostnameFailed("Getting the hostname failed", WSAGetLastError());
        }

        return std::string(_hostname);
    }
    std::unique_ptr<HOSTENT> getHostData() const
    {
        std::unique_ptr<HOSTENT> hstData = std::make_unique<HOSTENT>(*gethostbyname(hostname.c_str()));

        if (hstData == nullptr)
        {
            throw TcpServerException::GettingHostnameDataFailed("Getting hostname data failed", WSAGetLastError());
        }

        return hstData;
    }
    void createSocket()
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // семейство адресов IPv4, спецификация сокета - передача надеждных потоков байтов для протокола TCP, протокол TCP.

        if (serverSocket == INVALID_SOCKET)
        {
            throw TcpServerException::GettingSocketFailed("Getting the socket failed", WSAGetLastError());
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
            throw TcpServerException::BindSocketFailed("Bind socket failed", WSAGetLastError());
        }

        std::cout << "Binded with host: " << hostname << std::endl;
    }

public:
    TcpServer(void (*clientFunc)(void*), const std::string& address, const std::size_t& port) : clientFunction(clientFunc)
    {
        winsockInitialization();
        hostname = getHostname();
        hostData = getHostData();
        createSocket();
        bindSocket(address, port);
        system("pause");
    }
    ~TcpServer()
    {
        closesocket(serverSocket);
        WSACleanup();
    }


    void showWinsockInfo()
    {
        serverConfigTable << "WS Version" << initializationParams.wVersion;
        serverConfigTable << "WS High Version" << initializationParams.wHighVersion;
        serverConfigTable << "WS Description" << initializationParams.szDescription;
        serverConfigTable << "Status" << initializationParams.szSystemStatus;
    }
    void showHostInfo()
    {
        serverConfigTable << "Hostname" << hostData -> h_name;
        serverConfigTable << "Address type" << hostData -> h_addrtype;
        serverConfigTable << "Address length" << hostData -> h_length;

        for (std::size_t i = 0; hostData -> h_aliases[i] != 0; i++)
        {
            std::cout << std::setw(29) << "Alias #" << i + 1 << std::setw(30) << hostData -> h_aliases[i] << std::endl;
        }

        for (std::size_t i = 0; hostData -> h_addr_list[i] != 0; i++)
        {
            in_addr addr; // структура, представляющая собой адреса интернета, где s_addr - адрес в формате ulong
            addr.s_addr = *(unsigned long*)hostData -> h_addr_list[i];
            serverConfigTable << "Address #" + std::to_string(i + 1) << inet_ntoa(addr); // inet_ntoa - преобразует сетевой формат к строковому виду
        }
    }
    void getClients()
    {
        if (clientSockets.empty())
        {
            std::cout << "0 Clients" << std::endl;
        }

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
                _endthread();

            Client clientSocket(*this, newClientSocket, clientSocketData, amountOfClients);
            clientSockets.insert(std::make_pair(amountOfClients++, clientSocket)); // inet_ntoa преобразует in_addr к строковому виду

            _beginthread(clientFunction, 0, (void*)&clientSocket);

        }
    }
    void stop()
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
        std::uint32_t ID;
        TcpServer& server;
        SOCKET socket = NULL;
        SOCKADDR_IN socketData;
    public:
        Client(TcpServer& _server, SOCKET _socket, SOCKADDR_IN _socketData, std::uint32_t amountOfClients) : server(_server), socket(_socket), socketData(_socketData), ID(amountOfClients) {}

        void sendData(const std::string& command, const std::string& data)
        {
            if (send(socket, command.c_str(), 4, 0) == -1)
            {
                throw TcpServerException::SendDataFailed("sending data failed", WSAGetLastError());
            }

            char sizeStr[4096];
            itoa(data.size() + 1, sizeStr, 10);
            if (send(socket, sizeStr, sizeof(sizeStr), 0) == -1)
            {
                throw TcpServerException::SendDataFailed("sending data failed", WSAGetLastError());
            }

            if (send(socket, data.c_str(), data.size() + 1, 0) == -1)
            {
                throw TcpServerException::SendDataFailed("sending data failed", WSAGetLastError());
            }

        }
        void recvData(std::string& command, std::string& data) try
        {
            char com[4];
            if (recv(socket, com, sizeof(com), 0) == -1)
            {
                throw TcpServerException::RecvDataFailed("receiving data failed", WSAGetLastError());
            }

            char sizeStr[4096];
            if (recv(socket, sizeStr, sizeof(sizeStr), 0) == -1)
            {
                throw TcpServerException::RecvDataFailed("receiving data failed", WSAGetLastError());
            }

            int size = atoi(sizeStr);

            char* strData = new char[size];
            if (recv(socket, strData, size, 0) == -1)
            {
                throw TcpServerException::RecvDataFailed("receiving data failed", WSAGetLastError());
            }

            command = com;
            data = strData;
        }
        catch (const TcpServerException::RecvDataFailed& error)
        {
            _endthread();
        }

        void close()
        {
            if (socket != NULL)
            {
                closesocket(socket);
                server.clientSockets.erase(std::find_if(server.clientSockets.begin(),
                                                        server.clientSockets.end(),
                                                        [&](std::pair<std::uint32_t, Client> element)
                                                        {
                                                            return element.first == ID;
                                                        })
                                           );
            }
        }

        std::multimap<std::uint32_t, Client> getSockets()
        {
            return server.clientSockets;
        }

    };
private:
    WSADATA initializationParams;

    std::string hostname;
    std::unique_ptr<HOSTENT> hostData;

    SOCKET serverSocket;
    SOCKADDR_IN socketData;

    std::multimap<std::uint32_t, Client> clientSockets; // хранит по IP сокеты клиентов (поменять на map в конце)
    std::uint32_t amountOfClients = 0;
    void (*clientFunction)(void*);

    bool isWork;
};

#endif // WINSOCK_TCP_SERVER_H
