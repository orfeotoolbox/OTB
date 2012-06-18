# - Find PQXX
# Find the native PQXX includes and library
#
#   PQXX_FOUND        - True if PQXX found.
#   PQXX_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   PQXX_LIBRARIES    - List of libraries when using PQXX.
#

IF( PQXX_INCLUDE_DIR )
    # Already in cache, be silent
    SET( PQXX_FIND_QUIETLY TRUE )
ENDIF( PQXX_INCLUDE_DIR )

FIND_PATH( PQXX_INCLUDE_DIR pqxx/pqxx )

FIND_LIBRARY( PQXX_LIBRARY
              NAMES pqxx
              PATH_SUFFIXES pqxx )

# handle the QUIETLY and REQUIRED arguments and set PQXX_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( PQXX DEFAULT_MSG PQXX_INCLUDE_DIR PQXX_LIBRARY )

MARK_AS_ADVANCED( PQXX_INCLUDE_DIR PQXX_LIBRARY )

IF(PQXX_FOUND)
  SET(PQXX_INCLUDE_DIRS ${PQXX_INCLUDE_DIR})
  SET(PQXX_LIBRARIES ${PQXX_LIBRARY})
ELSE(PQXX_FOUND)
  SET(PQXX_INCLUDE_DIRS)
  SET(PQXX_LIBRARIES)
ENDIF(PQXX_FOUND)

