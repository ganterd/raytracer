
#find_package(SDL2 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx -mavx2 -mfma")


add_library(rt SHARED
    rt/math/float4.hpp
    
    rt/scene/ray.hpp

    rt/buffer/buffer.cpp
)
add_dependencies(rt assimp)
SET_TARGET_PROPERTIES(rt PROPERTIES LINKER_LANGUAGE CXX)

