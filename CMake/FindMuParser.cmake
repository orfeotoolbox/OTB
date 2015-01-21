# - Find MuParser
# Find the native MuParser includes and library
#
#   MUPARSER_FOUND       - True if MuParser found.
#   MUPARSER_INCLUDE_DIR - where to find tinyxml.h, etc.
#   MUPARSER_LIBRARIES   - List of libraries when using MuParser.
#

if( MUPARSER_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParser_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSER_INCLUDE_DIR muParser.h
           PATHS 
           ${CMAKE_PREFIX_PATH}/include
           PATH_SUFFIXES muParser )

find_library( MUPARSER_LIBRARIES
              NAMES muparser
              PATHS
              ${CMAKE_PREFIX_PATH}/lib
              PATH_SUFFIXES muparser )

# handle the QUIETLY and REQUIRED arguments and set MUPARSER_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParser DEFAULT_MSG MUPARSER_LIBRARIES MUPARSER_INCLUDE_DIR )

mark_as_advanced( MUPARSER_INCLUDE_DIR MUPARSER_LIBRARIES )
