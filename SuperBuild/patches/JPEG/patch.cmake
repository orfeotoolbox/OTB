file(READ ${CMAKE_SOURCE_DIR}/CMakeLists.txt   cmake_lists_orig1)
string( REPLACE "-DUSE_SETMODE" "" cmake_lists_patch1 ${cmake_lists_orig1})
file(WRITE ${CMAKE_SOURCE_DIR}/CMakeLists.txt "${cmake_lists_patch1}")

file(READ ${CMAKE_SOURCE_DIR}/sharedlib/CMakeLists.txt   cmake_lists_orig)
string( REPLACE "-DUSE_SETMODE" "" cmake_lists_patch ${cmake_lists_orig})
file(WRITE ${CMAKE_SOURCE_DIR}/sharedlib/CMakeLists.txt "${cmake_lists_patch}")
