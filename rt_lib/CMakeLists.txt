
#find_package(SDL2 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(/arch:SSE2)
else(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")
endif(WIN32)

set(
    RT_LIB_HPP_SOURCES

    rt/math/float4.hpp
    rt/math/mat4x4f.hpp
    rt/math/quaternion.hpp
    rt/math/uchar4.hpp
    rt/math/utils.hpp

    rt/buffer/buffer.hpp

    rt/scene/aabb.hpp
    rt/scene/camera.hpp
    rt/scene/ray.hpp
    rt/scene/rayhit.hpp
    rt/scene/scene.hpp
    rt/scene/tri.hpp
)

set(
    RT_LIB_CPP_SOURCES

    rt/scene/scene.cpp
    rt/scene/bvh/bvh.cpp

    rt/buffer/buffer.cpp
)

set(
    RT_LIB_SOURCES
    ${RT_LIB_HPP_SOURCES}
    ${RT_LIB_CPP_SOURCES}
)

foreach(source IN LISTS RT_LIB_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()


add_library(rt SHARED ${RT_LIB_SOURCES})
add_dependencies(rt assimp)
target_link_libraries(rt assimp)
set_target_properties(rt PROPERTIES LINKER_LANGUAGE CXX)

add_custom_command(
    TARGET rt
    POST_BUILD
    COMMAND cmake -E copy_if_different ${CMAKE_BINARY_DIR}/external/assimp/assimp/bin/ ${CMAKE_BINARY_DIR}/bin/
 )