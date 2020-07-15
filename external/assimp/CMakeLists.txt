option(ASSIMP_BUILD_ASSIMP_TOOLS "" off)
option(ASSIMP_INSTALL "Disable this if you want to use assimp as a submodule." OFF)
option(ASSIMP_NO_EXPORT "" on)
option(ASSIMP_BUILD_TESTS "" off)

add_subdirectory(assimp)

include_directories(assimp/include)