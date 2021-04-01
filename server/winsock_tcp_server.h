#ifndef WINSOCK_TCP_SERVER_H
#define WINSOCK_TCP_SERVER_H

#include "tcp_server_exception.h"
#include "winsock.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <climits>
#include <cstddef>
#include <memory>
#include <string>

class TcpServer
{
private:
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
    SOCKET getSocket()
    {
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // семейство адресов IPv4, спецификация сокета - передача надеждных потоков байтов для протокола TCP, протокол TCP.

        if (_socket == INVALID_SOCKET)
        {
            throw TcpServerException::GettingSocketFailed("Getting the socket failed");
        }
    }
    void winsockInitialization()
    {
        if (WSAStartup(version, &initializationParams) != 0)
        {
            throw TcpServerException::WinSockInitializationFailed("WinSock initialization failed");
        }
        else
        {
            std::cout << "Winsock initialization succeeded" << std::endl;
        }
    }
    void bindSocket()
    {
        socketData.sin_family = AF_INET; // указываем семейство адресов, AF_INET - IPv4
        socketData.sin_addr.s_addr  = htonl(INADDR_ANY); // указываем адрес хоста, INADDR_ANY - адрес локального хоста
        socketData.sin_port = htons(3360); // указываем порт
        // функции htonl(), htons() - преобразуют номер хоста к сетевому формату, причина этому 2 порядка хранения байтов little-endian & big-endian
        // функции ntonl(), ntons() - преобразуют сетевой формат чисел к конерктному номеру хоста

        if (bind(_socket, (SOCKADDR*)&socketData, sizeof(socketData)) != 0) // связываем сокет с хостом (socketData содержит данные хоста)
        {
            throw TcpServerException::BindSocketFailed("Bind socket failed");
        }

        std::cout << "Binded with host: " << hostname << std::endl;
    }

public:
    TcpServer()
    {
        winsockInitialization();
        hostname = getHostname();
        hostData = getHostData();
        _socket = getSocket();
        bindSocket();
        listen(_socket, INT_MAX);
    }

    ~TcpServer()
    {
        closesocket(_socket);
        WSACleanup();
    }

    void stopServer()
    {
        closesocket(_socket);
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

private:
    const WORD version = MAKEWORD(2, 2);
    WSADATA initializationParams;
    std::string hostname;
    std::unique_ptr<HOSTENT> hostData;
    SOCKET _socket;
    SOCKADDR_IN socketData;
};

#endif // WINSOCK_TCP_SERVER_H
