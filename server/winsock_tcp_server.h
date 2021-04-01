#ifndef WINSOCK_TCP_SERVER_H
#define WINSOCK_TCP_SERVER_H

#include "tcp_server_exception.h"
#include "winsock.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cstdlib>
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

        if (bind(_socket, (SOCKADDR*)&socketData, sizeof(socketData)) != 0)
        {
            throw TcpServerException::BindSocketFailed("Bind socket failed");
        }
    }
public:
    TcpServer()
    {
        winsockInitialization();
        hostname = getHostname();
        _socket = getSocket();
        bindSocket();
    }

    ~TcpServer()
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

private:
    const WORD version = MAKEWORD(2, 2);
    WSADATA initializationParams;
    std::string hostname;
    SOCKET _socket;
    SOCKADDR_IN socketData;
};

#endif // WINSOCK_TCP_SERVER_H
