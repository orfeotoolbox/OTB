# - Find LTDL
# Find the native LTDL includes and library
#
#   LTDL_FOUND        - True if LTDL found.
#   LTDL_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   LTDL_LIBRARIES    - List of libraries when using LTDL.
#

IF( LTDL_INCLUDE_DIR )
    # Already in cache, be silent
    SET( LTDL_FIND_QUIETLY TRUE )
ENDIF( LTDL_INCLUDE_DIR )

FIND_PATH( LTDL_INCLUDE_DIR ltdl.h )

FIND_LIBRARY( LTDL_LIBRARY
              NAMES ltdl )

# handle the QUIETLY and REQUIRED arguments and set LTDL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LTDL DEFAULT_MSG LTDL_INCLUDE_DIR LTDL_LIBRARY )

MARK_AS_ADVANCED( LTDL_INCLUDE_DIR LTDL_LIBRARY )

IF(LTDL_FOUND)
  SET(LTDL_INCLUDE_DIRS ${LTDL_INCLUDE_DIR})
  SET(LTDL_LIBRARIES ${LTDL_LIBRARY})
ELSE(LTDL_FOUND)
  SET(LTDL_INCLUDE_DIRS)
  SET(LTDL_LIBRARIES)
ENDIF(LTDL_FOUND)

