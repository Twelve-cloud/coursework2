#include <string>
#include <cstdint>
#include <cstdarg>
#include <cstring>

void getFields(std::string& str, const std::size_t& argc, ...)
{

    va_list list;
    va_start(list, str);

    for (std::size_t i = 0; i < argc - 1; i++)
    {
        std::string::size_type indexOfTheEndOfTheField = str.find("~~~");

        if (indexOfTheEndOfTheField != std::string::npos)
        {
            std::strcpy(va_arg(list, char*), str.substr(0, indexOfTheEndOfTheField).c_str());
            str.erase(0, indexOfTheEndOfTheField + 3);
        }
        else
        {
            std::strcpy(va_arg(list, char*), str.c_str());
        }
    }

    va_end(list);
}
