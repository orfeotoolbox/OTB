# Locate the glfw library
# This module defines the following variables:
# GLFW_LIBRARY, the name of the library;
# GLFW_INCLUDE_DIR, where to find glfw include files.
# GLFW_FOUND, true if both the GLFW_LIBRARY and GLFW_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you could define an environment variable called
# GLFW_ROOT which points to the root of the glfw library installation. This is pretty useful
# on a Windows platform.
#
#
# Usage example to compile an "executable" target to the glfw library:
#
# FIND_PACKAGE (glfw REQUIRED)
# INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIR})
# ADD_EXECUTABLE (executable ${EXECUTABLE_SRCS})
# TARGET_LINK_LIBRARIES (executable ${GLFW_LIBRARY})
#
# TODO:
# Allow the user to select to link to a shared library or to a static library.

#Search for the include file...
FIND_PATH(GLFW_INCLUDE_DIR GLFW/glfw3.h DOC "Path to GLFW include directory."
    HINTS
      $ENV{GLFW_ROOT}
    PATH_SUFFIX
      include # For finding the include file under the root of the glfw expanded archive, typically on Windows.
)

FIND_LIBRARY(GLFW_LIBRARY DOC "Absolute path to GLFW library."
    NAMES glfw glfw3dll glfw3
    HINTS
      $ENV{GLFW_ROOT}
    PATH_SUFFIXES
       lib # For finding the library file under the root of the glfw expanded archive, typically on Windows.
)


IF(GLFW_INCLUDE_DIR AND EXISTS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h")
    FILE(STRINGS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h" glfw_version_str
         REGEX "^#[\t ]*define[\t ]+GLFW_VERSION_(MAJOR|MINOR|REVISION)[\t ]+[0-9]+$")

    UNSET(GLFW_VERSION_STRING)
    FOREACH(VPART MAJOR MINOR REVISION)
        FOREACH(VLINE ${glfw_version_str})
            IF(VLINE MATCHES "^#[\t ]*define[\t ]+GLFW_VERSION_${VPART}")
                STRING(REGEX REPLACE "^#[\t ]*define[\t ]+GLFW_VERSION_${VPART}[\t ]+([0-9]+)$" "\\1"
                       GLFW_VERSION_PART "${VLINE}")
                IF(GLFW_VERSION_STRING)
                    SET(GLFW_VERSION_STRING "${GLFW_VERSION_STRING}.${GLFW_VERSION_PART}")
                ELSE()
                    SET(GLFW_VERSION_STRING "${GLFW_VERSION_PART}")
                ENDIF()
                UNSET(GLFW_VERSION_PART)
            ENDIF()
        ENDFOREACH()
    ENDFOREACH()
ENDIF()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW
                                  REQUIRED_VARS GLFW_LIBRARY GLFW_INCLUDE_DIR
                                  VERSION_VAR GLFW_VERSION_STRING)