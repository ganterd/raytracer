cmake_minimum_required(VERSION 3.10)

project(Raytracer)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ext/SFML/cmake" ${CMAKE_PREFIX_PATH})

# External Libraries
include(ExternalProject)
ExternalProject_Add(
	SFML 
	GIT_REPOSITORY https://github.com/SFML/SFML.git 
	CMAKE_ARGS 
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext 
		-DSFML_BUILD_AUDIO=Off
		-DSFML_BUILD_NETWORK=Off	
)

add_subdirectory(rt_lib)
add_subdirectory(rt_app_minimal_sfml)

