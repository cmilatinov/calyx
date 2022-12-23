# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(EnTT CONFIG REQUIRED)

# Library
add_library("ReflectLib" "Reflect.cpp")
target_include_directories("ReflectLib" PUBLIC "./")
target_link_libraries("ReflectLib" PUBLIC EnTT::EnTT)