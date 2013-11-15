# - Find LTDL
# Find the native LTDL includes and library
#
#   LTDL_FOUND        - True if LTDL found.
#   LTDL_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   LTDL_LIBRARIES    - List of libraries when using LTDL.
#

if( LTDL_INCLUDE_DIR )
    # Already in cache, be silent
    set( LTDL_FIND_QUIETLY TRUE )
endif()

find_path( LTDL_INCLUDE_DIR ltdl.h )

find_library( LTDL_LIBRARY
              NAMES ltdl )

# handle the QUIETLY and REQUIRED arguments and set LTDL_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LTDL DEFAULT_MSG LTDL_LIBRARY LTDL_INCLUDE_DIR )

mark_as_advanced( LTDL_INCLUDE_DIR LTDL_LIBRARY )

if(LTDL_FOUND)
  set(LTDL_INCLUDE_DIRS ${LTDL_INCLUDE_DIR})
  set(LTDL_LIBRARIES ${LTDL_LIBRARY})
else()
  set(LTDL_INCLUDE_DIRS)
  set(LTDL_LIBRARIES)
endif()

