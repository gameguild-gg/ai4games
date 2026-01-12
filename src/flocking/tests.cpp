#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include "flocking.hpp"
#include "MemoryLeakDetector.h"

namespace fs = std::filesystem;

// Helper function to normalize line endings (convert \r\n and \r to the standard linux style \n)
std::string normalizeLineEndings(const std::string& str) {
    std::string result = str;
    // Replace \r\n with \n first
    size_t pos = 0;
    while ((pos = result.find("\r\n", pos)) != std::string::npos) {
        result.replace(pos, 2, "\n");
        pos += 1;
    }
    // Replace remaining \r with \n
    pos = 0;
    while ((pos = result.find('\r', pos)) != std::string::npos) {
        result[pos] = '\n';
        pos += 1;
    }
    return result;
}

// Helper function to trim whitespace from both ends of a string
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// Helper function to compare floating point numbers with tolerance
bool isClose(double a, double b, double tolerance = 1e-6) {
    return std::abs(a - b) < tolerance;
}

// Helper function to run simulator with input and capture output
std::string runFlockingSimulation(const std::string& input) {
    std::istringstream inputStream(input);
    Simulator simulator(inputStream);
    simulator.Run();
    
    std::ostringstream outputStream;
    simulator.OutputStates(outputStream);

    return outputStream.str();
}

// Helper function to compare output strings with floating point tolerance
bool compareOutputs(const std::string& actual, const std::string& expected, double tolerance = 1e-3) {
    std::istringstream actualStream(actual);
    std::istringstream expectedStream(expected);
    
    double actualVal, expectedVal;
    while (actualStream >> actualVal && expectedStream >> expectedVal) {
        if (!isClose(actualVal, expectedVal, tolerance)) {
            return false;
        }
    }
    
    // Try to read one more value from each stream to trigger EOF
    double dummy;
    actualStream >> dummy;
    expectedStream >> dummy;
    
    // Check if both streams reached the end
    return actualStream.eof() && expectedStream.eof();
}

// Test function template
void runTestCase(const std::string& testName, const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    std::ifstream outFile(outputFile);
    
    REQUIRE(inFile.is_open());
    REQUIRE(outFile.is_open());
    
    std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string expectedOutput((std::istreambuf_iterator<char>(outFile)), std::istreambuf_iterator<char>());
    
    // Normalize line endings and trim whitespace
    input = normalizeLineEndings(input);
    expectedOutput = normalizeLineEndings(expectedOutput);

    std::string actualOutput = runFlockingSimulation(input);
    
    INFO("Test case:\n" << testName);
    INFO("Input:\n" << input);
    INFO("Expected output:\n" << expectedOutput);
    INFO("Actual output:\n" << actualOutput);
    
    CHECK(compareOutputs(actualOutput, expectedOutput));
}

// Find test files in the tests directory
std::vector<std::pair<std::string, std::string>> findTestFiles() {
    std::vector<std::pair<std::string, std::string>> testFiles;
    
    // Use filesystem::current_path() for more reliable path detection
    fs::path currentPath = fs::current_path();

    // Try different possible relative paths for test files
    std::vector<fs::path> possiblePaths = {
        currentPath / "tests",                                    // Direct tests folder (new location)
        currentPath / "flocking" / "tests",
        currentPath / "src" / "flocking" / "tests",
        currentPath / ".." / "tests",                             // One level up
        currentPath / ".." / "flocking" / "tests",
        currentPath / ".." / "src" / "flocking" / "tests",
        currentPath / ".." / ".." / "src" / "flocking" / "tests"
    };
    
    fs::path testsDir;
    for (const auto& path : possiblePaths) {
        if (fs::exists(path) && fs::is_directory(path)) {
            testsDir = path;
            break;
        }
    }
    
    if (testsDir.empty()) {
        // If we can't find the directory, return empty vector
        return testFiles;
    }
    
    // Collect all .in files and match them with .out files
    try {
        for (const auto& entry : fs::directory_iterator(testsDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".in") {
                fs::path inputFile = entry.path();
                fs::path outputFile = testsDir / (entry.path().stem().string() + ".out");

                if (fs::exists(outputFile)) {
                    testFiles.emplace_back(inputFile.string(), outputFile.string());
                }
            }
        }
    } catch (const fs::filesystem_error& ex) {
        // Handle filesystem errors gracefully
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    // Sort test files for consistent ordering across platforms
    std::sort(testFiles.begin(), testFiles.end());

    return testFiles;
}

TEST_CASE("Flocking simulation tests") {
    auto testFiles = findTestFiles();
    
    if (testFiles.empty()) {
        WARN("No test files found. Make sure test files are copied to the build directory.");
        return;
    }
    
    for (const auto& [inputFile, outputFile] : testFiles) {
        std::string testName = fs::path(inputFile).stem().string();
        
        SUBCASE(testName.c_str()) {
            runTestCase(testName, inputFile, outputFile);
        }
    }
}