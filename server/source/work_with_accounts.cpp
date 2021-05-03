#include "header/mysql_api.h"
#include "header/constants.h"

extern MySqlAPI database;

using namespace Constants;

void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem);
void accountsMenu(std::size_t menuItem);

void workWithAccounts()
{
    bool isEnd;
    std::size_t menuItem = Range::FIRST;

    do
    {
        renderMenu(accountsMenu, Range::LAST_MAIN, menuItem);

        switch (menuItem)
        {
            case 1: break;
            case 2: break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: database.connect(); database.select("SELECT * FROM Account"); database.disconnect(); system("pause"); break;
            case 7: break;
            case 8: break;
            case 9: isEnd = true;
        }

    } while (!isEnd);
}
