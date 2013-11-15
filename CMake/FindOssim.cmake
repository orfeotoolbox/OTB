# - Find Ossim
# Find the native Ossim includes and library
#
#   OSSIM_FOUND        - True if Ossim found.
#   OSSIM_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   OSSIM_LIBRARIES    - List of libraries when using Ossim.
#

if( OSSIM_INCLUDE_DIR )
    # Already in cache, be silent
    set( Ossim_FIND_QUIETLY TRUE )
endif( OSSIM_INCLUDE_DIR )

find_path( OSSIM_INCLUDE_DIR
           NAMES ossim/init/ossimInit.h
           PATHS $ENV{OSSIM_INCLUDE_DIR} /usr/local )

find_library(OSSIM_LIBRARY
             NAMES ossim
             PATHS /usr/local/lib/ossim)

# handle the QUIETLY and REQUIRED arguments and set OSSIM_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Ossim DEFAULT_MSG OSSIM_LIBRARY OSSIM_INCLUDE_DIR )

mark_as_advanced( OSSIM_INCLUDE_DIR OSSIM_LIBRARY )

if(OSSIM_FOUND)
  set(OSSIM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIR})
  set(OSSIM_LIBRARIES ${OSSIM_LIBRARY})
else(OSSIM_FOUND)
  set(OSSIM_INCLUDE_DIRS)
  set(OSSIM_LIBRARIES)
endif(OSSIM_FOUND)

