# CMAKE Minimum Version
cmake_minimum_required(VERSION 3.20)

# Project name
project("Calyx")

# Subdirectories
add_subdirectory("build/reflect")
add_subdirectory("engine")
add_subdirectory("sandbox")
add_subdirectory("editor")