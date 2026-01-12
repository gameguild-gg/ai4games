#include <iostream>
#include <vector>
#include <stack>
#include "maze.hpp"

using namespace std;

int main()
{
    size_t width, height, index;
    std::cin >> width >> height >> index;
    Maze maze(width, height, index);
    maze.generate();
    std::cout << maze.print() << std::endl;
}
