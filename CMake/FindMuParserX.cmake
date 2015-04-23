# Find MuParserX
#
#   MUPARSERX_FOUND        - True if MuParserX found.
#   MUPARSERX_INCLUDE_DIRS - where to find mpParser.h, etc.
#   MUPARSERX_LIBRARIES    - List of libraries when using MuParserX.
#   MUPARSERX_VERSION        - version string
#   MUPARSERX_VERSION_MAJOR  - version major number
#   MUPARSERX_VERSION_MINOR  - version minor number
#   MUPARSERX_VERSION_PATCH  - version patch number
#   MUPARSERX_VERSION_NUMBER - version number ((major*100)+minor)*100+patch
#

if( MUPARSERX_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParserX_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSERX_INCLUDE_DIR mpParser.h
           PATH_SUFFIXES mpParser )

if(EXISTS "${MUPARSERX_INCLUDE_DIR}/mpDefines.h")
  file(READ "${MUPARSERX_INCLUDE_DIR}/mpDefines.h" _mpDefines_h_CONTENTS)
  string(REGEX REPLACE ".*# *define MUP_PARSER_VERSION *_T\\(\"([0-9.]+)( .*)?\"\\).*" "\\1" MUPARSERX_VERSION "${_mpDefines_h_CONTENTS}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MUPARSERX_VERSION_MAJOR "${MUPARSERX_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MUPARSERX_VERSION_MINOR "${MUPARSERX_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MUPARSERX_VERSION_PATCH "${MUPARSERX_VERSION}")
  math(EXPR MUPARSERX_VERSION_NUMBER
    "((${MUPARSERX_VERSION_MAJOR})*100+${MUPARSERX_VERSION_MINOR})*100+${MUPARSERX_VERSION_PATCH}")
else()
  if(NOT MuParserX_FIND_QUIETLY)
  message(WARNING "mpDefines.h not found !")
  endif()
endif()


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
