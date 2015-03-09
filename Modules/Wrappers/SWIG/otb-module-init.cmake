option ( OTB_WRAP_PYTHON "Wrap Python" OFF )
option ( OTB_WRAP_JAVA   "Wrap Java"   OFF )

if ( OTB_WRAP_PYTHON OR OTB_WRAP_JAVA )
  find_package ( SWIG REQUIRED )
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)
endif()

#
# SimpleITK does this check, but it does not seem necessary in our case
# TODO: figure out why SimpleITK needs this...
#
macro(check_PIC_flag Language)
  if ( UNIX AND NOT APPLE )
    if ( NOT ${CMAKE_CXX_FLAGS} MATCHES "-fPIC")
      # message ( FATAL_ERROR "${Language} wrapping requires CMAKE_CXX_FLAGS (or equivalent) to include -fPIC and ITK built with this flag" )
    endif()
  endif()
endmacro()


if ( OTB_WRAP_PYTHON )
  check_PIC_flag ( Python )
  find_package ( PythonLibs REQUIRED )
  find_package ( PythonInterp REQUIRED )
endif()

#
# JAVA SWIG configuration
#
if ( OTB_WRAP_JAVA )
  check_PIC_flag ( Java )
  find_package ( Java REQUIRED )
  find_package ( JNI REQUIRED )
endif()