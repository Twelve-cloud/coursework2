#ifndef MYSQL_API_H
#define MYSQL_API_H

#include "mysql_api_exception.h"
#include <mysql.h>
#include <iostream>
#include <cstring>

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
    MySqlAPI(const std::string& hst, const std::string& usr, const std::string& password, const std::string& dbname) : host(hst), user(usr), userPassword(password), databaseName(dbname) {}

    ~MySqlAPI()
    {
        mysql_close(connection);
    }

    void connect()
    {
        connection = getInstance();
        createConnection();
    }

    void disconnect()
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

    void select(const std::string& command)
    {
       if (mysql_query(connection, command.c_str()) != 0)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       resultOfSelect = mysql_store_result(connection);

       if (resultOfSelect == nullptr)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       if (mysql_num_rows(resultOfSelect) == 0)
       {
           std::cout << "Query has returned no data" << std::endl;
           return;
       }

       std::size_t numFields = mysql_num_fields(resultOfSelect);
       MYSQL_ROW row;

       std::cout << "Result of SELECT" << std::endl;
       while ((row = mysql_fetch_row(resultOfSelect)))
       {
           for (std::size_t i = 0; i < numFields; i++)
           {
               std::cout << "[" << (row[i] ? row[i] : "NULL") << "] ";
           }
           std::cout << std::endl;
       }
    }

    void select(const std::string& command, const std::size_t& argc, ...)
    {
        if (mysql_query(connection, command.c_str()) != 0)
        {
            throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
        }

        resultOfSelect = mysql_store_result(connection);

        if (resultOfSelect == nullptr)
        {
            throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
        }

        if (mysql_num_rows(resultOfSelect) == 0)
        {
            std::cout << "Query has returned no data" << std::endl;
            return;
        }

        std::size_t numFields = mysql_num_fields(resultOfSelect);
        MYSQL_ROW row;

        std::string allFields;

        while ((row = mysql_fetch_row(resultOfSelect)))
        {
            for (std::size_t i = 0; i < numFields; i++)
            {
                allFields += std::string((row[i] ? row[i] : "NULL")) + "~~~";
            }
        }

        va_list list;
        va_start(list, allFields);

        for (std::size_t i = 0; i < argc - 1; i++)
        {
            std::string::size_type indexOfTheEndOfTheField = allFields.find("~~~");

            if (indexOfTheEndOfTheField != std::string::npos)
            {
                std::strcpy(va_arg(list, char*), allFields.substr(0, indexOfTheEndOfTheField).c_str());
                allFields.erase(0, indexOfTheEndOfTheField + 3);
            }
        }

        va_end(list);
    }

    void showLibraryVersion()
    {
        std::cout << "Version: " << mysql_get_client_info() << std::endl;
    }

    void showConnectionInfo()
    {
        std::cout << "Host: " << mysql_get_host_info(connection) << std::endl;
        std::cout << "Server version: " << mysql_get_server_info(connection) << std::endl;
        std::cout << "Protocol version: " << mysql_get_proto_info(connection) << std::endl;
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
