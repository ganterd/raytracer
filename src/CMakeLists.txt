cmake_minimum_required(VERSION 3.10)

project(Raytracer)

file(GLOB_RECURSE RT_HEADERS "*.hpp")
file(GLOB_RECURSE RT_SOURCES "*.cpp")

add_executable(raytracer ${RT_HEADERS} ${RT_SOURCES})
