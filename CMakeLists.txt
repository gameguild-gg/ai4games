# set the minimum cmake version to generate projects.
cmake_minimum_required(VERSION 3.20.6)

project(
        AwesomeCppCourse
        LANGUAGES CXX
        VERSION 0.0.1 # todo: get this version from git tag
)

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info
include(cmake/get_cpm.cmake)

# Set C++23 globally
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include external libs
message("Git clonning doctest...")
CPMAddPackage("gh:doctest/doctest@2.4.12")

message("Git clonning Format...")
CPMAddPackage("gh:TheLartians/Format.cmake@1.8.3")

# Enable testing
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)