option(ASSIMP_BUILD_ASSIMP_TOOLS "" off)
option(ASSIMP_INSTALL "Disable this if you want to use assimp as a submodule." OFF)
option(ASSIMP_NO_EXPORT "" on)
option(ASSIMP_BUILD_TESTS "" off)
SET(ASSIMP_LIBRARY_SUFFIX "" CACHE STRING "")

add_subdirectory(assimp)
set_target_properties(assimp PROPERTIES FOLDER external)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER external)
set_target_properties(IrrXML PROPERTIES FOLDER external)
set_target_properties(zlibstatic PROPERTIES FOLDER external)
set_target_properties(uninstall PROPERTIES FOLDER external)

include_directories(assimp/include)