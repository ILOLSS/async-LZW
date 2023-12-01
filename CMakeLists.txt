cmake_minimum_required(VERSION 3.0.0)
project(multithread_lzw VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fsanitize=address")

set(PROJECT_SOURCES 
    src/main.cpp 
    async_encode/src/async_lzw.cpp
    async_encode/src/async_lzw_task.cpp
    async_engine/src/async_tasks_pool.cpp
    async_engine/src/thread_guard.cpp
    universal/src/dictionary.cpp
    universal/src/file.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

include_directories("async_encode/include/" "async_engine/include/" "sync_lzw/include/" "universal/include/")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
