# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# Compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif ()

# Find packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)

# Generator
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBRARIES support option)

# Source
file(GLOB_RECURSE REFLECT_SRC "src/**.cpp")
add_executable("ReflectTool" ${REFLECT_SRC})

# Target definitions, includes, libraries
target_compile_definitions("ReflectTool" PRIVATE ${LLVM_DEFINITIONS_LIST} INJA_NOEXCEPTION)
target_include_directories("ReflectTool" PRIVATE include ${LLVM_INCLUDE_DIRS})
target_link_libraries("ReflectTool" PRIVATE
        ReflectLib
        libclang clangAST clangASTMatchers clangBasic clangFrontend clangRewrite clangTooling
        ${LLVM_LIBRARIES}
        nlohmann_json::nlohmann_json
        pantor::inja
        )
target_precompile_headers("ReflectTool" PRIVATE include/pch.h)
