cmake_minimum_required(VERSION 3.20)
project(LinuxProject C)

set(CMAKE_C_STANDARD 99)

add_executable(LinuxProject main.c)

target_link_libraries(LinuxProject raylib)