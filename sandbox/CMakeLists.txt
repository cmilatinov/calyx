# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")

# Includes
include(../build/include/utils.cmake)

# Required libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

# Clump source files from src
file(GLOB_RECURSE SANDBOX_SRC "src/**.cpp")

# Create executable from source
add_executable("Sandbox" ${SANDBOX_SRC})

# Link target
target_link_libraries("Sandbox" PUBLIC "Engine")
target_include_directories("Sandbox" PUBLIC "include")
target_precompile_headers("Sandbox" PUBLIC "../engine/include/Calyx.h")

# Copy assets
add_dir_copy("Sandbox" "$<TARGET_FILE_DIR:Sandbox>" "assets")