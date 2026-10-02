cmake_minimum_required(VERSION 3.20)

project(ThreadPool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ThreadPool
    src/main.cpp
    src/tp.cpp
)

target_include_directories(ThreadPool PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)