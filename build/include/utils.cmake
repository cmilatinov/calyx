set(UTILS_DIR ${CMAKE_CURRENT_LIST_DIR})

# Find clang
find_program(CLANG_EXECUTABLE clang)

# Find libclang
find_file(LIBCLANG_PATH NAMES libclang.dll libclang.so)
if (NOT EXISTS "${LIBCLANG_PATH}")
    message(FATAL_ERROR "libclang not found")
endif ()

# Copy a single file to a target build folder
function(add_file_copy target dest)
    set(files "${ARGN}")
    set(target_files "")

    foreach (file ${files})
        get_filename_component(filename "${file}" NAME)
        list(APPEND target_files "${dest}/${filename}")
    endforeach ()

    # Copy new files
    add_custom_command(
            TARGET "${target}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${files}
            "${dest}"
    )
endfunction()

# Copy an entire directory to a target build folder
function(add_dir_copy target dest dir)
    add_custom_command(
            TARGET "${target}" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}"
            "${dest}/${dir}"
    )
endfunction()

function(add_reflected_target target)
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    foreach (header ${pch})
        get_filename_component(filename "${header}" NAME_WE)
        add_custom_command(
                COMMAND "${CLANG_EXECUTABLE}" -xc++-header
                -Xclang -std=c++${CMAKE_CXX_STANDARD} "${header}"
                -o "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch"
                COMMENT "Generating precompiled header '${header}' using clang"
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch"
        )
    endforeach ()
    target_sources("${target}" PRIVATE "${UTILS_DIR}/../reflect/src/reflect.cpp")
endfunction()

# Add a header to the reflection tool
function(add_reflected_headers target root_include)
    set(headers ${ARGN})
    target_include_directories("${target}" PUBLIC "${UTILS_DIR}/../reflect/include")
    get_target_property(includes "${target}" INCLUDE_DIRECTORIES)

    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    set(pch_compiled "")
    set(pch_flags "")
    foreach (header ${pch})
        get_filename_component(filename "${header}" NAME_WE)
        list(APPEND pch_compiled "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
        list(APPEND pch_flags "-include-pch")
        list(APPEND pch_flags "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
    endforeach ()

    list(TRANSFORM includes PREPEND "-I")
    foreach (header ${headers})
        get_filename_component(filename "${header}" NAME_WE)
        add_custom_command(
                COMMAND "Reflect" ${header}
                -output-dir "${CMAKE_CURRENT_SOURCE_DIR}/generated"
                -include-root "${root_include}"
                -- -xc++ -std=c++${CMAKE_CXX_STANDARD} -Wno-pragma-once-outside-header
                ${includes} ${pch_flags}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${header} ${pch_compiled}
                OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/generated/${filename}.gen.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/${filename}.gen.h"
                COMMENT "Running reflection code generation for '${header}'"
        )
        target_sources("${target}" PRIVATE "generated/${filename}.gen.cpp")
    endforeach ()
endfunction()