#pragma once

static const char* TEMPLATE_ASSEMBLIES_CMAKE =
    R"(
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project asset directory
set(ASSET_DIR "${PROJECT_SOURCE_DIR}/{{ asset_directory }}")

# Dependencies
find_package(Calyx REQUIRED)

## for assembly in assemblies
# Assembly '{{ assembly.name }}'
set(ASSEMBLY_{{ assembly.name }}_SRC
## for source_file in assembly.source_files
    "{{ source_file }}"
## endfor
)
add_library(Assembly{{ assembly.name }} SHARED ${ASSEMBLY_{{ assembly.name }}_SRC})
target_include_directories(Assembly{{ assembly.name }} PUBLIC "${ASSET_DIR}")
## endfor
)";