set(UTILS_DIR ${CMAKE_CURRENT_LIST_DIR})

find_package(Python3 COMPONENTS Interpreter)

# Copy a single file to a target build folder
function(add_file_copy target dest)
    set(files "${ARGN}")
    set(target_files "")

    foreach(file ${files})
        get_filename_component(filename "${file}" NAME)
        list(APPEND target_files "${dest}/${filename}")
    endforeach()

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

# Add a header to the reflection tool
function(add_reflected_headers target root_include headers)
    set(options ${ARGN})
    target_include_directories("${target}" PUBLIC "${UTILS_DIR}/../reflect/include")
    get_target_property(includes "${target}" INCLUDE_DIRECTORIES)
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    list(TRANSFORM includes PREPEND "-I")
    foreach(header ${headers})
        get_filename_component(filename "${header}" NAME_WE)
        add_custom_command(
                COMMAND ${Python3_EXECUTABLE} "${UTILS_DIR}/../reflect/reflect.py" ${header}
                -o "${CMAKE_CURRENT_SOURCE_DIR}/generated"
                -inc "${root_include}"
                $<IF:$<BOOL:${pch}>,-pch,> $<IF:$<BOOL:${pch}>,${pch},>
                -std=c++17 ${includes} ${options}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${header}
                OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/generated/${filename}.gen.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/generated/${filename}.gen.h"
                COMMENT "Running reflection code generation for '${header}'"
        )
        target_sources("${target}" PRIVATE "${UTILS_DIR}/../reflect/src/reflect.cpp" "generated/${filename}.gen.cpp")
    endforeach()
endfunction()