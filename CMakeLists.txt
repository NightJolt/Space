cmake_minimum_required(VERSION 3.22.0)

project(space)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})
    
add_subdirectory(FunEngine2D)
add_subdirectory(subprojects)