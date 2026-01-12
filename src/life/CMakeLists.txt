# Set module-specific output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/life)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/life)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/life)

# Main executable
add_executable(life main.cpp ../lib/MemoryLeakDetector.cpp)
target_include_directories(life PRIVATE ../lib)

# Test executable using doctest
add_executable(life-tests tests.cpp ../lib/MemoryLeakDetector.cpp)
target_link_libraries(life-tests PRIVATE doctest::doctest)
target_include_directories(life-tests PRIVATE ../lib)

# Copy test files to build directory
file(GLOB TEST_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.in)
file(GLOB TEST_OUTPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.out)

# Copy test files to the module-specific tests directory
set(TEST_FILES_DESTINATION ${CMAKE_BINARY_DIR}/out/life/tests)

# Create a custom target to handle test file copying
add_custom_target(copy-life-test-files ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_FILES_DESTINATION}
    COMMENT "Creating test directory at ${TEST_FILES_DESTINATION}"
)

# Copy all test files in a batch
if(TEST_INPUT_FILES)
    add_custom_command(TARGET copy-life-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_INPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying test input files"
    )
endif()

if(TEST_OUTPUT_FILES)
    add_custom_command(TARGET copy-life-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_OUTPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying test output files"
    )
endif()

# Make sure life-tests depends on the file copying
add_dependencies(life-tests copy-life-test-files)

# Enable testing
enable_testing()
add_test(NAME life-tests COMMAND life-tests)
