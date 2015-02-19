# Find MuParserX
#
#   MUPARSERX_FOUND        - True if MuParser found.
#   MUPARSERX_INCLUDE_DIRS - where to find mpParser.h, etc.
#   MUPARSERX_LIBRARIES    - List of libraries when using MuParser.
#

if( MUPARSERX_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParserX_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSERX_INCLUDE_DIR mpParser.h
           PATH_SUFFIXES mpParser )

find_library( MUPARSERX_LIBRARY
              NAMES muparserx
              PATH_SUFFIXES muparserx )

# handle the QUIETLY and REQUIRED arguments and set MUPARSERX_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( MuParserX DEFAULT_MSG MUPARSERX_LIBRARY MUPARSERX_INCLUDE_DIR )

mark_as_advanced( MUPARSERX_INCLUDE_DIR MUPARSERX_LIBRARY )

if(MUPARSERX_FOUND)
  set(MUPARSERX_INCLUDE_DIRS ${MUPARSERX_INCLUDE_DIR})
  set(MUPARSERX_LIBRARIES    ${MUPARSERX_LIBRARY})
else()
  set(MUPARSERX_INCLUDE_DIRS)
  set(MUPARSERX_LIBRARIES)
endif()
