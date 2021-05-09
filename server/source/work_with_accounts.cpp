#include "header/mysql_api.h"
#include "header/constants.h"

extern MySqlAPI database;

using namespace Constants;
extern StreamTable selectTable;

void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void accountsMenu(std::size_t menuItem);
void rolesMenu(std::size_t menuItem);
void sortMenu(std::size_t menuItem);

void addAccount();
void deleteAccount();
void banAccount();
void unbanAccount();
void changeAccountData();
void showAccounts();
void showBanList();
void findAccount();
void sortAccounts();

void workWithAccounts()
{
    bool isEnd = false;
    std::size_t menuItem = Range::FIRST;

    do
    {
        renderMenu(accountsMenu, Range::LAST_ACC_MENU, menuItem);

        switch (menuItem)
        {
            case 1: addAccount(); break;
            case 2: banAccount(); system("pause"); break;
            case 3: unbanAccount(); system("pause"); break;
            case 4: deleteAccount(); system("pause"); break;
            case 5: changeAccountData(); system("pause"); break;
            case 6: showAccounts(); system("pause"); break;
            case 7: showBanList(); system("pause"); break;
            case 8: findAccount(); system("pause"); break;
            case 9: sortAccounts(); break;
            case 10: isEnd = true;
        }

    } while (!isEnd);

}

void addAccount() try
{
    bool isEnd = false;
    std::size_t menuItem = Range::FIRST;
    std::string login, password, role;

    do
    {
        renderMenu(rolesMenu, Range::LAST_ROLES_MENU, menuItem);

        switch (menuItem)
        {
            case 1: role = "USER"; isEnd = true; break;
            case 2: role = "CONSULTANT"; isEnd = true; break;
            case 3: role = "BROKER"; isEnd = true; break;
            case 4: return;
        }

    } while (!isEnd);

    std::cout << "Enter login: "; std::getline(std::cin, login);
    std::cout << "Enter password: "; std::getline(std::cin, password);

    std::string existsLogin = database.select("SELECT AccountLogin FROM Account WHERE AccountLogin = '" + login + "';", true);

    if (existsLogin == "")
    {
        database.execQuery("INSERT INTO Account(AccountLogin, AccountPassword, Rolename) VALUES ('" + login + "', '" + password + "', '" + role + "');");
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "This login is already taken" << std::endl;
    }
    system("pause");
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
   system("pause");
}

void banAccount() try
{
    std::string login, started, ended;
    std::cout << "Enter login: "; std::getline(std::cin, login);

    login = database.select("SELECT AccountLogin FROM Account WHERE AccountLogin = '" + login + "';", true);

    if (login != "")
    {
        std::cout << "Enter ban begin: "; std::getline(std::cin, started);
        std::cout << "Enter ban end: "; std::getline(std::cin, ended);

        database.execQuery("INSERT INTO BanList(AccountID, started, ended) VALUES ((SELECT ID FROM Account WHERE AccountLogin = '" + login + "'), '" + started + "', '" + ended + "');");
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Account not found" << std::endl;
    }
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}
void unbanAccount()  try
{
    std::string login;
    std::cout << "Enter login: "; std::getline(std::cin, login);
    login = database.select("SELECT AccountLogin FROM Account WHERE AccountLogin = '" + login + "';", true);


    if (login != "")
    {
        database.execQuery("DELETE FROM BanList WHERE AccountID = (SELECT ID FROM Account WHERE AccountLogin = '" + login + "');");
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Account not found" << std::endl;
    }
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}

void deleteAccount() try
{
    std::string login;
    std::cout << "Enter login: "; std::getline(std::cin, login);
    login = database.select("SELECT AccountLogin FROM Account WHERE AccountLogin = '" + login + "';", true);


    if (login != "")
    {
        database.execQuery("DELETE FROM Account WHERE AccountLogin = '" + login + "';");
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Account not found" << std::endl;
    }
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}

void changeAccountData() try
{
    std::string old_login;
    std::cout << "Enter old login: "; std::getline(std::cin, old_login);
    old_login = database.select("SELECT AccountLogin FROM Account WHERE AccountLogin = '" + old_login + "';", true);

    if (old_login != "")
    {
        std::string login, password, mnumber, email, rolename;
        std::cout << "Enter login: "; std::getline(std::cin, login);
        std::cout << "Enter password: "; std::getline(std::cin, password);
        std::cout << "Enter mobile number: "; std::getline(std::cin, mnumber);
        std::cout << "Enter email: "; std::getline(std::cin, email);
        std::cout << "Enter rolename: "; std::getline(std::cin, rolename);

        database.execQuery("UPDATE Account SET AccountLogin = '" + login + "', AccountPassword = '" + password + "', MobileNumber = '" + mnumber + "', Email = '" + email + "', Rolename = '" + rolename + "' WHERE AccountLogin = '" + old_login + "';");
    }
    else
    {
        std::cout << "Account not found" << std::endl;
    }
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}

void showAccounts() try
{
    selectTable.SetCols(6, 20);
    database.select("SELECT * FROM Account ORDER BY " + database.getOrderParams());
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}
void showBanList() try
{
    selectTable.SetCols(4, 20);
    database.select("SELECT * FROM BanList");
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}

void findAccount() try
{
    std::string data;
    std::cout << "Enter data: "; std::getline(std::cin, data);
    database.select("SELECT * FROM Account WHERE AccountLogin = '" + data + "' OR AccountPassword = '" + data + "' OR MobileNumber = '" + data + "' OR Email = '" + data + "' OR Rolename = '" + data + "';");
}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
}


void sortAccounts() try
{
    bool isEnd = false;
    std::size_t menuItem = Range::FIRST;
    std::string orderBy, direction;

    do
    {
        renderMenu(sortMenu, Range::LAST_SORT_MENU, menuItem);

        switch (menuItem)
        {
            case 1: orderBy = "ID"; break;
            case 2: orderBy = "AccountLogin"; break;
            case 3: orderBy = "AccountPassword"; break;
            case 4: orderBy = "MobileNumber"; break;
            case 5: orderBy = "Email"; break;
            case 6: orderBy = "Rolename"; break;
            case 7: return;
        }

        std::cout << "Enter direction (ASC, DESC): ";
        std::getline(std::cin, direction);

        if (direction != "ASC" && direction != "DESC")
        {
            std::cout << "Invalid direction" << std::endl;
            system("pause");
            continue;
        }

        database.setOrderParams(orderBy, direction);

        std::cout << "Success" << std::endl;
        system("pause");

    } while (!isEnd);

}
catch (const MySqlException::ExecutionQueryFailed& error)
{
   std::cout << error.what() << std::endl;
   system("pause");
}
