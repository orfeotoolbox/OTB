# - Find TinyXML
# Find the native TinyXML includes and library
#
#   TINYXML_FOUND        - True if TinyXML found.
#   TINYXML_INCLUDE_DIRS - where to find tinyxml.h, etc.
#   TINYXML_LIBRARIES    - List of libraries when using TinyXML.
#

if( TINYXML_INCLUDE_DIR )
    # Already in cache, be silent
    set( TinyXML_FIND_QUIETLY TRUE )
endif( TINYXML_INCLUDE_DIR )

find_path( TINYXML_INCLUDE_DIR tinyxml.h
           PATH_SUFFIXES tinyxml )

find_library( TINYXML_LIBRARY
              NAMES tinyxml
              PATH_SUFFIXES tinyxml )

# handle the QUIETLY and REQUIRED arguments and set TINYXML_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( TinyXML DEFAULT_MSG TINYXML_LIBRARY TINYXML_INCLUDE_DIR )

mark_as_advanced( TINYXML_INCLUDE_DIR TINYXML_LIBRARY )

if(TINYXML_FOUND)

  set(TINYXML_INCLUDE_DIRS ${TINYXML_INCLUDE_DIR})
  set(TINYXML_LIBRARIES    ${TINYXML_LIBRARY})

else(TINYXML_FOUND)

  set(TINYXML_INCLUDE_DIRS)
  set(TINYXML_LIBRARIES)

endif(TINYXML_FOUND)
