cmake_minimum_required(VERSION 3.10)

project(Raytracer)

set (CMAKE_CXX_STANDARD 11)

# External Libraries
include(ExternalProject)

set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")

add_subdirectory(external)

include_directories(external/assimp/assimp/include)
include_directories(${CMAKE_BINARY_DIR}/external/assimp/assimp/include)

find_package(Threads)

include_directories(${CMAKE_BINARY_DIR}/ext/include)

add_subdirectory(rt_lib)
include_directories(rt_lib)

option(RT_LIB_TESTS "Build the tests" ON)
if(RT_LIB_TESTS)
	enable_testing()
	include(GoogleTest)
	add_subdirectory(rt_lib_test)
endif()

option(RT_MINIMAL_SFML_APP "Build a minimal SFML app" off)
if(RT_MINIMAL_SFML_APP)
	ExternalProject_Add(
		SFML 
		GIT_REPOSITORY https://github.com/SFML/SFML.git 
		CMAKE_ARGS 
			-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext 
			-DSFML_BUILD_AUDIO=Off
			-DSFML_BUILD_NETWORK=Off	
	)

	add_subdirectory(rt_app_minimal_sfml)
endif()

