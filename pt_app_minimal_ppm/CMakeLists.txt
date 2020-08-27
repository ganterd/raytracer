cmake_minimum_required(VERSION 3.10)

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(/arch:SSE2)
else(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")
endif(WIN32)

add_executable(
    pt_app_minimal_ppm 

    main.cpp
)
add_dependencies(pt_app_minimal_ppm rt)
target_link_libraries(pt_app_minimal_ppm rt)

# Link the resources folder into te binary directory
add_custom_command(
    TARGET pt_app_minimal_ppm 
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${resource_target} ${resource_destination} DEPENDS ${resource_destination}
)

set_property(
    TARGET pt_app_minimal_ppm
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)