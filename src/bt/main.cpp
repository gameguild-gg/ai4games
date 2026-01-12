// Minimal runner for Behavior Tree assignment
// Usage: ./bt < input.txt

#include "bt.h"
#include <iostream>
#include <sstream>

int main()
{
    std::ostringstream buf;
    buf << std::cin.rdbuf();
    std::string input = buf.str();

    try
    {
        std::string output = BT::runBT(input);
        std::cout << output;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
