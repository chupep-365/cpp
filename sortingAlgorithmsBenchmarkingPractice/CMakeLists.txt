cmake_minimum_required(VERSION 3.10)
project(SortingBenchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(benchmark src/main.cpp src/generator.cpp src/sorting_algorithms.cpp src/memory_tracker.cpp)