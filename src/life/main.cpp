#include "MemoryLeakDetector.h"
#include "life.hpp"
#include <iostream>
#include <limits>
using namespace std;

int main() {
  int32_t columns, lines, steps;
  // read header
  std::cin >> columns >> lines >> steps;

  std::vector<bool> data;
  // go to next line
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  // read entry char by char
  for (int l = 0; l < lines; ++l) {
    for (int c = 0; c < columns; ++c) {
      char e;
      std::cin >> e;
      if (e == '.')
        data.push_back(false);
      else if (e == '#')
        data.push_back(true);
      else
        throw std::runtime_error("Invalid input");
    }
    // go to next line
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  Life life(columns, lines, data);
  life.run(steps);
  std::cout << life.toString();
  return 0;
}