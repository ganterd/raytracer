cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE RT_HEADERS "*.hpp")
file(GLOB_RECURSE RT_SOURCES "*.cpp")

add_executable(rt_app_minimal_sdl ${RT_HEADERS} ${RT_SOURCES})
