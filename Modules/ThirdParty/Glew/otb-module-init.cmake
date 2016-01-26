find_package(Glew REQUIRED)

mark_as_advanced(GLEW_INCLUDE_DIR)
mark_as_advanced(GLEW_LIBRARY)

if(NOT GLEW_FOUND)
 message(FATAL_ERROR "Cannot find Glew. Set GLEW_INCLUDE_DIR and GLEW_LIBRARY")
endif()
