#ifndef MYSQL_API_H
#define MYSQL_API_H

#include "mysql_api_exception.h"
#include <mysql.h>
#include <iostream>

class MySqlAPI
{
private:
    MYSQL* getInstance()
    {
        MYSQL* tmpConnection = mysql_init(nullptr);

        if (tmpConnection == nullptr)
        {
            throw MySqlException::InitializationFailed(mysql_error(connection), mysql_errno(connection));
        }

        return tmpConnection;
    }
    void createConnection()
    {
        if (mysql_real_connect(connection, host.c_str(), user.c_str(), userPassword.c_str(), databaseName.c_str(), 0L, nullptr, 0L) == nullptr)
        {
            throw MySqlException::ConnectionFailed(mysql_error(connection), mysql_errno(connection));
        }
    }

public:
    MySqlAPI(const std::string& hst, const std::string& usr, const std::string& password, const std::string& dbname) : host(hst), user(usr), userPassword(password), databaseName(dbname)
    {
        connection = getInstance();
        createConnection();
    }

    ~MySqlAPI()
    {
        mysql_close(connection);
    }

    void execQuery(const std::string& command)
    {
       if (mysql_query(connection, command.c_str()) != 0)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }
    }

private:
    MYSQL* connection;
    MYSQL_RES* resultOfSelect;
    std::string host;
    std::string user;
    std::string userPassword;
    std::string databaseName;
};

#endif // MYSQL_API_H
