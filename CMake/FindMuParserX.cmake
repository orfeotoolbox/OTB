# - Find MuParserX
# Find the native MuParser includes and library
#
#   MUPARSER_FOUND       - True if MuParser found.
#   MUPARSER_INCLUDE_DIR - where to find tinyxml.h, etc.
#   MUPARSER_LIBRARIES   - List of libraries when using MuParser.
#

if( MUPARSERX_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParserX_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSERX_INCLUDE_DIR mpParser.h
           PATH_SUFFIXES mpParser )

find_library( MUPARSERX_LIBRARIES
              NAMES muparserx
              PATH_SUFFIXES muparserx )

# handle the QUIETLY and REQUIRED arguments and set MUPARSER_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParserX DEFAULT_MSG MUPARSERX_LIBRARIES MUPARSERX_INCLUDE_DIR )

mark_as_advanced( MUPARSERX_INCLUDE_DIR MUPARSERX_LIBRARIES )
