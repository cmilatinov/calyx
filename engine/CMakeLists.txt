# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")

# Includes
include(../build/include/reflect.cmake)

# Required libraries
find_package(assimp CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(efsw CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nameof CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")

# Clump source files from src
file(GLOB_RECURSE ENGINE_SRC "src/**.cpp")

# Create library from source
add_library("Engine_STATIC" STATIC ${ENGINE_SRC})
add_library("Engine" SHARED /dev/null)

# Link static target
target_link_libraries("Engine_STATIC" PUBLIC
        ReflectLib
        Utils
        IconFonts
        assimp::assimp
        cxxopts::cxxopts
        efsw::efsw
        EnTT::EnTT
        glad::glad
        glfw
        glm::glm
        imgui::imgui
        imguizmo::imguizmo
        implot::implot
        nameof::nameof
        spdlog::spdlog_header_only)
target_include_directories("Engine_STATIC" PUBLIC include ${STB_INCLUDE_DIRS})
target_precompile_headers("Engine_STATIC" PRIVATE include/pch.h)
if(UNIX)
    set_target_properties("Engine_STATIC" PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Linked shared target
target_link_libraries("Engine" PUBLIC Engine_STATIC)
target_precompile_headers("Engine" PRIVATE include/pch.h)

# Add reflection
add_reflected_target("Engine")
add_reflected_headers("Engine" "include"
        "include/assets/Asset.h"
        "include/assets/Mesh.h"
        "include/render/objects/Shader.h"
        "include/render/objects/Texture2D.h"
        "include/ecs/Component.h"
        "include/ecs/components/CameraComponent.h"
        "include/ecs/components/MeshComponent.h"
        "include/ecs/components/TransformComponent.h"
        "include/math/Transform.h"
        "include/serialization/DefaultSerializers.h"
        "include/serialization/types/AssetSerializer.h")
target_sources("Engine" PRIVATE "src/reflect/Global.cpp")

# Compile definitions based on platform
if(WIN32)
    target_compile_definitions("Engine" PUBLIC CX_BUILD_DLL)
    target_compile_definitions("Engine" PRIVATE CX_DLL_EXPORT)
    
    add_file_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:Engine>/Engine.dll")
    add_file_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:Engine>/Engine.pdb")
endif()

# Copy assets to build folder
add_dir_copy("Engine" "${PROJECT_BINARY_DIR}/sandbox" "assets")
add_dir_copy("Engine" "${PROJECT_BINARY_DIR}/editor" "assets")
add_dir_copy("Engine" "${PROJECT_BINARY_DIR}/launcher" "assets")