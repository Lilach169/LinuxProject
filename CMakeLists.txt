cmake_minimum_required(VERSION 3.20)
project(LinuxProject C)

set(CMAKE_C_STANDARD 99)

# define the path to raylib
set(RAYLIB_PATH "C:/raylib")

include_directories(${RAYLIB_PATH}/include)
link_directories(${RAYLIB_PATH}/lib)

# add your main.c file
add_executable(LinuxProject main.c)

# link raylib and windows system libraries
target_link_libraries(LinuxProject raylib opengl32 gdi32 winmm)