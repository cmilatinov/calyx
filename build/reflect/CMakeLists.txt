# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Includes
include(../include/utils.cmake)

# Find python
find_package(Python3 COMPONENTS Interpreter)

# Set source files
set(REFLECT_SRC "src/main.cpp")

# Create executable from sources
add_executable("Reflect" ${REFLECT_SRC})

# Set target includes
target_include_directories("Reflect" PUBLIC include generated)

# Add reflected header
add_reflected_headers("Reflect" "include" "include/test.h;include/subfolder/test2.h")
