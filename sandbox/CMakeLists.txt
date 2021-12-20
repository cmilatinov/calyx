# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
file(GLOB_RECURSE SANDBOX_SRC "src/**.cpp")

# Create library from source
add_executable("Sandbox" ${SANDBOX_SRC})

# Link target
target_link_libraries("Sandbox" PRIVATE "Engine" GLEW::GLEW glfw imgui::imgui glm assimp::assimp spdlog::spdlog spdlog::spdlog_header_only)
target_include_directories("Sandbox" PRIVATE "include" "../engine/include")

function(add_file_copy target step)
    set(files "${ARGN}")
    set(target_files "")

    foreach(file ${files})
        get_filename_component(filename "${file}" NAME)
        list(APPEND target_files "$<TARGET_FILE_DIR:${target}>/${filename}")
    endforeach()

    # Delete old files
    add_custom_command(
            TARGET "${target}" "${step}"
            COMMAND ${CMAKE_COMMAND} -E rm -f
            ${target_files}
    )

    # Copy new files
    add_custom_command(
            TARGET "${target}" "${step}"
            COMMAND ${CMAKE_COMMAND} -E copy
            ${files}
            "$<TARGET_FILE_DIR:${target}>"
    )
endfunction()

function(add_dir_copy target step dir)
    # Delete old directory
    add_custom_command(
            TARGET "${target}" "${step}"
            COMMAND ${CMAKE_COMMAND} -E rm -rf
            "$<TARGET_FILE_DIR:${target}>/${dir}"
    )

    # Copy new directory
    add_custom_command(
            TARGET "${target}" "${step}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}"
            "$<TARGET_FILE_DIR:${target}>/${dir}"
    )
endfunction()

# Copy assets
add_dir_copy("Sandbox" POST_BUILD "assets")

# Copy DLL file to the same folder as the executable on Windows
if (WIN32)
    add_file_copy("Sandbox" POST_BUILD
            "$<TARGET_FILE_DIR:Engine>/fmtd.dll"
            "$<TARGET_FILE_DIR:Engine>/glfw3.dll"
            "$<TARGET_FILE_DIR:Engine>/spdlogd.dll"
            "$<TARGET_FILE_DIR:Engine>/Engine.dll"
            "$<TARGET_FILE_DIR:Engine>/Engine.pdb")
endif()