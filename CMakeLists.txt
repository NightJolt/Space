cmake_minimum_required(VERSION 3.22.0)

project(projectname)

set(CMAKE_CXX_STANDARD 20)

if (FE2D_WINDOWS)

    add_subdirectory(FunEngine2D)
    add_subdirectory(subprojects)

endif()