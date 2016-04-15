#detection of OpenGL is apply is bit tricy as we deactivate
#framework on OSX globally. see mantis #1193
if(APPLE)
  set(OPENGL_FOUND FALSE)
  find_library(OPENGL_gl_LIBRARY OpenGL DOC "OpenGL lib for OSX")
  find_library(OPENGL_glu_LIBRARY AGL DOC "AGL lib for OSX")
  find_path(OPENGL_INCLUDE_DIR OpenGL/gl.h DOC "Include for OpenGL on OSX")
  if(OPENGL_gl_LIBRARY)
    set( OPENGL_LIBRARIES  ${OPENGL_gl_LIBRARY} ${OPENGL_LIBRARIES})
    if(OPENGL_glu_LIBRARY)
      set( OPENGL_GLU_FOUND "YES" )
      set( OPENGL_LIBRARIES ${OPENGL_glu_LIBRARY} ${OPENGL_LIBRARIES} )
    else()
      set( OPENGL_GLU_FOUND "NO" )
    endif()
    # This deprecated setting is for backward compatibility with CMake1.4
    set (OPENGL_LIBRARY ${OPENGL_LIBRARIES})
  endif()

  # This deprecated setting is for backward compatibility with CMake1.4
  set(OPENGL_INCLUDE_PATH ${OPENGL_INCLUDE_DIR})
  mark_as_advanced(
    OPENGL_INCLUDE_DIR
    OPENGL_LIBRARY
    OPENGL_glu_LIBRARY
    OPENGL_gl_LIBRARY
    )
  if(NOT OPENGL_LIBRARY OR NOT OPENGL_INCLUDE_DIR)
    message(FATAL_ERROR "Cannot find OpenGL. Set OPENGL_INCLUDE_DIR and OPENGL_LIBRARY")
  endif()

  set(OPENGL_FOUND TRUE)
else(APPLE)
  find_package(OpenGL REQUIRED)
  mark_as_advanced(OPENGL_INCLUDE_DIR)
  mark_as_advanced(OPENGL_LIBRARY)
  if(NOT OPENGL_FOUND)
    message(FATAL_ERROR "Cannot find OpenGL. Set OPENGL_INCLUDE_DIR and OPENGL_LIBRARY")
  endif()
endif(APPLE)
