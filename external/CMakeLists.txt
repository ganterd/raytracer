
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

add_subdirectory(assimp)
add_subdirectory(gtest)

if(WIN32)
	add_custom_target(CopyExternalBinaries
	  COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/bin

	  COMMAND cmake -E copy_directory ${CMAKE_BINARY_DIR}/external/assimp/assimp/bin/ ${CMAKE_BINARY_DIR}/bin/
	)
else()
	add_custom_target(CopyExternalBinaries )
endif()