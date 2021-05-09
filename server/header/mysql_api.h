#ifndef MYSQL_API_H
#define MYSQL_API_H

#include "mysql_api_exception.h"
#include "streamtable.h"
#include <mysql.h>
#include <iostream>
#include <cstring>

extern StreamTable mysqlConfigTable;
extern StreamTable selectTable;

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

    void setOrderParams(const std::string& orderBy_, const std::string& direction_)
    {
        orderBy = orderBy_;
        direction = direction_;
    }

    std::string getOrderParams() const
    {
        return (orderBy + " " + direction);
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

       if ((resultOfSelect = mysql_store_result(connection)) == nullptr)
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

       while ((row = mysql_fetch_row(resultOfSelect)))
       {
           for (std::size_t i = 0; i < numFields; i++)
           {
               selectTable << (row[i] ? row[i] : "NULL");
           }
       }
    }

    std::string select(const std::string& command, bool)
    {
        std::string resOfQuery;

       if (mysql_query(connection, command.c_str()) != 0)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       if ((resultOfSelect = mysql_store_result(connection)) == nullptr)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       if (mysql_num_rows(resultOfSelect) == 0)
       {
           return "";
       }

       std::size_t numFields = mysql_num_fields(resultOfSelect);
       MYSQL_ROW row;

       while ((row = mysql_fetch_row(resultOfSelect)))
       {
           for (std::size_t i = 0; i < numFields; i++)
           {
               resOfQuery += (row[i] ? row[i] : "NULL");

           }
       }

       return resOfQuery;
    }

    bool isExists(const std::string& command)
    {
       if (mysql_query(connection, command.c_str()) != 0)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       if ((resultOfSelect = mysql_store_result(connection)) == nullptr)
       {
           throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
       }

       if (mysql_num_rows(resultOfSelect) == 0)
       {
           std::cout << "Query has returned no data" << std::endl;
           return false;
       }

       std::size_t numFields = mysql_num_fields(resultOfSelect);
       MYSQL_ROW row;

       std::string exists;
       while ((row = mysql_fetch_row(resultOfSelect)))
       {
           for (std::size_t i = 0; i < numFields; i++)
           {
              exists += (row[i] ? row[i] : "NULL");
           }
       }

       if (exists == "1")
       {
           return true;
       }

       return false;
    }

    void select(const std::string& command, const std::size_t& argc, ...)
    {
        if (mysql_query(connection, command.c_str()) != 0)
        {
            throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
        }

        if ((resultOfSelect = mysql_store_result(connection)) == nullptr)
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

    bool isExists(const std::string& table, const std::string& field, const std::string& target)
    {
        if (mysql_query(connection, std::string("SELECT * FROM " + table + " WHERE " + field + " = '" + target + "';").c_str()) != 0)
        {
            throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
        }

        if ((resultOfSelect = mysql_store_result(connection)) == nullptr)
        {
            throw MySqlException::ExecutionQueryFailed(mysql_error(connection), mysql_errno(connection));
        }

        if (mysql_num_rows(resultOfSelect) == 0)
        {
            return false;
        }

        return true;
    }

    void showInfo()
    {
        mysqlConfigTable << "Version: " << mysql_get_client_info();
        mysqlConfigTable << "Host: " << mysql_get_host_info(connection);
        mysqlConfigTable << "Server version: " << mysql_get_server_info(connection);
        mysqlConfigTable << "Protocol version: " << mysql_get_proto_info(connection);
    }

private:
    MYSQL* connection;
    MYSQL_RES* resultOfSelect;
    std::string host;
    std::string user;
    std::string userPassword;
    std::string databaseName;
    std::string orderBy = "ID";
    std::string direction = "ASC";
};

#endif // MYSQL_API_H
