# - Find TinyXML
# Find the native TinyXML includes and library
#
#   TINYXML_FOUND        - True if TinyXML found.
#   TINYXML_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   TINYXML_LIBRARIES    - List of libraries when using TinyXML.
#   TIXML_USE_STL        - ON or undefined whether STL support is found
#                          can be used directly in a #cmakedefine
#

IF( TINYXML_INCLUDE_DIR )
    # Already in cache, be silent
    SET( TinyXML_FIND_QUIETLY TRUE )
ENDIF( TINYXML_INCLUDE_DIR )

FIND_PATH( TINYXML_INCLUDE_DIR tinyxml.h
           PATH_SUFFIXES tinyxml )

FIND_LIBRARY( TINYXML_LIBRARY
              NAMES tinyxml
              PATH_SUFFIXES tinyxml )

# handle the QUIETLY and REQUIRED arguments and set TINYXML_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( TinyXML DEFAULT_MSG TINYXML_INCLUDE_DIR TINYXML_LIBRARY )

MARK_AS_ADVANCED( TINYXML_INCLUDE_DIR TINYXML_LIBRARY )

IF(TINYXML_FOUND)

  SET(TINYXML_INCLUDE_DIRS ${TINYXML_INCLUDE_DIR})
  SET(TINYXML_LIBRARIES    ${TINYXML_LIBRARY})

  # Check if STL support is enabled with macro TIXML_USE_STL
  TRY_COMPILE(TIXML_USE_STL
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestTinyXMLUseSTL.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TINYXML_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${TINYXML_LIBRARIES}"
              COMPILE_DEFINITIONS "-DTIXML_USE_STL"
              OUTPUT_VARIABLE OUTPUT)

ELSE(TINYXML_FOUND)

  SET(TINYXML_INCLUDE_DIRS)
  SET(TINYXML_LIBRARIES)
  SET(TIXML_USE_STL)

ENDIF(TINYXML_FOUND)
