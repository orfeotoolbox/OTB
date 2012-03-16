# - Find Ossim
# Find the native Ossim includes and library
#
#   OSSIM_FOUND        - True if Ossim found.
#   OSSIM_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   OSSIM_LIBRARIES    - List of libraries when using Ossim.
#

IF( OSSIM_INCLUDE_DIR )
    # Already in cache, be silent
    SET( Ossim_FIND_QUIETLY TRUE )
ENDIF( OSSIM_INCLUDE_DIR )

FIND_PATH( OSSIM_INCLUDE_DIR
           NAMES ossim/init/ossimInit.h 
           PATHS $ENV{OSSIM_INCLUDE_DIR} /usr/local )
           
FIND_LIBRARY(OSSIM_LIBRARY
             NAMES ossim
             PATHS /usr/local/lib/ossim)
 
# handle the QUIETLY and REQUIRED arguments and set OSSIM_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( Ossim DEFAULT_MSG OSSIM_INCLUDE_DIR OSSIM_LIBRARY )

MARK_AS_ADVANCED( OSSIM_INCLUDE_DIR OSSIM_LIBRARY )

IF(OSSIM_FOUND)
  SET(OSSIM_INCLUDE_DIRS ${OSSIM_INCLUDE_DIR})
  SET(OSSIM_LIBRARIES ${OSSIM_LIBRARY})
ELSE(OSSIM_FOUND)
  SET(OSSIM_INCLUDE_DIRS)
  SET(OSSIM_LIBRARIES)
ENDIF(OSSIM_FOUND)

