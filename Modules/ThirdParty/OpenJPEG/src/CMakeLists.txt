# so that we have both public headers (openjpeg.h and openjpeg_mangle.h) in 
# in the OpenJPEG/src/otbopenjpeg subdir, for external module interface
file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/otbopenjpeg )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/otbopenjpeg/libopenjpeg/openjpeg.h
  ${CMAKE_CURRENT_BINARY_DIR}/otbopenjpeg
  )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/otb_openjpeg.h
  ${CMAKE_CURRENT_BINARY_DIR}
  )

add_subdirectory( otbopenjpeg )