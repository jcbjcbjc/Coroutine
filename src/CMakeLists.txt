cmake_minimum_required(VERSION 3.22)
project(Coroutine-jcb)

set(CMAKE_CXX_STANDARD 23)

add_executable(test main.cpp )

target_link_libraries(test Coroutine_base)

add_subdirectory(base)
add_subdirectory(Coroutine)
add_subdirectory(examples/RPC)

include_directories(${PROJECT_SOURCE_DIR})

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)