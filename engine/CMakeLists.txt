# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-offsetof")
endif ()

# Includes
include(../build/include/reflect.cmake)

# Required libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")

# Clump source files from src
file(GLOB_RECURSE ENGINE_SRC "src/**.cpp")

# Create library from source
add_library("Engine" STATIC ${ENGINE_SRC})

# Link target
target_link_libraries("Engine" PUBLIC glfw glad::glad imgui::imgui imguizmo::imguizmo glm::glm assimp::assimp spdlog::spdlog_header_only EnTT::EnTT nameof::nameof)
target_include_directories("Engine" PUBLIC include ${STB_INCLUDE_DIRS})
target_precompile_headers("Engine" PUBLIC include/pch.h)
target_compile_definitions("Engine" PUBLIC "STB_IMAGE_IMPLEMENTATION")

# Add reflection
add_reflected_target("Engine")
add_reflected_headers("Engine" "include" "include/ecs/Component.h" "include/ecs/components/TransformComponent.h" "include/ecs/components/MeshComponent.h")
target_sources("Engine" PUBLIC "src/reflect/Global.cpp")

# Compile definitions based on platform
if (WIN32)
    get_target_property(LIB_TYPE "Engine" TYPE)
    if (LIB_TYPE STREQUAL SHARED_LIBRARY)
        target_compile_definitions("Engine" PUBLIC CX_BUILD_DLL)
        target_compile_definitions("Engine" PRIVATE CX_DLL_EXPORT)

        add_file_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:Engine>/Engine.dll")
        add_file_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:Engine>/Engine.pdb")
    endif ()
endif ()

# Copy assets to build folder
add_dir_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "assets")
add_dir_copy("Engine" "${PROJECT_BINARY_DIR}/editor" "assets")