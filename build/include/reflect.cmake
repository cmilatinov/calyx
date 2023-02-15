include("${PROJECT_SOURCE_DIR}/build/include/utils.cmake")

# Find clang
find_program(CLANG_EXECUTABLE clang)

# Find libclang
find_package(Clang REQUIRED)

# Add a reflection target to the reflection build system
function(add_reflected_target target)
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    get_target_include_directories(includes "${target}")
    list(APPEND includes "${PROJECT_SOURCE_DIR}/build/reflect/lib/include")
    list(TRANSFORM includes PREPEND "-I")
    if(DEFINED pch)
        foreach(header ${pch})
            get_filename_component(filename "${header}" NAME_WE)
            add_custom_command(
                    COMMAND "${CLANG_EXECUTABLE}" -xc++-header
                    -Xclang -std=c++${CMAKE_CXX_STANDARD} "${header}" ${includes}
                    -o "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch" -Wno-everything
                    COMMENT "Generating precompiled header '${header}' using clang"
                    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch"
            )
        endforeach()
    endif()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options("${target}" PRIVATE "-Wno-invalid-offsetof")
    endif()
endfunction()

# Add a header to the reflection tool
function(add_reflected_headers target root_include)
    # Collect header list
    set(headers ${ARGN})
    
    # Add reflect lib dependency
    target_link_libraries("${target}" PUBLIC "ReflectLib")
    
    # Create list of pch flags
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    set(pch_compiled "")
    set(pch_flags "")
    foreach(header ${pch})
        get_filename_component(filename "${header}" NAME_WE)
        list(APPEND pch_compiled "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
        list(APPEND pch_flags "-include-pch")
        list(APPEND pch_flags "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
    endforeach()
    string(REPLACE ";" "," pch_flags "${pch_flags}")
    
    # Get include directories and reflect headers
    get_target_include_directories(includes "${target}")
    list(TRANSFORM includes PREPEND "-I")
    string(REPLACE ";" "," includes "${includes}")
    foreach(header ${headers})
        get_filename_component(abs_root_include "${root_include}" ABSOLUTE)
        get_filename_component(abs_header "${header}" ABSOLUTE)
        file(RELATIVE_PATH rel_path "${abs_root_include}" "${abs_header}")
        string(REGEX REPLACE "\\.[^.]*$" "" rel_path "${rel_path}")
        add_custom_command(
                COMMAND "ReflectTool"
                ${header}
                -o "${CMAKE_CURRENT_SOURCE_DIR}/generated"
                -r "${root_include}"
                --args "-xc++,-std=c++${CMAKE_CXX_STANDARD},-Wno-everything,${includes},${pch_flags}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${header} ${pch_compiled}
                OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/generated/${rel_path}.gen.cpp"
                COMMENT "Running reflection code generation for '${header}'"
        )
        target_sources("${target}" PRIVATE "generated/${rel_path}.gen.cpp")
        set_source_files_properties("generated/${rel_path}.gen.cpp" PROPERTIES GENERATED 1)
    endforeach()
endfunction()