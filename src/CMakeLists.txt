cmake_minimum_required(VERSION 3.22.1)
project(coroutine-c++ C CXX)

set(CMAKE_CXX_STANDARD 23)


add_executable(coroutine-c++ main.cpp Scheduler/scheduler.h Coroutine/coroutine.h)
add_subdirectory(base)