find_package(OpenGL REQUIRED)

mark_as_advanced(OPENGL_INCLUDE_DIR)
mark_as_advanced(OPENGL_LIBRARY)

if(NOT OPENGL_FOUND)
 message(FATAL_ERROR "Cannot find OpenGL. Set OPENGL_INCLUDE_DIR and OPENGL_LIBRARY")
endif()
