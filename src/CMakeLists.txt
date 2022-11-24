cmake_minimum_required(VERSION 3.22)
project(Coroutine)

set(CMAKE_CXX_STANDARD 23)

<<<<<<< Updated upstream
add_executable(Coroutine main.cpp Scheduler/scheduler.h Coroutine/coroutine.h Task/task.h)
=======
add_executable(Coroutine main.cpp Scheduler/scheduler.h Coroutine/coroutine.h Task/task.h Task/task.cpp)
>>>>>>> Stashed changes
target_link_libraries(Coroutine common_base)
add_subdirectory(base)

add_subdirectory(examples/RPC)

include_directories(${PROJECT_SOURCE_DIR})

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)