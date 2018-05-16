# cmake_minimum_required(VERSION <specify CMake version here>)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(DES C)

set(CMAKE_C_STANDARD 99)

add_executable(DES main.c des.h des.c)