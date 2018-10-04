cmake_minimum_required(VERSION 3.12)
project(fifo C)

set(CMAKE_C_STANDARD 11)

include_directories(../)
add_executable(fifo fifo.h fifo.c main.c)
