#ifndef TCP_SERVER_EXCEPTION_H
#define TCP_SERVER_EXCEPTION_H

#include <stdexcept>

namespace TcpServerException // можно будет добавить к методу what() WSALastError
{
    class WinSockInitializationFailed : public std::runtime_error
    {
    public:
        WinSockInitializationFailed(const std::string& initError) : std::runtime_error(initError) {}
    };

    class GettingHostnameFailed : public std::runtime_error
    {
    public:
        GettingHostnameFailed(const std::string& hostnameError) : std::runtime_error(hostnameError) {}
    };

    class GettingHostnameDataFailed : public std::runtime_error
    {
    public:
        GettingHostnameDataFailed(const std::string& hostnameDataError) : std::runtime_error(hostnameDataError) {}
    };

    class GettingSocketFailed : public std::runtime_error
    {
    public:
        GettingSocketFailed(const std::string& socketError) : std::runtime_error(socketError) {}
    };

    class BindSocketFailed : public std::runtime_error
    {
    public:
        BindSocketFailed(const std::string& bindError) : std::runtime_error(bindError) {}
    };

    class RecvDataFailed : public std::runtime_error
    {
    public:
        RecvDataFailed(const std::string& recvError) : std::runtime_error(recvError) {}
    };

    class SendDataFailed : public std::runtime_error
    {
    public:
        SendDataFailed(const std::string& sendError) : std::runtime_error(sendError) {}
    };
}

#endif // TCP_SERVER_EXCEPTION_H
