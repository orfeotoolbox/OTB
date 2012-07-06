# - Find MuParser
# Find the native MuParser includes and library
#
#   MUPARSER_FOUND       - True if MuParser found.
#   MUPARSER_INCLUDE_DIR - where to find tinyxml.h, etc.
#   MUPARSER_LIBRARIES   - List of libraries when using MuParser.
#

IF( MUPARSER_INCLUDE_DIR )
    # Already in cache, be silent
    SET( MuParser_FIND_QUIETLY TRUE )
ENDIF( MUPARSER_INCLUDE_DIR )

FIND_PATH( MUPARSER_INCLUDE_DIR muParser.h
           PATH_SUFFIXES muParser )

FIND_LIBRARY( MUPARSER_LIBRARIES
              NAMES muparser
              PATH_SUFFIXES muparser )

# handle the QUIETLY and REQUIRED arguments and set MUPARSER_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParser DEFAULT_MSG MUPARSER_LIBRARIES MUPARSER_INCLUDE_DIR )

MARK_AS_ADVANCED( MUPARSER_INCLUDE_DIR MUPARSER_LIBRARIES )
