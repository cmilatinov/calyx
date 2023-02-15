# CMAKE Minimum Version
cmake_minimum_required(VERSION 3.20)

# Project name
project("Calyx")

# Subdirectories
add_subdirectory("utils")
add_subdirectory("build/reflect")
add_subdirectory("extern/icon-font-headers")
add_subdirectory("engine")
add_subdirectory("editor")
add_subdirectory("launcher")
add_subdirectory("sandbox")