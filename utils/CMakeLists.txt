# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Required packages
if(UNIX)
    find_library(UUID_LIB NAMES uuid libuuid)
endif()
find_package(fmt CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)

# Clump header files from include
file(GLOB_RECURSE UTILS_SRC "src/**.cpp")

# Create library with interface include directory
add_library("Utils" ${UTILS_SRC})
if(UNIX)
    target_link_libraries("Utils" PUBLIC "${UUID_LIB}")
endif()
target_link_libraries("Utils" PUBLIC fmt::fmt-header-only stduuid)
target_include_directories("Utils" PUBLIC include)
target_compile_definitions("Utils" PUBLIC "UUID_SYSTEM_GENERATOR")
