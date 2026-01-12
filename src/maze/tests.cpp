#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "MemoryLeakDetector.h"
#include "maze.hpp"
#include <doctest/doctest.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include <future>
#include <stdexcept>

namespace fs = std::filesystem;

// Helper function to normalize line endings (convert \r\n and \r to \n)
std::string normalizeLineEndings(const std::string &str)
{
    std::string result = str;
    // Replace \r\n with \n first
    size_t pos = 0;
    while ((pos = result.find("\r\n", pos)) != std::string::npos)
    {
        result.replace(pos, 2, "\n");
        pos += 1;
    }
    // Replace remaining \r with \n
    pos = 0;
    while ((pos = result.find('\r', pos)) != std::string::npos)
    {
        result[pos] = '\n';
        pos += 1;
    }
    return result;
}

// Helper function to trim whitespace from both ends of a string
std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// Helper function to normalize string by removing trailing spaces and empty lines with only spaces
std::string normalizeSpaces(const std::string &str)
{
    std::istringstream iss(str);
    std::string line;
    std::vector<std::string> lines;
    
    while (std::getline(iss, line)) {
        // Remove trailing spaces from the line
        size_t end = line.find_last_not_of(" \t");
        if (end != std::string::npos) {
            line = line.substr(0, end + 1);
            lines.push_back(line);
        } else if (line.empty()) {
            // Keep truly empty lines
            lines.push_back("");
        }
        // Skip lines that contain only spaces (they become empty after trimming)
    }
    
    // Remove trailing empty lines
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    
    // Join lines with newlines
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (i > 0) result += "\n";
        result += lines[i];
    }
    
    return result;
}

// Helper function to run Maze generation with input and capture output
std::string runMazeGeneration(const std::string &input)
{
    std::istringstream inputStream(input);

    size_t width, height;
    size_t index;
    inputStream >> width >> height >> index;

    Maze maze(width, height, index);
    maze.generate();

    return maze.print();
}

// Helper function to compare output strings
bool compareOutputs(const std::string &actual, const std::string &expected)
{
    std::string normalizedActual = normalizeSpaces(normalizeLineEndings(actual));
    std::string normalizedExpected = normalizeSpaces(normalizeLineEndings(expected));

    return normalizedActual == normalizedExpected;
}

// Test function template
void runTestCase(const std::string &testName, const std::string &inputFile,
                 const std::string &outputFile)
{
    std::ifstream inFile(inputFile);
    std::ifstream outFile(outputFile);

    REQUIRE(inFile.is_open());
    REQUIRE(outFile.is_open());

    std::string input((std::istreambuf_iterator<char>(inFile)),
                      std::istreambuf_iterator<char>());
    std::string expectedOutput((std::istreambuf_iterator<char>(outFile)),
                               std::istreambuf_iterator<char>());

    // Normalize line endings
    input = normalizeLineEndings(input);
    expectedOutput = normalizeLineEndings(expectedOutput);

    std::string actualOutput;
    try
    {
        actualOutput = runMazeGeneration(input);
    }
    catch (const std::exception &e)
    {
        FAIL("Exception during maze generation: " << e.what());
        return;
    }

    INFO("Test case: " << testName);
    INFO("Input:\n"
         << input);
    INFO("Expected output:\n"
         << expectedOutput);
    INFO("Actual output:\n"
         << actualOutput);

    CHECK(compareOutputs(actualOutput, expectedOutput));
}

// Find test files in the tests directory
std::vector<std::pair<std::string, std::string>> findTestFiles()
{
    std::vector<std::pair<std::string, std::string>> testFiles;

    // Use filesystem::current_path() for more reliable path detection
    fs::path currentPath = fs::current_path();

    // Try different possible relative paths for test files
    std::vector<fs::path> possiblePaths = {
        currentPath / "tests", // Direct tests folder (new location)
        currentPath / "maze" / "tests",
        currentPath / "src" / "maze" / "tests",
        currentPath / ".." / "tests", // One level up
        currentPath / ".." / "maze" / "tests",
        currentPath / ".." / "src" / "maze" / "tests",
        currentPath / ".." / ".." / "src" / "maze" / "tests"};

    fs::path testsDir;
    for (const auto &path : possiblePaths)
    {
        if (fs::exists(path) && fs::is_directory(path))
        {
            testsDir = path;
            break;
        }
    }

    if (testsDir.empty())
    {
        // If we can't find the directory, return empty vector
        return testFiles;
    }

    // Collect all .in files and match them with .out files
    try
    {
        for (const auto &entry : fs::directory_iterator(testsDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".in")
            {
                fs::path inputFile = entry.path();
                fs::path outputFile =
                    testsDir / (entry.path().stem().string() + ".out");

                if (fs::exists(outputFile))
                {
                    testFiles.emplace_back(inputFile.string(), outputFile.string());
                }
            }
        }
    }
    catch (const fs::filesystem_error &ex)
    {
        // Handle filesystem errors gracefully
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    // Sort test files for consistent ordering across platforms
    std::sort(testFiles.begin(), testFiles.end());

    return testFiles;
}

TEST_CASE("Basic Maze functionality tests")
{
    SUBCASE("Random class functionality")
    {
        Random::setIndex(0);
        uint8_t first = Random::next();
        CHECK(first == 72); // First value in the array

        uint8_t second = Random::next();
        CHECK(second == 99); // Second value in the array
    }

    SUBCASE("Random setIndex to different position")
    {
        Random::setIndex(5);
        uint8_t value = Random::next();
        CHECK(value == 62); // Value at index 5
    }
}

TEST_CASE("Maze constructor")
{
    SUBCASE("Basic constructor parameters")
    {
        Maze maze(5, 5, 0);
        CHECK(maze.width == 5);
        CHECK(maze.height == 5);
    }

    SUBCASE("Different dimensions")
    {
        Maze maze(10, 3, 1);
        CHECK(maze.width == 10);
        CHECK(maze.height == 3);
    }

    SUBCASE("Single cell maze")
    {
        Maze maze(1, 1, 0);
        CHECK(maze.width == 1);
        CHECK(maze.height == 1);
    }

    SUBCASE("Large maze dimensions")
    {
        Maze maze(100, 100, 0);
        CHECK(maze.width == 100);
        CHECK(maze.height == 100);
    }
}

TEST_CASE("Maze generate method")
{
    SUBCASE("Generate does not crash on small maze")
    {
        Maze maze(2, 2, 0);
        CHECK_NOTHROW(maze.generate());
    }

    SUBCASE("Generate does not crash on medium maze")
    {
        Maze maze(5, 5, 0);
        CHECK_NOTHROW(maze.generate());
    }

    SUBCASE("Generate does not crash on rectangular maze")
    {
        Maze maze(10, 3, 0);
        CHECK_NOTHROW(maze.generate());
    }

    SUBCASE("Generate does not crash on single cell")
    {
        Maze maze(1, 1, 0);
        CHECK_NOTHROW(maze.generate());
    }
    
    SUBCASE("Generate with different random seeds") {
        Maze maze1(5, 5, 0);
        Maze maze2(5, 5, 1);
        CHECK_NOTHROW(maze1.generate());
        CHECK_NOTHROW(maze2.generate());
    }
}

TEST_CASE("Maze print method") {
    SUBCASE("Print returns string") {
        Maze maze(3, 3, 0);
        maze.generate();
        std::string result = maze.print();
        CHECK(result.length() > 0);
    }

    SUBCASE("Deterministic generation with same seed")
    {
        Maze maze1(3, 3, 0);
        maze1.generate();
        std::string result1 = maze1.print();

        Maze maze2(3, 3, 0);
        maze2.generate();
        std::string result2 = maze2.print();

        CHECK(result1 == result2);
    }
    
    SUBCASE("Print different seeds produce different results") {
        Maze maze1(5, 5, 0);
        maze1.generate();
        std::string result1 = maze1.print();
        
        Maze maze2(5, 5, 1);
        maze2.generate();
        std::string result2 = maze2.print();
        
        CHECK(result1 != result2);
    }
    
    SUBCASE("Single cell maze print") {
        Maze maze(1, 1, 0);
        maze.generate();
        std::string result = maze.print();
        CHECK(result.length() > 0);
    }
}

TEST_CASE("Maze integration tests") {
    SUBCASE("Complete workflow - construct, generate, print") {
        Maze maze(4, 4, 0);
        CHECK_NOTHROW(maze.generate());
        std::string output = maze.print();
        CHECK(output.length() > 0);
    }
    
    SUBCASE("Multiple mazes with same parameters") {
        for (int i = 0; i < 5; ++i) {
            Maze maze(3, 3, 0);
            CHECK_NOTHROW(maze.generate());
            std::string output = maze.print();
            CHECK(output.length() > 0);
        }
    }
    
    SUBCASE("Edge case dimensions") {
        // Very wide maze
        Maze wide_maze(20, 1, 0);
        CHECK_NOTHROW(wide_maze.generate());
        CHECK(wide_maze.print().length() > 0);
        
        // Very tall maze
        Maze tall_maze(1, 20, 0);
        CHECK_NOTHROW(tall_maze.generate());
        CHECK(tall_maze.print().length() > 0);
    }
}

TEST_CASE("Random number sequence validation") {
    SUBCASE("Random sequence is deterministic") {
        Random::setIndex(0);
        std::vector<uint8_t> sequence1;
        for (int i = 0; i < 10; ++i) {
            sequence1.push_back(Random::next());
        }
        
        Random::setIndex(0);
        std::vector<uint8_t> sequence2;
        for (int i = 0; i < 10; ++i) {
            sequence2.push_back(Random::next());
        }
        
        CHECK(sequence1 == sequence2);
    }
    
    SUBCASE("Random values are within expected range") {
        Random::setIndex(0);
        for (int i = 0; i < 20; ++i) {
            uint8_t value = Random::next();
            CHECK(value <= 99); // All values should be 0-99
        }
    }
}

TEST_CASE("Maze boundary conditions") {
    SUBCASE("Minimum size maze") {
        Maze maze(1, 1, 0);
        CHECK(maze.width == 1);
        CHECK(maze.height == 1);
        CHECK_NOTHROW(maze.generate());
        CHECK(maze.print().length() > 0);
    }
    
    SUBCASE("Rectangular mazes") {
        // Test various rectangular configurations
        std::vector<std::pair<size_t, size_t>> dimensions = {
            {2, 5}, {5, 2}, {10, 1}, {1, 10}, {3, 7}, {7, 3}
        };
        
        for (auto& dim : dimensions) {
            Maze maze(dim.first, dim.second, 0);
            CHECK(maze.width == dim.first);
            CHECK(maze.height == dim.second);
            CHECK_NOTHROW(maze.generate());
            CHECK(maze.print().length() > 0);
        }
    }
}

TEST_CASE("Performance and stress tests") {
    SUBCASE("Large maze generation") {
        Maze large_maze(50, 50, 0);
        CHECK_NOTHROW(large_maze.generate());
        CHECK(large_maze.print().length() > 0);
    }
    
    SUBCASE("Multiple maze generations") {
        for (int seed = 0; seed < 10; ++seed) {
            Maze maze(10, 10, seed);
            CHECK_NOTHROW(maze.generate());
            std::string output = maze.print();
            CHECK(output.length() > 0);
        }
    }
}

TEST_CASE("File-based maze generation tests") {
    auto testFiles = findTestFiles();
    
    if (testFiles.empty()) {
        WARN("No test files found. Skipping file-based tests.");
        return;
    }
    
    for (const auto& testFile : testFiles) {
        const std::string& inputFile = testFile.first;
        const std::string& outputFile = testFile.second;
        
        // Extract test name from file path
        fs::path inputPath(inputFile);
        std::string testName = inputPath.stem().string();
        
        SUBCASE(("Test case: " + testName).c_str()) {
            runTestCase(testName, inputFile, outputFile);
        }
    }
}