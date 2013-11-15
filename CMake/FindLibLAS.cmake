# - Find LIBLAS
# Find the native LIBLAS includes and library
#
#   LIBLAS_FOUND        - True if LIBLAS found.
#   LIBLAS_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   LIBLAS_LIBRARIES    - List of libraries when using LIBLAS.
#

if( LIBLAS_INCLUDE_DIR )
    # Already in cache, be silent
    set( LIBLAS_FIND_QUIETLY TRUE )
endif()

find_path( LIBLAS_INCLUDE_DIR liblas/capi/liblas.h )

find_library( LIBLAS_LIBRARY
              NAMES liblas_c liblas )

# handle the QUIETLY and REQUIRED arguments and set LIBLAS_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LIBLAS DEFAULT_MSG LIBLAS_LIBRARY LIBLAS_INCLUDE_DIR )

mark_as_advanced( LIBLAS_INCLUDE_DIR LIBLAS_LIBRARY )

if(LIBLAS_FOUND)
  set(LIBLAS_INCLUDE_DIRS ${LIBLAS_INCLUDE_DIR})
  set(LIBLAS_LIBRARIES ${LIBLAS_LIBRARY})
else()
  set(LIBLAS_INCLUDE_DIRS)
  set(LIBLAS_LIBRARIES)
endif()

