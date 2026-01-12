#include <iostream>
#include "flocking.hpp"
#include "MemoryLeakDetector.h"
using namespace std;

// feel free to edit this main function to meet your needs
int main() {
  Simulator simulator(cin);
  simulator.Run();
  simulator.OutputStates(cout);
  return 0;
}
