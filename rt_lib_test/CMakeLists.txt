cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE RT_HEADERS "*.hpp")
file(GLOB_RECURSE RT_SOURCES "*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")

add_executable(rt_lib_tests 
    math/float4.cpp
    math/mat4x4f.cpp
    math/quaternion.cpp

    scene/ray.cpp
    scene/tri.cpp
    scene/aabb.cpp

    buffer/buffer.cpp
)

target_link_libraries(rt_lib_tests rt gtest gmock gtest_main)

gtest_discover_tests(rt_lib_tests)
