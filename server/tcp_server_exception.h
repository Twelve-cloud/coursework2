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
}

#endif // TCP_SERVER_EXCEPTION_H
