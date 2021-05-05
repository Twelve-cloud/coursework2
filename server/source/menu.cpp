#include "header/streamtable.h"
#include "header/constants.h"
#include <cstdint>
#include <windows.h>
#include <conio.h>

using namespace Constants;
extern StreamTable serverTable;

void renderMenu(void (*renderMenuAndHighlightItem)(std::size_t), std::size_t RangeLast, std::size_t& menuItem)
{
    unsigned char cursor;
    renderMenuAndHighlightItem(menuItem);

    do
    {
        cursor = getch();

        if (cursor == Key::PAGE_UP && menuItem > Range::FIRST)
        {
           --menuItem;
        }
        else if (cursor == Key::PAGE_DOWN && menuItem < RangeLast)
        {
            ++menuItem;
        }

        renderMenuAndHighlightItem(menuItem);
    } while (cursor != Key::ENTER);
}

void serverMenu(std::size_t menuItem)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    serverTable.firstCell(true);

    static std::vector<std::string> items = { "Server start", "Work with data", "Get mysql API info", "Get server info", "Get clients", "Get lib info", "Stop server", "Exit" };

    for (std::size_t i = 0; i < items.size(); ++i)
    {
        if (i == menuItem - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            serverTable << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            serverTable << items[i];
        }
    }
}

void accountsMenu(std::size_t menuItem)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    serverTable.firstCell(true);

    static std::vector<std::string> items = { "Add account", "Ban account", "Unban account", "Delete account", "Change account data", "Show accounts", "Find account", "Sort accounts", "Back" };

    for (std::size_t i = 0; i < items.size(); ++i)
    {
        if (i == menuItem - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            serverTable << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            serverTable << items[i];
        }
    }
}


void rolesMenu(std::size_t menuItem)
{
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    serverTable.firstCell(true);

    static std::vector<std::string> items = { "USER", "CONSULTANT", "BROKER", "Back" };

    for (std::size_t i = 0; i < items.size(); ++i)
    {
        if (i == menuItem - 1)
        {
            SetConsoleTextAttribute(hConsole, (WORD)((Color::LIGHT_BLUE << Color::RED) | Color::WHITE));
            serverTable << items[i];
            SetConsoleTextAttribute(hConsole, (WORD)((Color::BLACK << Color::RED) | Color::LIGHT_BLUE));
        }
        else
        {
            serverTable << items[i];
        }
    }
}
