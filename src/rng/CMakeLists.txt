# Set module-specific output directories (match maze setup)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/rng)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/rng)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/rng)

# Test executable using doctest
add_executable(rng-tests tests.cpp)
target_link_libraries(rng-tests PRIVATE doctest::doctest)

# Copy test files to build directory
file(GLOB TEST_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.in)
file(GLOB TEST_OUTPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.out)

# Copy test files to the module-specific tests directory
set(TEST_FILES_DESTINATION ${CMAKE_BINARY_DIR}/out/rng/tests)

# Create a custom target to handle test file copying
add_custom_target(copy-rng-test-files ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_FILES_DESTINATION}
    COMMENT "Creating RNG test directory at ${TEST_FILES_DESTINATION}"
)

# Copy all test files in a batch
if(TEST_INPUT_FILES)
    add_custom_command(TARGET copy-rng-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_INPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying RNG test input files"
    )
endif()

if(TEST_OUTPUT_FILES)
    add_custom_command(TARGET copy-rng-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_OUTPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying RNG test output files"
    )
endif()

# Make sure rng-tests depends on the file copying
add_dependencies(rng-tests copy-rng-test-files)

# Enable testing
enable_testing()
add_test(NAME rng-tests COMMAND rng-tests)

