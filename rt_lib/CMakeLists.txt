
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE RT_LIB_HEADERS "*.hpp")
file(GLOB_RECURSE RT_LIB_SOURCES "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(rt SHARED ${RT_LIB_HEADERS} ${RT_LIB_SOURCES})
add_dependencies(rt assimp glm)
