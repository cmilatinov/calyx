# Minimum Version
cmake_minimum_required(VERSION 3.17)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
set(REFLECT_SRC "src/reflect.cpp" "src/main.cpp")

# Create executable from sources
add_executable("Reflect" ${REFLECT_SRC})

# Set target includes
target_include_directories("Reflect" PUBLIC "include")