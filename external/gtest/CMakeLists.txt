option(INSTALL_GTEST "" off)

add_subdirectory(googletest)
set_target_properties(gtest PROPERTIES FOLDER external)
set_target_properties(gtest_main PROPERTIES FOLDER external)
set_target_properties(gmock PROPERTIES FOLDER external)
set_target_properties(gmock_main PROPERTIES FOLDER external)