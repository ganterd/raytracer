cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE RT_SOURCES "main.cpp")

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(/arch:SSE)
else(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")
endif(WIN32)

add_executable(pt_app_minimal_ppm ${RT_SOURCES})
add_dependencies(pt_app_minimal_ppm rt CopyExternalBinaries)
target_link_libraries(pt_app_minimal_ppm rt)
