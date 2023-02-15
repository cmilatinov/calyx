# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required libraries
find_package(Boost REQUIRED)

# Clump source files from src
file(GLOB_RECURSE LAUNCHER_SRC "src/**.cpp")

# Create executable from source
add_executable("Launcher" ${LAUNCHER_SRC})

# Link target
target_link_libraries("Launcher" PUBLIC Engine_STATIC Boost::boost)
target_include_directories("Launcher" PUBLIC "include")
target_precompile_headers("Launcher" PRIVATE "include/pch.h")
target_compile_definitions("Launcher" PRIVATE "CX_EDITOR_PATH=\"$<TARGET_FILE:Editor>\"")
add_dependencies("Launcher" Editor)