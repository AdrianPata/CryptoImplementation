cmake_minimum_required(VERSION 3.10)
project(SHA C)

set(CMAKE_C_STANDARD 99)

add_executable(SHA main.c sha256.h sha256.c)