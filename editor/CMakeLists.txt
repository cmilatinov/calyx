# Includes
include(../cmake/include/utils.cmake)
include(../cmake/include/reflect.cmake)

# Clump source files from src
file(GLOB_RECURSE EDITOR_SRC "src/**.cpp")

# Create executable from source
add_executable("CalyxEditor" ${EDITOR_SRC})

# Link target
target_link_libraries("CalyxEditor" PUBLIC CalyxEngine CalyxProject)
target_include_directories("CalyxEditor" PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include/calyx>")
target_precompile_headers("CalyxEditor" PRIVATE "include/pch.h")

# Add reflection
add_reflected_target("CalyxEditor")
add_reflected_headers("CalyxEditor" "include"
        "include/inspector/TypeInspector.h"
        "include/inspector/types/StringInspector.h"
        "include/inspector/types/FloatInspector.h"
        "include/inspector/types/Vec2Inspector.h"
        "include/inspector/types/Vec3Inspector.h"
        "include/inspector/types/Vec4Inspector.h"
        "include/inspector/types/AssetInspector.h"
        "include/inspector/components/CameraComponentInspector.h"
        "include/inspector/components/TransformComponentInspector.h")

# Copy assets
add_dir_copy("CalyxEditor" "${PROJECT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:CalyxEditor>/assets")