SET(TEMP ${CMAKE_BINARY_DIR}/Testing/Temporary)
FILE(MAKE_DIRECTORY ${TEMP})

add_subdirectory(Core)
add_subdirectory(Python)
add_subdirectory(Java)
add_subdirectory(QtWidget)