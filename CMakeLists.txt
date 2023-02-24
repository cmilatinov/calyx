# CMAKE Minimum Version
cmake_minimum_required(VERSION 3.23)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCX_DEBUG")

# Project name
project("Calyx")
set(CALYX_VERSION 1.0.0)

# Subdirectories
add_subdirectory("utils")
add_subdirectory("reflect")
add_subdirectory("engine")
add_subdirectory("project")
add_subdirectory("editor")
add_subdirectory("launcher")

# Install targets
include(GNUInstallDirs)
install(TARGETS CalyxUtils CalyxReflectLib CalyxEngine CalyxProject CalyxEditor
        EXPORT CalyxTargets
        FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/calyx"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/calyx"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/calyx")
install(TARGETS CalyxLauncher
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/calyx")
install(DIRECTORY "${CMAKE_BINARY_DIR}/editor/assets" DESTINATION "${CMAKE_INSTALL_BINDIR}/calyx")

# Install exports & package
include(CMakePackageConfigHelpers)
string(TOLOWER "${CMAKE_PROJECT_NAME}" PROJECT_NAME_LOWER)
install(EXPORT CalyxTargets
        FILE CalyxTargets.cmake
        NAMESPACE Calyx::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME_LOWER}")
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CalyxConfigVersion.cmake"
        VERSION "${CALYX_VERSION}"
        COMPATIBILITY AnyNewerVersion)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/CalyxConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME_LOWER}")
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/CalyxConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CalyxConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME_LOWER})

# Add ldconfig file on UNIX platforms
if(UNIX)
    set(CMAKE_INSTALL_FINAL_LIBDIR "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/lib.conf.in"
            "${CMAKE_CURRENT_BINARY_DIR}/lib.conf"
            IMMEDIATE @ONLY)
    unset(CMAKE_INSTALL_FINAL_LIBDIR)
    install(SCRIPT cmake/install/unix.cmake)
endif()

# Uninstall
if(NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)
    
    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()