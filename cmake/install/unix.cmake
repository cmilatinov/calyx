message(STATUS "Installing: /etc/ld.so.conf.d/lib.conf")
execute_process(COMMAND sudo mkdir -p /etc/ld.so.conf.d)
execute_process(COMMAND sudo cp "${CMAKE_CURRENT_BINARY_DIR}/lib.conf" "/etc/ld.so.conf.d/lib.conf")
execute_process(COMMAND sudo ldconfig)