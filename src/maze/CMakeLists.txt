# Set module-specific output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/maze)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/maze)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/maze)

# Main executable
add_executable(maze maze.cpp ../lib/MemoryLeakDetector.cpp)
target_include_directories(maze PRIVATE ../lib)

# Test executable using doctest
add_executable(maze-tests tests.cpp ../lib/MemoryLeakDetector.cpp)
target_link_libraries(maze-tests PRIVATE doctest::doctest)
target_include_directories(maze-tests PRIVATE ../lib)

# Copy test files to build directory
file(GLOB TEST_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.in)
file(GLOB TEST_OUTPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.out)

# Copy test files to the module-specific tests directory
set(TEST_FILES_DESTINATION ${CMAKE_BINARY_DIR}/out/maze/tests)

# Create a custom target to handle test file copying
add_custom_target(copy-maze-test-files ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_FILES_DESTINATION}
    COMMENT "Creating test directory at ${TEST_FILES_DESTINATION}"
)

# Copy all test files in a batch
if(TEST_INPUT_FILES)
    add_custom_command(TARGET copy-maze-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_INPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying test input files"
    )
endif()

if(TEST_OUTPUT_FILES)
    add_custom_command(TARGET copy-maze-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_OUTPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying test output files"
    )
endif()

# Make sure maze-tests depends on the file copying
add_dependencies(maze-tests copy-maze-test-files)

# Enable testing
enable_testing()
add_test(NAME maze-tests COMMAND maze-tests)
