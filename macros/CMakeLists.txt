# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Clump header files from include
file(GLOB_RECURSE MACROS_SRC "include/**.h")

# Create library with interface include directory
add_library("Macros" INTERFACE ${MACROS_SRC})
target_include_directories("Macros" INTERFACE "include")