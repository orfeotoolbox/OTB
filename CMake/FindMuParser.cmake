# - Find MuParser
# Find the native MuParser includes and library
#
#   MUPARSER_FOUND        - True if MuParser found.
#   MUPARSER_INCLUDE_DIRS - where to find muParser.h, etc.
#   MUPARSER_LIBRARIES    - List of libraries when using MuParser.
#   MUPARSER_VERSION        - version string
#   MUPARSER_VERSION_MAJOR  - version major number
#   MUPARSER_VERSION_MINOR  - version minor number
#   MUPARSER_VERSION_PATCH  - version patch number
#   MUPARSER_VERSION_NUMBER - version number ((major*100)+minor)*100+patch
#

if( MUPARSER_INCLUDE_DIR )
    # Already in cache, be silent
    set( MuParser_FIND_QUIETLY TRUE )
endif()

find_path( MUPARSER_INCLUDE_DIR muParser.h
           PATH_SUFFIXES muParser )

if(EXISTS "${MUPARSER_INCLUDE_DIR}/muParserDef.h")
  file(READ "${MUPARSER_INCLUDE_DIR}/muParserDef.h" _muParserDef_h_CONTENTS)
  string(REGEX REPLACE ".*# *define MUP_VERSION *_T\\(\"([0-9.]+)\"\\).*" "\\1" MUPARSER_VERSION "${_muParserDef_h_CONTENTS}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MUPARSER_VERSION_MAJOR "${MUPARSER_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MUPARSER_VERSION_MINOR "${MUPARSER_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MUPARSER_VERSION_PATCH "${MUPARSER_VERSION}")
  math(EXPR MUPARSER_VERSION_NUMBER
    "((${MUPARSER_VERSION_MAJOR})*100+${MUPARSER_VERSION_MINOR})*100+${MUPARSER_VERSION_PATCH}")
else()
  if(NOT MuParser_FIND_QUIETLY)
  message(WARNING "muParserDef.h not found !")
  endif()
endif()

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
