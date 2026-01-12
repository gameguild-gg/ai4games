// File-based tests for RNG::xorShift using doctest and the tests/ fixtures
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "random.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Normalize line endings to \n
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

// Remove trailing spaces per line and trailing empty lines
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

// Run RNG generation according to README: chain seed with raw xorshift result,
// then clamp to [min,max] inclusive and output one number per line.
static std::string runRngGeneration(const std::string &input)
{
  std::istringstream in(input);
  unsigned int seed; // 32-bit unsigned seed
  unsigned int N;    // number of values
  int r1, r2;        // range endpoints (inclusive), may be unordered
  in >> seed >> N >> r1 >> r2;

  int minv = std::min(r1, r2);
  int maxv = std::max(r1, r2);
  unsigned int span = static_cast<unsigned int>(maxv - minv + 1);

  std::ostringstream out;
  for (unsigned int i = 0; i < N; ++i)
  {
    // Students' function returns the next raw 32-bit value; r1/r2 are
    // passed for signature completeness (may be ignored by implementation).
    seed = RNG::xorShift(seed, minv, maxv);
    int clamped = minv + static_cast<int>(seed % span);
    out << clamped;
    if (i + 1 < N)
      out << '\n';
  }
  return out.str();
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
  std::vector<fs::path> candidates = {
      cwd / "tests",
      cwd / "rng" / "tests",
      cwd / "src" / "rng" / "tests",
      cwd / ".." / "rng" / "tests",
      cwd / ".." / "src" / "rng" / "tests",
      cwd / ".." / ".." / "src" / "rng" / "tests"};

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

static void runFixtureTest(const std::string &name, const std::string &inPath, const std::string &outPath)
{
  std::ifstream inFile(inPath);
  std::ifstream outFile(outPath);
  REQUIRE(inFile.is_open());
  REQUIRE(outFile.is_open());

  std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
  std::string expected((std::istreambuf_iterator<char>(outFile)), std::istreambuf_iterator<char>());

  input = normalizeLineEndings(input);
  expected = normalizeLineEndings(expected);

  std::string actual = runRngGeneration(input);

  INFO("Fixture: " << name);
  INFO("Input: " << input);
  INFO("Expected:\n"
       << expected);
  INFO("Actual:\n"
       << actual);
  CHECK(compareOutputs(actual, expected));
}

TEST_CASE("RNG file-based fixtures")
{
  auto fixtures = findTestFiles();
  if (fixtures.empty())
  {
    WARN("No RNG fixtures found. Check working directory.");
    return;
  }
  for (const auto &f : fixtures)
  {
    fs::path inPath(f.first);
    std::string name = inPath.stem().string();
    SUBCASE(("Fixture: " + name).c_str())
    {
      runFixtureTest(name, f.first, f.second);
    }
  }
}
