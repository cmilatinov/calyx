# Find packages
find_package(EnTT REQUIRED)

# Library
file(GLOB_RECURSE REFLECT_LIB_SRC "src/**.cpp")
add_library("CalyxReflectLib" STATIC ${REFLECT_LIB_SRC})
target_include_directories("CalyxReflectLib" PUBLIC
        "$<INSTALL_INTERFACE:include/calyx>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries("CalyxReflectLib" PRIVATE EnTT::EnTT)
if(UNIX)
    set_target_properties("CalyxReflectLib" PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# Add header file set
file(GLOB_RECURSE REFLECT_LIB_HEADERS "include/**.h")
list(FILTER REFLECT_LIB_HEADERS EXCLUDE REGEX ".*/pch.h$")
target_sources("CalyxReflectLib" PUBLIC FILE_SET HEADERS BASE_DIRS include FILES ${REFLECT_LIB_HEADERS})