cmake_minimum_required(VERSION 3.15)
project(Labo3 C CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD 17)

add_executable(1 src/1.c)

add_executable(2 src/2.c)

add_executable(writestring src/writestring.c)

add_executable(3.1 src/3.1.c)
add_dependencies(3.1 writestring)

add_executable(3.2 src/3.2.c)
add_dependencies(3.2 writestring)

add_executable(watchfile src/watchfile.c)

#exercise 4
add_executable(watchfiled src/watchfiled.cpp)
add_dependencies(watchfiled watchfile)

#exercise 5
add_executable(watchfiled_5 src/watchfiled_5.cpp)
add_dependencies(watchfiled_5 watchfile)

add_custom_command(
        TARGET watchfiled watchfiled_5
        COMMAND ${CMAKE_COMMAND} -E make_directory testfiles
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/data/testfiles/*.txt
        ${CMAKE_CURRENT_BINARY_DIR}/testfiles/
)
add_custom_command(
        TARGET watchfiled watchfiled_5
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/data/watchfile.txt
        ${CMAKE_CURRENT_BINARY_DIR}/watchfile.txt
)