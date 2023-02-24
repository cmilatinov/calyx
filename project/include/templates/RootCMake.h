#pragma once

static const char* TEMPLATE_ROOT_CMAKE =
    R"(
cmake_minimum_required(VERSION 3.23)

project("{{ project.name }}")

include("cmake/assemblies.cmake")
)";