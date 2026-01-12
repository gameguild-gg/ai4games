# Set module-specific output directories (match maze/fsm setup)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bt)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bt)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out/bt)

# Local run executable
add_executable(bt main.cpp)

# Test executable using doctest
add_executable(bt-tests tests.cpp)
target_link_libraries(bt-tests PRIVATE doctest::doctest)

# Copy test files to build directory
file(GLOB TEST_INPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.in)
file(GLOB TEST_OUTPUT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.out)

# Copy test files to the module-specific tests directory
set(TEST_FILES_DESTINATION ${CMAKE_BINARY_DIR}/out/bt/tests)

add_custom_target(copy-bt-test-files ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TEST_FILES_DESTINATION}
    COMMENT "Creating bt test directory at ${TEST_FILES_DESTINATION}"
)

if(TEST_INPUT_FILES)
    add_custom_command(TARGET copy-bt-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_INPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying bt test input files"
    )
endif()

if(TEST_OUTPUT_FILES)
    add_custom_command(TARGET copy-bt-test-files POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TEST_OUTPUT_FILES} ${TEST_FILES_DESTINATION}/
        COMMENT "Copying bt test output files"
    )
endif()

add_dependencies(bt-tests copy-bt-test-files)

# Enable testing
enable_testing()
add_test(NAME bt-tests COMMAND bt-tests)
