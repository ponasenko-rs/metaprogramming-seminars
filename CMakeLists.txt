cmake_minimum_required(VERSION 3.15)
project(seminars)

set(CMAKE_CXX_STANDARD 20)

add_executable(seminars main.cpp)

add_custom_target(
        run
        COMMAND seminars
        DEPENDS seminars
        )
