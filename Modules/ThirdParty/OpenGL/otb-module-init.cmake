#detection of OpenGL is apply is bit tricy as we deactivate
#framework on OSX globally. see mantis #1193
if(APPLE)
  set( OPENGL_FOUND FALSE )
  set( OPENGL_GLU_FOUND FALSE)

  set( OPENGL_INCLUDE_DIR)
  set( OPENGL_LIBRARIES)

  find_path(
    OPENGL_INCLUDE_DIR OpenGL/gl.h
    PATHS "/System/Library/Frameworks/"
    DOC "Include for OpenGL on OSX"
    )

  find_library(
    OPENGL_gl_LIBRARY OpenGL
    PATHS "/System/Library/Frameworks/"
    DOC "OpenGL lib for OSX"
    )
  
  find_library(
    OPENGL_glu_LIBRARY AGL
    PATHS "/System/Library/Frameworks/"
    DOC "AGL lib for OSX"
    )

  if( OPENGL_gl_LIBRARY )
    set( OPENGL_LIBRARIES  ${OPENGL_gl_LIBRARY})
    set( OPENGL_FOUND TRUE )
  endif()
  if( OPENGL_glu_LIBRARY )
    set( OPENGL_GLU_FOUND TRUE)
    set( OPENGL_LIBRARIES  ${OPENGL_LIBRARIES} ${OPENGL_glu_LIBRARY} )
  endif()

  mark_as_advanced(
    OPENGL_INCLUDE_DIR
    OPENGL_LIBRARIES
    OPENGL_glu_LIBRARY
    OPENGL_gl_LIBRARY
    )
else(APPLE)
  find_package(OpenGL REQUIRED)
  mark_as_advanced(OPENGL_INCLUDE_DIR)
  mark_as_advanced(OPENGL_LIBRARIES)
endif(APPLE)


if(NOT OPENGL_INCLUDE_DIR)
  if(NOT WIN32)
    message(FATAL_ERROR "Could not find OpenGL (missing: OPENGL_INCLUDE_DIR")
  endif()
endif()

if(NOT OPENGL_gl_LIBRARY)
  message(FATAL_ERROR "Could not find OpenGL (missing: OPENGL_gl_LIBRARY")
endif()

if(NOT OPENGL_glu_LIBRARY)
  message(FATAL_ERROR "Could not find OpenGL (missing: OPENGL_glu_LIBRARY")
endif()

