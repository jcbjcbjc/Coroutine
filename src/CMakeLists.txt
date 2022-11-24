cmake_minimum_required(VERSION 3.22)
project(Coroutine)

set(CMAKE_CXX_STANDARD 23)

add_executable(Coroutine main.cpp Scheduler/scheduler.h Coroutine/coroutine.h Task/task.h Task/task.cpp examples/RPC/RpcClient/RpcClient.h)
target_link_libraries(Coroutine common_base)
add_subdirectory(base)