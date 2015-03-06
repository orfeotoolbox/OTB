# so that we have both public headers (openjpeg.h and openjpeg_mangle.h) in 
# in the OpenJPEG/src/otbopenjpeg subdir, for external module interface
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/otbopenjpeg )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/otbopenjpeg/libopenjpeg/openjpeg.h
  ${CMAKE_CURRENT_BINARY_DIR}/otbopenjpeg
  )

if(WIN32)
  # TODO : change the static/shared detection when using external openjpeg
  if(BUILD_SHARED_LIBS)
    set(OPJ_EXPORTS 1)
    unset(OPJ_STATIC)
  else()
    unset(OPJ_EXPORTS)
    set(OPJ_STATIC  1)
  endif()
else()
  unset(OPJ_STATIC)
  unset(OPJ_EXPORTS)
endif()

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/otb_openjpeg.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/otb_openjpeg.h
  )

add_subdirectory( otbopenjpeg )
