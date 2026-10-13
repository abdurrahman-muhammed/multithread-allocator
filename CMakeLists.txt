cmake_minimum_required(VERSION 3.20)

project(multithread_allocator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(allocator_test
	allocator/main.cpp
    allocator/Arena.cpp 
	allocator/benchmark.cpp
)

target_include_directories(allocator_test PRIVATE
    allocator
)