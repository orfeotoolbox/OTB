# - Find Numpy
# Find the native numpy includes
#
#   NUMPY_FOUND        - True if Numpy headers are found.
#   NUMPY_INCLUDE_DIR   - where to find numpy/arrayobject.h, etc.

EXEC_PROGRAM ("${PYTHON_EXECUTABLE}"
  ARGS "${CMAKE_SOURCE_DIR}/CMake/otbTestNumpy.py"
  OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
  RETURN_VALUE NUMPY_NOT_FOUND)

if( NUMPY_INCLUDE_DIR MATCHES "Traceback" )
# Did not successfully include numpy
set( NUMPY_INCLUDE_DIR )
else()
  set( NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} CACHE STRING "Numpy include path." )
endif()

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Numpy DEFAULT_MSG NUMPY_INCLUDE_DIR )

mark_as_advanced( NUMPY_INCLUDE_DIR )

if(NUMPY_FOUND)
  set(NUMPY_INCLUDE_DIRS ${NUMPY_INCLUDE_DIR})
else()
  set(NUMPY_INCLUDE_DIRS)
endif()
