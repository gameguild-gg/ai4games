#include <cstdint>
#include <string>

struct Random
{
private:
    static const inline uint8_t randomNumbers[100] = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7, 30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60, 28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53, 51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13, 12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};
    static inline uint8_t index = 0;

public:
    static uint8_t next()
    {
        // todo: return the next random number from the array
        return 0;
    }

    static void setIndex(uint8_t i)
    {
        index = i;
    }
};

struct Maze
{
    size_t width, height, randomIndex;

    // todo: store the data in a meaningful way for you. I recommend a vector of bools and use matrix linearization technique
    // tip: I recommend to store the walls(not the cells) as horizontals and verticals in separate vectors to make your life easier

    Maze(size_t width, size_t height, uint8_t index) : width(width), height(height), randomIndex(index) {}

    /* In order to give consistency on how to decide the direction of the next cell, the following procedure should be followed:
     * List all visitable neighbors of the current cell;
     * Sort the list of visitable neighbors by clockwise order, starting from the top neighbor: UP, RIGHT, DOWN, LEFT;
     * If there is one visitable, do not call random, just return the first neighbor found;
     * If there are two or more visitable neighbors, call random and return the neighbor at the index of the random number modulo the number of visitable neighbors. vec[i]%visitableCount
     * this function should generate the full maze and store as the current state.
     */
    void generate()
    {
        // besides the walls, we need to store a way to check if the cell is visited or not. you may want to use a linearized matrix as vector of bools
        // you should keep a stack of your current path so far, so you can backtrack when you encounter a dead end
        // remember to (re)set the index at the begining of the maze generation
    }

    // print to the specific output stream
    std::string print() const
    {
        return "";
    }
};