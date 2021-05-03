#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants
{
    enum Color
    {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHT_GRAY = 7,
        DARK_GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        YELLOW = 14,
        WHITE = 15
    };

    enum Key
    {
        PAGE_UP = 72,
        PAGE_DOWN = 80,
        ENTER = 13,
        BACKSPACE = 8,
    };

    enum Range
    {
        FIRST = 1,
        LAST_MAIN = 8,
        LAST_ACC_MENU = 9
    };
}

#endif // CONSTANTS_H
