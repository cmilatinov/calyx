# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")

# Required libraries
find_package(glew REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

# Clump source files from src
file(GLOB_RECURSE ENGINE_SRC "src/**.cpp")

# Create library from source
add_library("Engine" ${ENGINE_SRC})

# Link target
target_link_libraries("Engine" PRIVATE GLEW::GLEW glfw imgui::imgui glm assimp::assimp spdlog::spdlog spdlog::spdlog_header_only)
target_include_directories("Engine" PRIVATE include ${STB_INCLUDE_DIRS})

# Preprocessor definitions
if (WIN32)
    target_compile_definitions("Engine" PRIVATE CX_PLATFORM_WINDOWS CX_BUILD_DLL)
endif()