#ifndef MYSQL_API_H
#define MYSQL_API_H

#include <mysql.h>
#include <iostream>

class MySqlAPI
{
private:
    MYSQL* connection;
    MYSQL_RES* resultOfSelect;
    std::string host;
    std::string user;
    std::string userPassword;
    std::string databaseName;
};

#endif // MYSQL_API_H
