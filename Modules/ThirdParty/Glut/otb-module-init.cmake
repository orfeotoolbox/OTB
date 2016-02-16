find_package(GLUT REQUIRED)

mark_as_advanced(GLUT_INCLUDE_DIR)
mark_as_advanced(GLUT_LIBRARY)

if(NOT GLUT_FOUND)
 message(FATAL_ERROR "Cannot find GLUT. Set GLUT_INCLUDE_DIR and GLUT_LIBRARY")
endif()
