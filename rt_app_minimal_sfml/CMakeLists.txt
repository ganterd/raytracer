cmake_minimum_required(VERSION 3.10)

#find_package(SFML REQUIRED graphics window system)

file(GLOB_RECURSE RT_HEADERS "*.hpp")
file(GLOB_RECURSE RT_SOURCES "*.cpp")

message("SDL " ${SFML_INCLUDE_DIR} " asd")



add_executable(rt_app_minimal_sfml ${RT_HEADERS} ${RT_SOURCES})
add_dependencies(rt_app_minimal_sfml rt SFML assimp)
target_link_libraries(rt_app_minimal_sfml rt sfml-window sfml-graphics sfml-system assimp)
