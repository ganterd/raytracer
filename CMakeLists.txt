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

ExternalProject_Add(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext
		-DASSIMP_NO_EXPORT=On
		-DASSIMP_BUILD_ASSIMP_TOOLS=Off
		-DASSIMP_BUILD_TESTS=Off
)

ExternalProject_Add(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/ext
		# Going to leave these commented out until the thing actually works
		#-DGLM_TEST_ENABLE_SIMD_SSE2=On
		#-DGLM_TEST_ENABLE_SIMD_SSE3=On
		#-DGLM_TEST_ENABLE_SIMD_AVX=On
		#-DGLM_TEST_ENABLE_SIMD_AVX2=On
)

add_subdirectory(rt_lib)
add_subdirectory(rt_app_minimal_sfml)

