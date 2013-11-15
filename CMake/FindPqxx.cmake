# - Find PQXX
# Find the native PQXX includes and library
#
#   PQXX_FOUND        - True if PQXX found.
#   PQXX_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   PQXX_LIBRARIES    - List of libraries when using PQXX.
#

if( PQXX_INCLUDE_DIR )
    # Already in cache, be silent
    set( PQXX_FIND_QUIETLY TRUE )
endif( PQXX_INCLUDE_DIR )

find_path( PQXX_INCLUDE_DIR pqxx/pqxx )

find_library( PQXX_LIBRARY
              NAMES pqxx
              PATH_SUFFIXES pqxx )

# handle the QUIETLY and REQUIRED arguments and set PQXX_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( PQXX DEFAULT_MSG PQXX_LIBRARY PQXX_INCLUDE_DIR )

mark_as_advanced( PQXX_INCLUDE_DIR PQXX_LIBRARY )

if(PQXX_FOUND)
  set(PQXX_INCLUDE_DIRS ${PQXX_INCLUDE_DIR})
  set(PQXX_LIBRARIES ${PQXX_LIBRARY})
else(PQXX_FOUND)
  set(PQXX_INCLUDE_DIRS)
  set(PQXX_LIBRARIES)
endif(PQXX_FOUND)

