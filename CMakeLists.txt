cmake_minimum_required(VERSION 3.25.2)

project(AdventOfCode CXX)

# Include the iros CMake build to get access to its libraries.
add_subdirectory(iros)

add_executable(aoc
    2022/01/solution.cpp
    2022/02/solution.cpp
    2022/03/solution.cpp
    2022/20/solution.cpp
    runner/main.cpp
)
target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(aoc PRIVATE Di::di Dius::dius)
