cmake_minimum_required(VERSION 3.15)
project(POSIX-threads)

set(CMAKE_C_STANDARD 90)

add_executable(1 1.c)
add_executable(2 2.c)
add_executable(3_multi 3_multi.c)
add_executable(3 3.c)
add_executable(4 4.c 4.c)

target_link_options(1 PRIVATE -pthread)
target_link_options(2 PRIVATE -pthread)
target_link_options(3_multi PRIVATE -pthread)
target_link_options(4 PRIVATE -pthread)
