cmake_minimum_required(VERSION 3.10)
project(os_2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fdiagnostics-color=always")

function(level name)
    add_executable(${name} ${name}.cpp)
endfunction()

level(1)
level(2)
level(3)
