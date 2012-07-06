# - Find LibKML
# Find the native LibKML includes and library
#
#   LIBKML_FOUND       - True if LibKML found.
#   LIBKML_INCLUDE_DIR - where to find tinyxml.h, etc.
#   LIBKML_LIBRARIES   - List of libraries when using LibKML.
#

IF( LIBKML_INCLUDE_DIR )
    # Already in cache, be silent
    SET( LibKML_FIND_QUIETLY TRUE )
ENDIF( LIBKML_INCLUDE_DIR )

FIND_PATH( LIBKML_INCLUDE_DIR kml/dom.h )

FIND_LIBRARY( LIBKML_BASE_LIBRARY
              NAMES kmlbase )
FIND_LIBRARY( LIBKML_CONVENIENCE_LIBRARY
              NAMES kmlconvenience )
FIND_LIBRARY( LIBKML_DOM_LIBRARY
              NAMES kmldom )
FIND_LIBRARY( LIBKML_ENGINE_LIBRARY
              NAMES kmlengine )
FIND_LIBRARY( LIBKML_REGIONATOR_LIBRARY
              NAMES kmlregionator )
FIND_LIBRARY( LIBKML_XSD_LIBRARY
              NAMES kmlxsd )
FIND_LIBRARY( LIBKML_MINIZIP_LIBRARY
              NAMES minizip )

MARK_AS_ADVANCED(   LIBKML_INCLUDE_DIR
                    LIBKML_BASE_LIBRARY
                    LIBKML_CONVENIENCE_LIBRARY
                    LIBKML_DOM_LIBRARY
                    LIBKML_ENGINE_LIBRARY
                    LIBKML_REGIONATOR_LIBRARY
                    LIBKML_XSD_LIBRARY
                    LIBKML_MINIZIP_LIBRARY )

# handle the QUIETLY and REQUIRED arguments and set LIBKML_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( FindPackageHandleStandardArgs )
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

IF(LIBKML_FOUND)
  SET(LIBKML_INCLUDE_DIRS ${LIBKML_INCLUDE_DIR})
  SET(LIBKML_LIBRARIES ${LIBKML_BASE_LIBRARY}
                       ${LIBKML_CONVENIENCE_LIBRARY}
                       ${LIBKML_DOM_LIBRARY}
                       ${LIBKML_ENGINE_LIBRARY}
                       ${LIBKML_REGIONATOR_LIBRARY}
                       ${LIBKML_XSD_LIBRARY}
                       ${LIBKML_MINIZIP_LIBRARY})
ENDIF(LIBKML_FOUND)
