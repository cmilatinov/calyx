# Find clang
find_package(Clang REQUIRED)
get_property(CLANG_EXECUTABLE TARGET clang PROPERTY LOCATION)

# Retrieve library interface directories recursively for the given target
function(_get_library_include_directories output_list target)
    if(TARGET "${target}")
        get_target_property(_inc "${target}" INTERFACE_INCLUDE_DIRECTORIES)
        list(APPEND "${output_list}" "${_inc}")
        get_target_property(libraries "${target}" LINK_LIBRARIES)
        foreach(lib ${libraries})
            if(TARGET "${lib}")
                _get_library_include_directories("${output_list}" "${lib}")
            endif()
        endforeach()
        set("${output_list}" "${${output_list}}" PARENT_SCOPE)
    endif()
endfunction()

# Retrieve all include directories for target, including interface directories from dependencies
function(get_target_include_directories output_list target)
    get_target_property(includes "${target}" INCLUDE_DIRECTORIES)
    get_target_property(libraries "${target}" LINK_LIBRARIES)
    foreach(lib ${libraries})
        _get_library_include_directories(includes "${lib}")
    endforeach()
    list(REMOVE_DUPLICATES includes)
    set("${output_list}" "${includes}" PARENT_SCOPE)
endfunction()

# Add a reflection target to the reflection build system
function(add_reflected_target target)
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    get_target_include_directories(includes "${target}")
    list(TRANSFORM includes REPLACE "^(.+)$" "$<IF:$<STREQUAL:\\1,>,,-I\\1>")
    if(pch)
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
    
    # Create list of pch flags
    get_target_property(pch "${target}" PRECOMPILE_HEADERS)
    set(pch_compiled "")
    set(pch_flags "")
    if(pch)
        foreach(header ${pch})
            get_filename_component(filename "${header}" NAME_WE)
            list(APPEND pch_compiled "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
            list(APPEND pch_flags "-include-pch")
            list(APPEND pch_flags "${CMAKE_CURRENT_BINARY_DIR}/${filename}.pch")
        endforeach()
    endif()
    string(REPLACE ";" "," pch_flags "${pch_flags}")
    
    # Get include directories and reflect headers
    get_target_include_directories(includes "${target}")
    list(TRANSFORM includes REPLACE "^(.+)$" "$<IF:$<STREQUAL:\\1,>,,-I\\1>")
    string(REPLACE ";" "," includes "${includes}")
    foreach(header ${headers})
        get_filename_component(abs_root_include "${root_include}" ABSOLUTE)
        get_filename_component(abs_header "${header}" ABSOLUTE)
        file(RELATIVE_PATH rel_path "${abs_root_include}" "${abs_header}")
        string(REGEX REPLACE "\\.[^.]*$" "" rel_path "${rel_path}")
        if(TARGET CalyxReflectTool)
            set(CALYX_REFLECT_TOOL "$<TARGET_FILE:CalyxReflectTool>")
        else()
            set(CALYX_REFLECT_TOOL "$<TARGET_FILE:Calyx::CalyxReflectTool>")
        endif()
        add_custom_command(
                COMMAND "${CALYX_REFLECT_TOOL}"
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