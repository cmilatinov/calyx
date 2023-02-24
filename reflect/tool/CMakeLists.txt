# Find packages
find_package(Clang REQUIRED)
find_package(inja REQUIRED)
find_package(cxxopts REQUIRED)
find_package(nlohmann_json REQUIRED)

# Source
file(GLOB_RECURSE REFLECT_SRC "src/**.cpp")
add_executable("CalyxReflectTool" ${REFLECT_SRC})

# Target definitions, includes, libraries
target_compile_definitions("CalyxReflectTool" PRIVATE INJA_NOEXCEPTION ${LLVM_DEFINITIONS})
target_include_directories("CalyxReflectTool" PRIVATE include ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
target_link_libraries("CalyxReflectTool" PRIVATE
        CalyxUtils
        CalyxReflectLib
        libclang
        clang-cpp
        LLVM
        cxxopts::cxxopts
        pantor::inja
        nlohmann_json::nlohmann_json
        )
target_precompile_headers("CalyxReflectTool" PRIVATE include/pch.h)
