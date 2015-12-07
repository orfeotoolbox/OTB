

FILE(READ ${CMAKE_SOURCE_DIR}/sharedlib/CMakeLists.txt   cmake_lists_orig)
string( REPLACE "-DUSE_SETMODE" "" cmake_lists_patch ${cmake_lists_orig})
file(WRITE ${CMAKE_SOURCE_DIR}/sharedlib/CMakeLists.txt "${cmake_lists_patch}")
