cmake_minimum_required(VERSION 2.8)
project(rez-compiled-packages CXX)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

# if GCC
add_definitions(-Wall -Wextra -Wpedantic)

# project properties
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add dependencies
add_subdirectory(fmt)
add_subdirectory(string-view-lite)

set(PYBIND11_PYTHON_VERSION 2.7 CACHE STRING "")
add_subdirectory(pybind11)

# add compiled packages
add_subdirectory(version)

# if MSVC
set(gtest_force_shared_crt ON)

add_subdirectory(googletest)
add_subdirectory(test)

# if bench
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)
add_subdirectory(bench)
