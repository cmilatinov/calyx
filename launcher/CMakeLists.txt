# Required libraries
find_package(Boost REQUIRED)

# Clump source files from src
file(GLOB_RECURSE LAUNCHER_SRC "src/**.cpp")

# Create executable from source
add_executable("CalyxLauncher" ${LAUNCHER_SRC})

# Link target
target_link_libraries("CalyxLauncher" PUBLIC CalyxEngine CalyxProject Boost::boost)
target_include_directories("CalyxLauncher" PUBLIC include)
target_precompile_headers("CalyxLauncher" PRIVATE include/pch.h)
target_compile_definitions("CalyxLauncher" PRIVATE "CX_EDITOR_PATH=\"./CalyxEditor\"")

# Create symlink to editor
add_dependencies("CalyxLauncher" CalyxEditor)
add_custom_command(
        TARGET "CalyxLauncher" PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "$<TARGET_FILE:CalyxEditor>"
        "${CMAKE_CURRENT_BINARY_DIR}/CalyxEditor"
        BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/CalyxEditor")

# Copy assets
add_dir_copy("CalyxLauncher" "${PROJECT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:CalyxLauncher>/assets")