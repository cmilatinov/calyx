# Copy a set of files to a target build folder
function(add_file_copy target dest)
    set(files "${ARGN}")
    set(target_files "")
    
    foreach(file ${files})
        get_filename_component(filename "${file}" NAME)
        list(APPEND target_files "${dest}/${filename}")
    endforeach()
    
    # Copy new files
    add_custom_command(
            TARGET "${target}" PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${files}
            "${dest}"
            DEPENDS ${files}
    )
endfunction()

# Copy an entire directory to a target build folder
function(add_dir_copy target dest dir)
    add_custom_command(
            TARGET "${target}" PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${dir}"
            "${dest}/${dir}"
    )
endfunction()

# Add an element to a list if it is not already present in the list
function(list_add_if_not_present list elem)
    list(FIND "${list}" "${elem}" exists)
    if(exists EQUAL -1)
        list(APPEND "${list}" "${elem}")
        set("${list}" "${${list}}" PARENT_SCOPE)
    endif()
endfunction()

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