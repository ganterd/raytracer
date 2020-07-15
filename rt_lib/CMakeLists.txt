
#find_package(SDL2 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(/arch:SSE)
else(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")
endif(WIN32)


add_library(rt SHARED
    rt/math/float4.hpp
    
    rt/scene/ray.hpp
    rt/scene/scene.cpp
    rt/scene/bvh/bvh.cpp

    rt/buffer/buffer.cpp
)
add_dependencies(rt assimp)
target_link_libraries(rt assimp)
set_target_properties(rt PROPERTIES LINKER_LANGUAGE CXX)

