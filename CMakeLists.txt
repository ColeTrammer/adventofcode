cmake_minimum_required(VERSION 3.25.2)

project(AdventOfCode CXX)

# Include the iros CMake build to get access to its libraries.
add_subdirectory(deps/iros)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp")
add_executable(aoc
    ${sources}
)
target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(aoc PRIVATE Di::di Dius::dius)
