# CMAKE Minimum Version
cmake_minimum_required(VERSION 3.17)

# Print build commands
set(CMAKE_VERBOSE_MAKEFILE ON)

# Project name
project("Calyx")

# Subdirectories
add_subdirectory("engine")
add_subdirectory("sandbox")
add_subdirectory("editor")