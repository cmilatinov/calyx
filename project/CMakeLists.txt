# Required packages
find_package(inja REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem)

# Clump source files from src
file(GLOB_RECURSE PROJECT_SRC "src/**.cpp")

# Create executable from source
add_library("CalyxProject" STATIC ${PROJECT_SRC})

# Link targets
target_link_libraries("CalyxProject" PRIVATE CalyxEngine pantor::inja Boost::boost Boost::system Boost::filesystem)
target_include_directories("CalyxProject" PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include/calyx>")
target_precompile_headers("CalyxProject" PRIVATE "include/pch.h")