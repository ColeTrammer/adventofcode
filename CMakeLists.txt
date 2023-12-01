cmake_minimum_required(VERSION 3.25.2)

project(AdventOfCode CXX)

# Include the iros CMake build to get access to its libraries.
add_subdirectory(iros)

function(watch)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ARGV})
endfunction()

file(STRINGS runner/problem_list.txt PROBLEM_LIST)
watch(runner/problem_list.txt)

set(SOLUTION_SOURCES "")
foreach(PROBLEM ${PROBLEM_LIST})
    list(APPEND SOLUTION_SOURCES "${PROBLEM}/solution.cpp")
endforeach()

add_executable(aoc
    ${SOLUTION_SOURCES}
    runner/main.cpp
)
target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(aoc PRIVATE Di::di Dius::dius)
