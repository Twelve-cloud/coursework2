#ifndef MYSQL_API_EXCEPTION_H
#define MYSQL_API_EXCEPTION_H

#include <stdexcept>

namespace MySqlException
{
    class ConnectionFailed : public std::runtime_error
    {
    public:
        ConnectionFailed(const std::string& connectionError, const std::size_t& errorCode) : std::runtime_error(connectionError + ", error code: " + std::to_string(errorCode)) {}
    };

    class InitializationFailed : public std::runtime_error
    {
    public:
        InitializationFailed(const std::string& connectionError, const std::size_t& errorCode) : std::runtime_error(connectionError + ", error code: " + std::to_string(errorCode)) {}
    };

    class ExecutionQueryFailed : public std::runtime_error
    {
    public:
        ExecutionQueryFailed(const std::string& connectionError, const std::size_t& errorCode) : std::runtime_error(connectionError + ", error code: " + std::to_string(errorCode)) {}
    };
}

#endif // MYSQL_API_EXCEPTION_H
