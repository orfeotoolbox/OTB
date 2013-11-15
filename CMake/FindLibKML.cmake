# - Find LibKML
# Find the native LibKML includes and library
#
#   LIBKML_FOUND       - True if LibKML found.
#   LIBKML_INCLUDE_DIR - where to find tinyxml.h, etc.
#   LIBKML_LIBRARIES   - List of libraries when using LibKML.
#

if( LIBKML_INCLUDE_DIR )
    # Already in cache, be silent
    set( LibKML_FIND_QUIETLY TRUE )
endif( LIBKML_INCLUDE_DIR )

find_path( LIBKML_INCLUDE_DIR kml/dom.h )

find_library( LIBKML_BASE_LIBRARY
              NAMES kmlbase )
find_library( LIBKML_CONVENIENCE_LIBRARY
              NAMES kmlconvenience )
find_library( LIBKML_DOM_LIBRARY
              NAMES kmldom )
find_library( LIBKML_ENGINE_LIBRARY
              NAMES kmlengine )
find_library( LIBKML_REGIONATOR_LIBRARY
              NAMES kmlregionator )
find_library( LIBKML_XSD_LIBRARY
              NAMES kmlxsd )
find_library( LIBKML_MINIZIP_LIBRARY
              NAMES minizip )

mark_as_advanced(   LIBKML_INCLUDE_DIR
                    LIBKML_BASE_LIBRARY
                    LIBKML_CONVENIENCE_LIBRARY
                    LIBKML_DOM_LIBRARY
                    LIBKML_ENGINE_LIBRARY
                    LIBKML_REGIONATOR_LIBRARY
                    LIBKML_XSD_LIBRARY
                    LIBKML_MINIZIP_LIBRARY )

# handle the QUIETLY and REQUIRED arguments and set LIBKML_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LibKML DEFAULT_MSG
                                    LIBKML_BASE_LIBRARY
                                    LIBKML_CONVENIENCE_LIBRARY
                                    LIBKML_DOM_LIBRARY
                                    LIBKML_ENGINE_LIBRARY
                                    LIBKML_REGIONATOR_LIBRARY
                                    LIBKML_XSD_LIBRARY
                                    LIBKML_MINIZIP_LIBRARY
                                    LIBKML_INCLUDE_DIR
                                     )

if(LIBKML_FOUND)
  set(LIBKML_INCLUDE_DIRS ${LIBKML_INCLUDE_DIR})
  set(LIBKML_LIBRARIES ${LIBKML_BASE_LIBRARY}
                       ${LIBKML_CONVENIENCE_LIBRARY}
                       ${LIBKML_DOM_LIBRARY}
                       ${LIBKML_ENGINE_LIBRARY}
                       ${LIBKML_REGIONATOR_LIBRARY}
                       ${LIBKML_XSD_LIBRARY}
                       ${LIBKML_MINIZIP_LIBRARY})
endif(LIBKML_FOUND)
