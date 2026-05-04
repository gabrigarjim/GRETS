cmake_minimum_required(VERSION 3.16)

project(grets CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release)

# Include headers
include_directories(include)

# Executable
add_executable(grets grets.cc)


# Compiler warnings (recommended)
target_compile_options(grets PRIVATE
    -Wall -Wextra -Wpedantic -O3 -march=native -mtune=native
)

target_link_options(grets PRIVATE -flto)
target_compile_options(grets PRIVATE -flto)

