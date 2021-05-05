#include "header/mysql_api.h"
#include "header/constants.h"

extern MySqlAPI database;

using namespace Constants;

void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void accountsMenu(std::size_t menuItem);
void rolesMenu(std::size_t menuItem);

void addAccount();
void deleteAccount();
void banAccount();
void unbanAccount();
void changeAccountData();
void showAccounts();
void findAccount();
void sortAccounts();

void workWithAccounts()
{
    database.connect();
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
            case 6: showAccounts(); break;
            case 7: findAccount(); system("pause"); break;
            case 8: sortAccounts(); system("pause"); break;
            case 9: isEnd = true;
        }

    } while (!isEnd);

    database.disconnect();
}

void addAccount()
{
    bool isEnd = false;
    std::size_t menuItem = Range::FIRST;
    std::string login, password, role;

    do
    {
        renderMenu(rolesMenu, Range::LAST_ACC_MENU, menuItem);

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

    database.execQuery("INSERT INTO Account(AccountLogin, AccountPassword, Rolename) VALUES ('" + login + "', '" + password + "', '" + role + "');");
    system("pause");
}

void banAccount()
{

}

void unbanAccount()
{

}

void deleteAccount()
{

}

void changeAccountData()
{

}

void showAccounts()
{
    database.select("SELECT * FROM Account");
    system("pause");
}

void findAccount()
{

}

void sortAccounts()
{

}
