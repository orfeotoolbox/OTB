# - Find MuParserX
# Find the native MuParser includes and library
#
#   MUPARSERX_FOUND       - True if MuParser found.
#   MUPARSERX_INCLUDE_DIR - where to find tinyxml.h, etc.
#   MUPARSERX_LIBRARIES   - List of libraries when using MuParser.
#

if( MUPARSERX_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParserX_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSERX_INCLUDE_DIR mpParser.h
           PATHS 
           ${CMAKE_PREFIX_PATH}/include
           PATH_SUFFIXES mpParser )

find_library( MUPARSERX_LIBRARIES
              NAMES muparserx
              PATHS 
              ${CMAKE_PREFIX_PATH}/lib
              PATH_SUFFIXES muparserx )

# handle the QUIETLY and REQUIRED arguments and set MUPARSERX_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParserX DEFAULT_MSG MUPARSERX_LIBRARIES MUPARSERX_INCLUDE_DIR )

mark_as_advanced( MUPARSERX_INCLUDE_DIR MUPARSERX_LIBRARIES )
