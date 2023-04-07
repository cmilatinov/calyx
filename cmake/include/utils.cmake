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
function(add_dir_copy target src dest)
    add_custom_command(
            TARGET "${target}" PRE_LINK
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${src}" "${dest}"
    )
endfunction()