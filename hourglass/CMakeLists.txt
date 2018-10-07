cmake_minimum_required(VERSION 3.12)
project(hourglass C)

set(CMAKE_C_STANDARD 11)

include_directories(../)
add_executable(hourglass hourglass.c hourglass.h main.c)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(hourglass PUBLIC USE_FULL_ASSERT=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(hourglass PUBLIC USE_FULL_ASSERT=0)
endif ()

TARGET_LINK_LIBRARIES( hourglass pthread rt)
