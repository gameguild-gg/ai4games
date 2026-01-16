# Set module-specific output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/fsm)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/fsm)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/fsm)

# Main executable
add_executable(fsm main.cpp ../lib/MemoryLeakDetector.cpp)
target_include_directories(fsm PRIVATE ../lib)

# Test executable using doctest
add_executable(fsm-tests tests.cpp ../lib/MemoryLeakDetector.cpp)
target_link_libraries(fsm-tests PRIVATE doctest::doctest)
target_include_directories(fsm-tests PRIVATE ../lib)

# Copy test files to build directory
file(GLOB TEST_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.in)
file(GLOB TEST_OUTPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.out)

# Copy test files to the module-specific tests directory
set(TEST_FILES_DESTINATION ${CMAKE_BINARY_DIR}/out/fsm/tests)

# Create a custom target to handle test file copying
add_custom_target(copy-fsm-test-files ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_FILES_DESTINATION}
        COMMENT "Creating test directory at ${TEST_FILES_DESTINATION}"
)

# Copy all test files in a batch
if(TEST_INPUT_FILES)
    add_custom_command(TARGET copy-fsm-test-files POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_INPUT_FILES} ${TEST_FILES_DESTINATION}/
            COMMENT "Copying test input files"
    )
endif()

if(TEST_OUTPUT_FILES)
    add_custom_command(TARGET copy-fsm-test-files POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_OUTPUT_FILES} ${TEST_FILES_DESTINATION}/
            COMMENT "Copying test output files"
    )
endif()

# Make sure fsm-tests depends on the file copying
add_dependencies(fsm-tests copy-fsm-test-files)

# Enable testing
enable_testing()
add_test(NAME fsm-tests COMMAND fsm-tests)
