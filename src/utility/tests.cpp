// File-based tests for Utility AI using doctest and tests/ fixtures
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "utility.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static std::string normalizeLineEndings(const std::string &str)
{
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find("\r\n", pos)) != std::string::npos)
    {
        result.replace(pos, 2, "\n");
        pos += 1;
    }
    pos = 0;
    while ((pos = result.find('\r', pos)) != std::string::npos)
    {
        result[pos] = '\n';
        pos += 1;
    }
    return result;
}

static std::string normalizeSpaces(const std::string &str)
{
    std::istringstream iss(str);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(iss, line))
    {
        size_t end = line.find_last_not_of(" \t");
        if (end != std::string::npos)
        {
            lines.push_back(line.substr(0, end + 1));
        }
        else if (line.empty())
        {
            lines.push_back("");
        }
    }
    while (!lines.empty() && lines.back().empty())
    {
        lines.pop_back();
    }
    std::string result;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (i > 0)
            result += "\n";
        result += lines[i];
    }
    return result;
}

static bool compareOutputs(const std::string &actual, const std::string &expected)
{
    std::string normActual = normalizeSpaces(normalizeLineEndings(actual));
    std::string normExpected = normalizeSpaces(normalizeLineEndings(expected));
    return normActual == normExpected;
}

static std::vector<std::pair<std::string, std::string>> findTestFiles()
{
    std::vector<std::pair<std::string, std::string>> files;
    fs::path cwd = fs::current_path();

    // Get the path to the source file to find tests relative to it
    fs::path exePath = fs::path(__FILE__).parent_path();

    std::vector<fs::path> candidates = {
        cwd / "tests",
        cwd / "out" / "utility" / "tests", // Build output directory
        cwd / "utility" / "tests",
        cwd / "src" / "utility" / "tests",
        cwd / ".." / "tests",
        cwd / ".." / "utility" / "tests",
        cwd / ".." / "src" / "utility" / "tests",
        cwd / ".." / ".." / "src" / "utility" / "tests",
        exePath / "tests"}; // Relative to source file location

    fs::path testsDir;
    for (const auto &p : candidates)
    {
        if (fs::exists(p) && fs::is_directory(p))
        {
            testsDir = p;
            break;
        }
    }
    if (testsDir.empty())
        return files;

    try
    {
        for (const auto &entry : fs::directory_iterator(testsDir))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".in")
            {
                fs::path inFile = entry.path();
                fs::path outFile = testsDir / (entry.path().stem().string() + ".out");
                if (fs::exists(outFile))
                {
                    files.emplace_back(inFile.string(), outFile.string());
                }
            }
        }
    }
    catch (const fs::filesystem_error &ex)
    {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
    }

    std::sort(files.begin(), files.end());
    return files;
}

static std::string readFile(const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs)
    {
        return "";
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

TEST_CASE("Utility AI file-based tests")
{
    auto testFiles = findTestFiles();
    REQUIRE_MESSAGE(!testFiles.empty(), "No test files found in tests/ directory");

    for (const auto &[inPath, outPath] : testFiles)
    {
        SUBCASE(fs::path(inPath).stem().string().c_str())
        {
            std::string input = readFile(inPath);
            std::string expectedOutput = readFile(outPath);

            REQUIRE_MESSAGE(!input.empty(), ("Failed to read input file: " + inPath).c_str());
            REQUIRE_MESSAGE(!expectedOutput.empty(), ("Failed to read output file: " + outPath).c_str());

            std::string actualOutput = Utility::runUtilityAI(input);

            INFO("Input file: " << inPath);
            INFO("Expected:\n"
                 << expectedOutput);
            INFO("Actual:\n"
                 << actualOutput);

            CHECK(compareOutputs(actualOutput, expectedOutput));
        }
    }
}
