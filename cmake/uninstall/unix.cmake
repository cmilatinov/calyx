message(STATUS "Uninstalling: /etc/ld.so.conf.d/lib.conf")
execute_process(COMMAND sudo rm -f /etc/ld.so.conf.d/lib.conf)
execute_process(COMMAND sudo ldconfig)