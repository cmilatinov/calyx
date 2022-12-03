# Minimum Version
cmake_minimum_required(VERSION 3.20)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-offsetof")
endif ()

# Includes
include(../build/include/utils.cmake)

# Required libraries
find_package(efsw CONFIG REQUIRED)

# Clump source files from src
file(GLOB_RECURSE EDITOR_SRC "src/**.cpp")

# Create executable from source
add_executable("Editor" ${EDITOR_SRC})

# Link target
target_link_libraries("Editor" PUBLIC "Engine" efsw::efsw)
target_include_directories("Editor" PUBLIC "include")
target_precompile_headers("Editor" PUBLIC "../engine/include/Calyx.h")

# Add reflection
add_reflected_target("Editor")
add_reflected_headers("Editor" "include"
        "include/inspector/TypeInspector.h"
        "include/inspector/types/StringInspector.h"
        "include/inspector/types/Vec2Inspector.h"
        "include/inspector/types/Vec3Inspector.h"
        "include/inspector/components/TransformComponentInspector.h")

# Copy assets
add_dir_copy("Editor" "$<TARGET_FILE_DIR:Editor>" "assets")