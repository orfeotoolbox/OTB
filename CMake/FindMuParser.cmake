# - Find MuParser
# Find the native MuParser includes and library
#
#   MUPARSER_FOUND        - True if MuParser found.
#   MUPARSER_INCLUDE_DIRS - where to find muParser.h, etc.
#   MUPARSER_LIBRARIES    - List of libraries when using MuParser.
#

if( MUPARSER_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParser_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSER_INCLUDE_DIR muParser.h
           PATH_SUFFIXES muParser )

find_library( MUPARSER_LIBRARY
              NAMES muparser
              PATH_SUFFIXES muparser )

# handle the QUIETLY and REQUIRED arguments and set MUPARSER_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParser DEFAULT_MSG MUPARSER_LIBRARY MUPARSER_INCLUDE_DIR )

mark_as_advanced( MUPARSER_INCLUDE_DIR MUPARSER_LIBRARY )

if(MUPARSER_FOUND)
  set(MUPARSER_INCLUDE_DIRS ${MUPARSER_INCLUDE_DIR})
  set(MUPARSER_LIBRARIES    ${MUPARSER_LIBRARY})
else()
  set(MUPARSER_INCLUDE_DIRS)
  set(MUPARSER_LIBRARIES)
endif()
