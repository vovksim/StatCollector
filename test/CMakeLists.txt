# Enable testing
enable_testing()

# Add test executable
add_executable(StatCollectorTest ~/CLionProjects/StatCollector/test/tests.cpp)

# Link the test executable with Google Test and the main project
target_link_libraries(StatCollectorTest)

