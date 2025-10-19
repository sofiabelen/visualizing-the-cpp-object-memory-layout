cmake_minimum_required(VERSION 3.25)

# Project name and C++ standard
project(VisualizingCppObjectMemoryLayout VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable out-of-source builds (build in separate folder)
if(NOT CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
endif()

# Add executables
add_executable(vtable part_1_single_inheritance/vtable.cpp)
add_executable(object_layout part_1_single_inheritance/object_layout.cpp)
