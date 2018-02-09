
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

bool RunPrintMessage(const char* message)
{
    printf("Message: %s\n", message);
    return true;
}

int main(int argc, char const* argv[])
{
    std::string arguments[3];
    switch (argc)
    {
    case 4:
        arguments[2] = argv[3];
    case 3:
        arguments[1] = argv[2];
    case 2:
        arguments[0] = argv[1];
        break;
    default:
        break;
    }

    if (!arguments[0].compare("print_message"))
        RunPrintMessage(arguments[1].c_str());
    else
        std::cout << "Invalid Command (" << argc << "):\n\t" << arguments[0] << "\nOptions:\n"
                  << "\tprint_message <message>\t\t\t\t\t-E.g.: print_message Hello\n"
                  << "\n";

    return 0;
}

