#ifndef TCP_SERVER_EXCEPTION_H
#define TCP_SERVER_EXCEPTION_H

#include "winsock.h"
#include <stdexcept>

namespace TcpServerException
{
    class WinSockInitializationFailed : public std::runtime_error
    {
    public:
        WinSockInitializationFailed(const std::string& initError) : std::runtime_error(initError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class GettingHostnameFailed : public std::runtime_error
    {
    public:
        GettingHostnameFailed(const std::string& hostnameError) : std::runtime_error(hostnameError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class GettingHostnameDataFailed : public std::runtime_error
    {
    public:
        GettingHostnameDataFailed(const std::string& hostnameDataError) : std::runtime_error(hostnameDataError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class GettingSocketFailed : public std::runtime_error
    {
    public:
        GettingSocketFailed(const std::string& socketError) : std::runtime_error(socketError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class BindSocketFailed : public std::runtime_error
    {
    public:
        BindSocketFailed(const std::string& bindError) : std::runtime_error(bindError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class RecvDataFailed : public std::runtime_error
    {
    public:
        RecvDataFailed(const std::string& recvError) : std::runtime_error(recvError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };

    class SendDataFailed : public std::runtime_error
    {
    public:
        SendDataFailed(const std::string& sendError) : std::runtime_error(sendError + ", error code: " + std::to_string(WSAGetLastError())) {}
    };
}

#endif // TCP_SERVER_EXCEPTION_H
