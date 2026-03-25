cmake_minimum_required(VERSION 3.16)

project(grets CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include headers
include_directories(include)

# Executable
add_executable(grets grets.cc)


# Compiler warnings (recommended)
target_compile_options(grets PRIVATE
    -Wall -Wextra -Wpedantic
)

