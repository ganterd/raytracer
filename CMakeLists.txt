cmake_minimum_required(VERSION 3.10)

project(Raytracer)

set (CMAKE_CXX_STANDARD 11)

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

#ExternalProject_Add(
#	ASSIMP
#	GIT_REPOSITORY https://github.com/assimp/assimp.git
#	CMAKE_ARGS
#		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext
#		-DASSIMP_NO_EXPORT=On
#		-DASSIMP_BUILD_ASSIMP_TOOLS=Off
#		-DASSIMP_BUILD_TESTS=Off
#)

set(ASSIMP_NO_EXPORT On)
set(ASSIMP_BUILD_ASSIMP_TOOLS Off)
set(ASSIMP_BUILD_TESTS Off)

add_subdirectory(external/glm)
add_subdirectory(external/assimp)

include_directories(external/glm)
include_directories(external/assimp/include)
include_directories(${CMAKE_BINARY_DIR}/external/assimp/include)

find_package(Threads)
message("CMake Build Dir: " ${CMAKE_BINARY_DIR})

#ExternalProject_Add(
#	glm
#	GIT_REPOSITORY https://github.com/g-truc/glm.git
#	CMAKE_ARGS
#		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext
#		# Going to leave these commented out until the thing actually works
#		#-DGLM_TEST_ENABLE_SIMD_SSE2=On
#		#-DGLM_TEST_ENABLE_SIMD_SSE3=On
#		#-DGLM_TEST_ENABLE_SIMD_AVX=On
#		#-DGLM_TEST_ENABLE_SIMD_AVX2=On
#)

include_directories(${CMAKE_BINARY_DIR}/ext/include)

add_subdirectory(rt_lib)
include_directories(rt_lib)

add_subdirectory(rt_app_minimal_sfml)

