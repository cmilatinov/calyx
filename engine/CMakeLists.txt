# Includes
include(../cmake/include/utils.cmake)
include(../cmake/include/reflect.cmake)

# Required libraries
find_package(assimp REQUIRED)
find_package(cxxopts REQUIRED)
find_package(efsw REQUIRED)
find_package(EnTT REQUIRED)
find_package(glad REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(imguizmo REQUIRED)
find_package(implot REQUIRED)
find_package(nameof REQUIRED)
find_package(spdlog REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h" REQUIRED)

# Clump source files from src
file(GLOB_RECURSE ENGINE_SRC "src/**.cpp")

# Create library from source
add_library("CalyxEngine" SHARED ${ENGINE_SRC})

# Link shared library
target_link_libraries("CalyxEngine"
        PRIVATE
        -Wl,--whole-archive
        CalyxUtils
        CalyxReflectLib
        -Wl,--no-whole-archive
        PUBLIC
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
set(ENGINE_INCLUDE_DIRS
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/reflect/lib/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include/calyx>"
        ${STB_INCLUDE_DIRS})
target_include_directories("CalyxEngine" PUBLIC ${ENGINE_INCLUDE_DIRS})
target_precompile_headers("CalyxEngine" PRIVATE include/pch.h)
if(UNIX)
    set_target_properties("CalyxEngine" PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Add header file set
file(GLOB_RECURSE ENGINE_HEADERS "include/**.h")
list(FILTER ENGINE_HEADERS EXCLUDE REGEX ".*/pch.h$")
target_sources("CalyxEngine" PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${ENGINE_HEADERS})

# Add reflection
add_reflected_target("CalyxEngine")
add_reflected_headers("CalyxEngine" "include"
        "include/assets/Asset.h"
        "include/assets/Mesh.h"
        "include/assets/Assembly.h"
        "include/render/Camera.h"
        "include/render/objects/Shader.h"
        "include/render/objects/ShaderDefinition.h"
        "include/render/objects/Texture2D.h"
        "include/ecs/Component.h"
        "include/ecs/components/CameraComponent.h"
        "include/ecs/components/MeshComponent.h"
        "include/ecs/components/TransformComponent.h"
        "include/ecs/components/PointLightComponent.h"
        "include/math/Transform.h"
        "include/serialization/types/AssetSerializer.h"
        "include/serialization/types/StringSerializer.h")
target_sources("CalyxEngine" PRIVATE "src/reflect/Global.cpp")

# Compile definitions based on platform
if(WIN32)
    target_compile_definitions("CalyxEngine" PUBLIC CX_BUILD_DLL)
    target_compile_definitions("CalyxEngine" PRIVATE CX_DLL_EXPORT)
    
    add_file_copy("CalyxEngine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:CalyxEngine>/CalyxEngine.dll")
    add_file_copy("CalyxEngine" "${PROJECT_BINARY_DIR}/sandbox" "$<TARGET_FILE_DIR:CalyxEngine>/CalyxEngine.pdb")
endif()