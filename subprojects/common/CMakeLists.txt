project(${CMAKE_PROJECT_NAME}_common CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(include)
include_directories(source)

file (
    GLOB_RECURSE SOURCE_FILES

    include/*.h
    source/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} fe2d_core)

fe2d_rpc_gen(space common rpc)