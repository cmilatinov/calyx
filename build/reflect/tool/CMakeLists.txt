# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(Clang REQUIRED)
find_package(LLVM REQUIRED)
find_library(CLANG_LIB NAMES clang libclang REQUIRED)
find_library(CLANG_CPP_LIB NAMES clang-cpp libclang-cpp REQUIRED)
find_library(LLVM_LIB NAMES LLVM libLLVM REQUIRED)
find_package(inja REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json REQUIRED)

# Source
file(GLOB_RECURSE REFLECT_SRC "src/**.cpp")
add_executable("ReflectTool" ${REFLECT_SRC})

# Target definitions, includes, libraries
target_compile_definitions("ReflectTool" PRIVATE INJA_NOEXCEPTION ${LLVM_DEFINITIONS})
target_include_directories("ReflectTool" PRIVATE include ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
target_link_libraries("ReflectTool" PRIVATE
        Utils
        ReflectLib
        ${CLANG_LIB}
        ${CLANG_CPP_LIB}
        ${LLVM_LIB}
        cxxopts::cxxopts
        pantor::inja
        nlohmann_json::nlohmann_json
        )
target_precompile_headers("ReflectTool" PRIVATE include/pch.h)
