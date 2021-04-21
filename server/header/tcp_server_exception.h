#ifndef TCP_SERVER_EXCEPTION_H
#define TCP_SERVER_EXCEPTION_H

#include "winsock.h"
#include <stdexcept>

namespace TcpServerException
{
    class WinSockInitializationFailed : public std::runtime_error
    {
    public:
        WinSockInitializationFailed(const std::string& initError, const std::size_t& errorCode) : std::runtime_error(initError + ", error code: " + std::to_string(errorCode)) {}
    };

    class GettingHostnameFailed : public std::runtime_error
    {
    public:
        GettingHostnameFailed(const std::string& hostnameError, const std::size_t& errorCode) : std::runtime_error(hostnameError + ", error code: " + std::to_string(errorCode)) {}
    };

    class GettingHostnameDataFailed : public std::runtime_error
    {
    public:
        GettingHostnameDataFailed(const std::string& hostnameDataError, const std::size_t& errorCode) : std::runtime_error(hostnameDataError + ", error code: " + std::to_string(errorCode)) {}
    };

    class GettingSocketFailed : public std::runtime_error
    {
    public:
        GettingSocketFailed(const std::string& socketError, const std::size_t& errorCode) : std::runtime_error(socketError + ", error code: " + std::to_string(errorCode)) {}
    };

    class BindSocketFailed : public std::runtime_error
    {
    public:
        BindSocketFailed(const std::string& bindError, const std::size_t& errorCode) : std::runtime_error(bindError + ", error code: " + std::to_string(errorCode)) {}
    };

    class RecvDataFailed : public std::runtime_error
    {
    public:
        RecvDataFailed(const std::string& recvError, const std::size_t& errorCode) : std::runtime_error(recvError + ", error code: " + std::to_string(errorCode)) {}
    };

    class SendDataFailed : public std::runtime_error
    {
    public:
        SendDataFailed(const std::string& sendError, const std::size_t& errorCode) : std::runtime_error(sendError + ", error code: " + std::to_string(errorCode)) {}
    };
}

#endif // TCP_SERVER_EXCEPTION_H
