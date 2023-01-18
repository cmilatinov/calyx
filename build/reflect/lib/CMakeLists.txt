# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(EnTT CONFIG REQUIRED)

# Library
file(GLOB_RECURSE REFLECT_LIB_SRC "src/**.cpp")
add_library("ReflectLib" ${REFLECT_LIB_SRC})
target_include_directories("ReflectLib" PUBLIC include)
target_link_libraries("ReflectLib" PUBLIC EnTT::EnTT)