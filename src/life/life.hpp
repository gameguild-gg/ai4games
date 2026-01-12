#pragma once
#include "MemoryLeakDetector.h"
#include <cstdint>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Life {
private:
  // store your data sturcture here
  uint32_t lines = 0, columns = 0;

  // todo: store the state properly. you may want to use vector<vector<bool>> as
  // matrix, or linearize the matrix and use only vector<bool>, or even use
  // hashtables to exploit sparce scenarios. todo: remember to double buffer the
  // state, so that you can calculate the next state without modifying the
  // current state. bonus: if you follow the hashtable approach and exploit
  // sparsity of elements. remember to state that on canvas hint 1: be aware
  // about the memory consumption of hashtables structures such as map, set,
  // unordered_map and unordered_set hint 2: vector<bool> is not a vector. It IS
  // memory efficient(8x) but it IS NOT that fast(0.25x??) Watch
  // `https://www.youtube.com/watch?v=OP9IDIeicZE`

public:
  Life(uint32_t columns, uint32_t lines, std::vector<bool> cells)
      : lines(lines), columns(columns) {
    // todo: process cells and store it properly, if you want to pursue bonus
    // with hashtables, pay attention to proper initialization
    for (auto lin = 0; lin < lines; ++lin) {
      for (auto col = 0; col < columns; ++col) {
        // store properly the data, in case of matrix linearization in
        // vector<bool>ex.: data[lin*columns + col] = cells[lin*columns + col];
      }
    }
  }

  // get the boolean at position y, x.
  // hint: y and x are in that way to speedup matrix memory lookup
  bool get(size_t y, size_t x) const {
    // todo: implement the logic to access the cell properly
    // hint: remember, if you use vector<bool>, it is the only vector<T> that
    // does not return T& on the subscript operator return data[x][y];

    // todo: change this
    return false;
  }

  void set(size_t y, size_t x, bool value) {
    // todo: code. you should set data on the new state, not the current one!
    // bonus: if you implement via hashtables, you may want to update the
    // vicinity of active nodes(not only live, or dead), the ones that might
    // become alive on next frame
  }

  void swapBuffer() {
    // todo: implement the logic to swap the buffers
  }

  // Count the ALIVE neighbors for a cell at position (y, x)
  // IMPORTANT: This implementation uses toroidal topology (wrap-around boundaries)
  // where cells on edges have opposite edge cells as neighbors, creating a continuous surface
  int countNeighbors(uint32_t y, uint32_t x) const{
    return 0;
  };

  void step() {
    // todo: implement your logic here. remember to read from the current state,
    // and store at the newstate. at the end swap buffer bonus: if you exploit
    // sparsity you will be granted extra points. remember to state that on
    // canvas!
  }

  // run steps N times
  void run(uint32_t steps) {
    for (auto s = 0; s < steps; ++s)
      step();
  }

  // in order to print the current state
  std::string toString() {
    std::stringstream ss;
    for (size_t i = 0; i < lines; i++) {
      for (size_t j = 0; j < columns; j++) {
        bool entry = get(i, j);
        if (entry)
          ss << '#';
        else
          ss << '.';
      }
      ss << std::endl;
    }
    return ss.str();
  }

  // to be used in tests
  std::vector<bool> toBits() {
    std::vector<bool> vet;
    for (auto y = 0; y < lines; y++) {
      for (auto x = 0; x < columns; x++) {
        vet.push_back(get(y, x));
      }
    }
    return vet;
  }
};
