
#find_package(SDL2 REQUIRED)

file(GLOB_RECURSE RT_LIB_HEADERS "*.hpp")
file(GLOB_RECURSE RT_LIB_SOURCES "*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mavx2 -mfma")


add_library(rt SHARED ${RT_LIB_HEADERS} ${RT_LIB_SOURCES})
add_dependencies(rt assimp glm)
