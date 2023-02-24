# Required packages
if(UNIX)
    find_library(UUID_LIB NAMES uuid libuuid REQUIRED)
endif()
find_package(fmt REQUIRED)
find_package(stduuid REQUIRED)

# Clump header files from include
file(GLOB_RECURSE UTILS_SRC "src/**.cpp")

# Create library with interface include directory
add_library("CalyxUtils" STATIC ${UTILS_SRC})
if(UNIX)
    set_target_properties("CalyxUtils" PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_link_libraries("CalyxUtils" PUBLIC "${UUID_LIB}")
endif()
target_link_libraries("CalyxUtils" PRIVATE fmt::fmt-header-only stduuid)
target_include_directories("CalyxUtils" PUBLIC
        "$<INSTALL_INTERFACE:include/calyx>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_definitions("CalyxUtils" PRIVATE "UUID_SYSTEM_GENERATOR")

# Add header file set
file(GLOB_RECURSE UTILS_HEADERS "include/**.h")
list(FILTER UTILS_HEADERS EXCLUDE REGEX ".*/pch.h$")
target_sources("CalyxUtils" PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${UTILS_HEADERS})