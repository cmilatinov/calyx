# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# Includes
include(../include/utils.cmake)

# Find packages
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)

# Generator
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(LLVM_LIBRARIES support)

file(GLOB_RECURSE REFLECT_SRC "src/**.cpp")
add_executable("Reflect" ${REFLECT_SRC})
target_compile_definitions("Reflect" PUBLIC ${LLVM_DEFINITIONS_LIST} INJA_NOEXCEPTION)
target_include_directories("Reflect" PUBLIC include generated ${LLVM_INCLUDE_DIRS})
target_link_libraries("Reflect" PUBLIC
        libclang clangAST clangASTMatchers clangBasic clangFrontend clangRewrite clangTooling
        ${LLVM_LIBRARIES}
        nlohmann_json::nlohmann_json
        pantor::inja
)
target_precompile_headers("Reflect" PUBLIC include/pch.h)
add_dir_copy("Reflect" "$<TARGET_FILE_DIR:Reflect>" "templates")

# Test build target
file(GLOB_RECURSE REFLECT_TEST_SRC "src/reflect.cpp" "test/**.cpp" "generated/**.cpp")
add_executable("ReflectTest" ${REFLECT_TEST_SRC})
target_include_directories("ReflectTest" PUBLIC include generated)
target_precompile_headers("ReflectTest" PUBLIC include/reflect.h)
add_reflected_target("ReflectTest")
add_reflected_headers("ReflectTest" "include" "include/test.h")
