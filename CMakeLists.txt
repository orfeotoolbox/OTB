# This is the root OTB CMakeLists file.
# $Id$
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT(OTB)

INCLUDE_REGULAR_EXPRESSION("^(otb|itk|vtk|vnl|vcl|vxl|f2c|netlib|ce|itpack|DICOM|meta|png|dbh|tif|jpeg|zlib).*$")
SOURCE_GROUP("XML Files" REGULAR_EXPRESSION "[.]xml$")

# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        IF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          ADD_DEFINITIONS(
            -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
            -D_CRT_IS_WCTYPE_NO_DEPRECATE
            -D_CRT_MANAGED_FP_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
            -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
            -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
            -D_CRT_VCCLRIT_NO_DEPRECATE
            -D_SCL_SECURE_NO_DEPRECATE
            )
        ENDIF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)
# __CYGWIN__ must be define for ossim (ex: ossimCommon.h, isnan definition, .. )
#IF(CYGWIN)
#  ADD_DEFINITIONS(-D__CYGWIN__)
#ENDIF(CYGWIN)

# If CMake is the version we're expecting, don't show the
# CMAKE_BACKWARDS_COMPATIBILITY option. If the version is higher than the
# minimal version required, then show the backward compatibility option.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^2\\.4$")
  MARK_AS_ADVANCED(FORCE CMAKE_BACKWARDS_COMPATIBILITY)
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^2\\.4$")
  MARK_AS_ADVANCED(CLEAR CMAKE_BACKWARDS_COMPATIBILITY)
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^2\\.4$")

#-----------------------------------------------------------------------------
# OTB option to access large file (> 4Go).
ADD_DEFINITIONS(
	-D_LARGEFILE_SOURCE
	-D_FILE_OFFSET_BITS=64
	-D_LARGEFILE64_SOURCE
	-D_LARGE_FILES
	)

#-----------------------------------------------------------------------------
# OTB version number.  An even minor number corresponds to releases.
SET(OTB_VERSION_MAJOR "2")
SET(OTB_VERSION_MINOR "9")
SET(OTB_VERSION_PATCH "0")


# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(OTB_VERSION_STRING "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")


# => Comment for OTB 2.6.0
OPTION(BUILD_TESTING "Build testing." OFF)

#-----------------------------------------------------------------------------
# OTB build configuration options.
IF(WIN32)
# do not build shared lib under windows
SET(BUILD_SHARED_LIBS OFF)
MESSAGE("Disabling build shared lib option on windows like OS.")
ELSE(WIN32)
OPTION(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)
ENDIF(WIN32)
SET(OTB_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#-----------------------------------------------------------------------------

# Case for release 2.2.1
# Use the openjpeg library.
OPTION(OTB_USE_JPEG2000 "Use to support jpeg2000 image file format." ON)

#-----------------------------------------------------------------------------


# FIND EXTERNAL LIBRAIRIES USE

#-------------------------------
# EXPAT Library
#-------------------------------

OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." OFF)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_EXPAT)

IF(OTB_USE_EXTERNAL_EXPAT)
    FIND_PACKAGE(EXPAT)
    IF(EXPAT_FOUND)
        # Control expat compatibility
        INCLUDE_DIRECTORIES(BEFORE ${EXPAT_INCLUDE_DIR})
        LINK_DIRECTORIES( ${EXPAT_LIBRARY} )
    ELSE(EXPAT_FOUND)
        MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without lib EXPAT.  Please set EXPAT_DIR or set OTB_USE_EXTERNAL_EXPAT OFF to use INTERNAL EXPAT set on OTB/Utilities repository.")
    ENDIF(EXPAT_FOUND)
ELSE(OTB_USE_EXTERNAL_EXPAT)
        ADD_DEFINITIONS(-DOTB_USE_INTERNAL_EXPAT)
        INCLUDE_DIRECTORIES(BEFORE "${OTB_SOURCE_DIR}/Utilities/otbexpat")
        INCLUDE_DIRECTORIES(BEFORE "${OTB_BINARY_DIR}/Utilities/otbexpat")
	IF(NOT BUILD_SHARED_LIBS)
        	ADD_DEFINITIONS(-DXML_STATIC)
        ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

#-------------------------------
# ITK Library
#-------------------------------

OPTION(OTB_USE_EXTERNAL_ITK "Use an outside build of ITK." OFF)
IF(OTB_USE_EXTERNAL_ITK)
  FIND_PACKAGE(ITK)
  IF(ITK_FOUND)
        # Builf ITK_INCLUDE_DIRS list wihout expat path
        SET(new_list "")
        FOREACH(files_h ${ITK_INCLUDE_DIRS})
                IF(NOT "${files_h}" MATCHES "expat")
                        SET(new_list "${new_list};${files_h}")
                ENDIF(NOT "${files_h}" MATCHES "expat")
        ENDFOREACH(files_h ${ITK_INCLUDE_DIRS})
        SET(ITK_INCLUDE_DIRS "${new_list}")

        INCLUDE(${ITK_USE_FILE})
  ELSE(ITK_FOUND)
        MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without ITK.  Please set ITK_DIR or set OTB_USE_EXTERNAL_ITK OFF to use INTERNAL ITK set on OTB/Utilities repository.")
  ENDIF(ITK_FOUND)
ELSE(OTB_USE_EXTERNAL_ITK)
# Required to have proper itk headers at first run
SET(ITK_SOURCE_DIR ${OTB_SOURCE_DIR}/Utilities/ITK)
SET(ITK_BINARY_DIR ${OTB_BINARY_DIR}/Utilities/ITK)
ENDIF(OTB_USE_EXTERNAL_ITK)


# JPEG2000 specification control
SET(OTB_COMPILE_JPEG2000 0)
IF( OTB_USE_JPEG2000 )
  SET(OTB_COMPILE_JPEG2000 1)
  IF(OTB_USE_EXTERNAL_ITK)
        MESSAGE(STATUS "WARNING: Due to conflicting libraries, jpeg2000 support is disabled when using external ITK.")
        SET(OTB_COMPILE_JPEG2000 0)
  ENDIF(OTB_USE_EXTERNAL_ITK)
  IF(CMAKE_GENERATOR MATCHES "^Visual Studio 7$")
        MESSAGE(STATUS "WARNING: For Visual Studio 7, jpeg2000 support is disabled.")
        SET(OTB_COMPILE_JPEG2000 0)
  ENDIF(CMAKE_GENERATOR MATCHES "^Visual Studio 7$")
  IF(CMAKE_GENERATOR MATCHES "^Visual Studio 7 .NET 2003$")
        MESSAGE(STATUS "WARNING: For Visual Studio 7, jpeg2000 support is disabled.")
        SET(OTB_COMPILE_JPEG2000 0)
  ENDIF(CMAKE_GENERATOR MATCHES "^Visual Studio 7 .NET 2003$")
  IF(CYGWIN)
        MESSAGE(STATUS "WARNING: For cygwin, jpeg2000 support is disabled.")
        SET(OTB_COMPILE_JPEG2000 0)
  ENDIF(CYGWIN)

ENDIF( OTB_USE_JPEG2000 )
# Check jpeg2000 support
IF(OTB_COMPILE_JPEG2000)
  ADD_DEFINITIONS(-DOTB_COMPILE_JPEG2000 -DUSE_OPJ_DEPRECATED)
  IF(NOT BUILD_SHARED_LIBS)
        ADD_DEFINITIONS(-DOPJ_STATIC)
  ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(OTB_COMPILE_JPEG2000)

#-----------------------------------------------------------------------------
# Option for OpenThreads (necessary because used by the new version of OSSIM library
# Find OpenThread library
IF(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")
        FIND_PACKAGE(OpenThreads)
ELSE(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")
        INCLUDE(${OTB_SOURCE_DIR}/FindOpenThreads.cmake)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")

SET(OTB_USE_EXTERNAL_OPENTHREADS 1)
IF(OPENTHREADS_FOUND)
        INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
        LINK_DIRECTORIES( ${OPENTHREADS_LIBRARY} )
ELSE(OPENTHREADS_FOUND)
        ADD_DEFINITIONS(-DOTB_USE_INTERNAL_OPENTHREADS)
        SET(OTB_USE_EXTERNAL_OPENTHREADS 0)
        SET(OPENTHREADS_INCLUDE_DIR "${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include")
        SET(OPENTHREADS_LIBRARY "OpenThreads" )
        INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
	IF(NOT BUILD_SHARED_LIBS)
         ADD_DEFINITIONS(-DOT_LIBRARY_STATIC)
        ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(OPENTHREADS_FOUND)

IF(NOT BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DOSSIM_STATIC)
ENDIF(NOT BUILD_SHARED_LIBS)


#-----------------------------------------------------------------------------
# Option for generate Visu tools !!!
OPTION(OTB_USE_VISU_GUI "Generate Gui/Visu tools directory. If ON, find OpenGL and FLTK packages." ON)
MARK_AS_ADVANCED(OTB_USE_VISU_GUI)

SET(OTB_GLU_INCLUDE_PATH "")
IF(OTB_USE_VISU_GUI)

        #-------------------------------
        # OpenGL Library
        #-------------------------------
        FIND_PACKAGE(OpenGL)

        IF( NOT OPENGL_FOUND )
                MESSAGE(FATAL_ERROR "Impossible to find OpenGL on your system.\nCannot build OTB Visu module without OpenGL library.\nInstall OpenGL on your system OR set OTB_USE_VISU to OFF to disable OTB Visu module generation !")
	ENDIF( NOT OPENGL_FOUND )
        IF( NOT OPENGL_GLU_FOUND )
                MESSAGE(FATAL_ERROR "Impossible to find GLU library on your system.\nCannot build OTB Visu module without GLU library.\nInstall GLU library on your system OR set OTB_USE_VISU to OFF to disable OTB Visu module generation !")
        ENDIF( NOT OPENGL_GLU_FOUND )

        IF(OPENGL_INCLUDE_DIR)
                INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
        ENDIF(OPENGL_INCLUDE_DIR)

	#-------------------------------
        # Display images using textures using video card acceleration
        #-------------------------------
	OPTION(OTB_GL_USE_ACCEL "Use video card acceleration." ON)
	MARK_AS_ADVANCED(OTB_GL_USE_ACCEL)

        #-------------------------------
        # FLTK Library
        #-------------------------------

        # Option for internal/external FLTK
        OPTION(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." ON)
        MARK_AS_ADVANCED(OTB_USE_EXTERNAL_FLTK)
        # Add an option to use or not use FLTK (http://www.fltk.org)
        IF(OTB_USE_EXTERNAL_FLTK)

                FIND_PACKAGE(FLTK)
                # Mingw Option doesn't exist in the FindFLTK.cmake default configuration file in CMake installationj directory.
                # Copy  FLTK_PLATFORM_DEPENDENT_LIBS from FLTK CmakeList.txt
                IF(MINGW)
                    ADD_DEFINITIONS(-DWIN32)
                    SET( FLTK_PLATFORM_DEPENDENT_LIBS ole32 uuid wsock32 gdi32 comdlg32)
                ENDIF(MINGW)
                IF (NOT FLTK_INCLUDE_DIR)
		        SET( FLTK_INCLUDE_DIR /usr/include )
                FIND_PACKAGE(FLTK)
	        ENDIF(NOT FLTK_INCLUDE_DIR)

                IF(FLTK_FOUND)
		        INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIRS})
		        LINK_DIRECTORIES(${FLTK_LIBRARY_DIRS})
                ELSE(FLTK_FOUND)
                        MESSAGE(FATAL_ERROR
	                        "Cannot build OTB project without FLTK.  Please set FLTK_DIR  or  set OTB_USE_VISU to OFF  or  set OTB_USE_EXTERNAL_FLTK OFF to use INTERNAL FLTK set on OTB/Utilities repository.")
                ENDIF(FLTK_FOUND)

        ELSE(OTB_USE_EXTERNAL_FLTK)

                SET( FLTK_PLATFORM_DEPENDENT_LIBS "")
                IF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
                        INCLUDE(${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake)
                ENDIF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
                #----------------------------------------------------------------
                # RESUME Alls VISU GUI libraries use by OTB in a single VARIABLE
                SET(OTB_VISU_GUI_LIBRARIES "${FLTK_LIBRARIES};${OPENGL_LIBRARIES} ")

        ENDIF(OTB_USE_EXTERNAL_FLTK)

ENDIF(OTB_USE_VISU_GUI)

#-------------------------------
# CURL Library
#-------------------------------
OPTION(OTB_USE_CURL "Use curl library." OFF)
MARK_AS_ADVANCED(OTB_USE_CURL)
IF(OTB_USE_CURL)

        FIND_PATH(CURL_INCLUDE_DIR curl/curl.h PATHS)
        MARK_AS_ADVANCED(CURL_INCLUDE_DIR)
        IF (NOT CURL_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find CURL include directory. Please set CURL_INCLUDE_DIR or SET OTB_USE_CURL OFF.")
        ENDIF (NOT CURL_INCLUDE_DIR)

        FIND_LIBRARY(CURL_LIBRARY curl )
        MARK_AS_ADVANCED(CURL_LIBRARY)
        IF (NOT CURL_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find CURL library. Please set CURL_LIBRARY or SET OTB_USE_CURL OFF.")
        ENDIF (NOT CURL_LIBRARY)

        # Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_CURL)

        INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

ENDIF(OTB_USE_CURL)

#-------------------------------
# GDAL Library
#-------------------------------

FIND_PATH(GDAL_INCLUDE_DIRS gdal.h $ENV{GDAL_INCLUDE_DIRS} /usr/include/gdal)

IF (NOT GDAL_INCLUDE_DIRS)
MESSAGE(FATAL_ERROR
         "Cannot find GDAL include directory. Please set GDAL_INCLUDE_DIRS.")
ENDIF (NOT GDAL_INCLUDE_DIRS)

INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIRS})

FIND_LIBRARY(GDAL_LIBRARY NAMES gdal gdal1.6.0 gdal1.5.0 gdal1.4.0 gdal1.3.2 PATHS /usr/lib/gdal)

IF (NOT GDAL_LIBRARY)
MESSAGE(FATAL_ERROR
         "Cannot find GDAL library. Please set GDAL_LIBRARY.")
ENDIF (NOT GDAL_LIBRARY)
MESSAGE(STATUS "GDAL library found: ${GDAL_LIBRARY}")

# Find geotiff headers
FIND_PATH(GEOTIFF_INCLUDE_DIRS geotiff.h $ENV{GDAL_INCLUDE_DIRS} ${GDAL_INCLUDE_DIRS} /usr/include/geotiff)
IF (NOT GEOTIFF_INCLUDE_DIRS)
MESSAGE(FATAL_ERROR
         "Cannot find geotiff headers. Please set GEOTIFF_INCLUDE_DIRS.")
ENDIF (NOT GEOTIFF_INCLUDE_DIRS)
INCLUDE_DIRECTORIES(${GEOTIFF_INCLUDE_DIRS})

# Find tiff headers
FIND_PATH(TIFF_INCLUDE_DIRS tiffio.h $ENV{GDAL_INCLUDE_DIRS} ${GDAL_INCLUDE_DIRS} /usr/include)
IF (NOT TIFF_INCLUDE_DIRS)
MESSAGE(FATAL_ERROR
         "Cannot find libtiff headers. Please set TIFF_INCLUDE_DIRS.")
ENDIF (NOT TIFF_INCLUDE_DIRS)
INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

# Find jpeg headers
FIND_PATH(JPEG_INCLUDE_DIRS jpeglib.h $ENV{GDAL_INCLUDE_DIRS} ${GDAL_INCLUDE_DIRS} /usr/include)
IF (NOT JPEG_INCLUDE_DIRS)
MESSAGE(FATAL_ERROR
         "Cannot find jpeg headers. Please set JPEG_INCLUDE_DIRS.")
ENDIF (NOT JPEG_INCLUDE_DIRS)
INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})

# Find ogr headers
FIND_PATH(OGR_INCLUDE_DIRS ogrsf_frmts.h  $ENV{GDAL_INCLUDE_DIRS} ${GDAL_INCLUDE_DIRS} /usr/include)
IF (NOT OGR_INCLUDE_DIRS)
MESSAGE(FATAL_ERROR
         "Cannot find ogr headers. Please set OGR_INCLUDE_DIRS.")
ENDIF (NOT OGR_INCLUDE_DIRS)
INCLUDE_DIRECTORIES(${OGR_INCLUDE_DIRS})

# Check if ${GDAL_LIBRARY} has tiff library
TRY_COMPILE(GDAL_HAS_TIFF
${CMAKE_CURRENT_BINARY_DIR}CMake
${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasTiff.cxx
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
OUTPUT_VARIABLE OUTPUT)
IF(GDAL_HAS_TIFF)
MESSAGE(STATUS "Testing if GDAL has tiff    -- yes")
SET(TIFF_LIBRARY ${GDAL_LIBRARY})
ELSE(GDAL_HAS_TIFF)
MESSAGE(STATUS "Testing if GDAL has tiff    -- no")
FIND_LIBRARY(TIFF_LIBRARY tiff PATHS)
IF (NOT TIFF_LIBRARY)
MESSAGE(FATAL_ERROR
         "Cannot find tiff library. Please set TIFF_LIBRARY.")
ENDIF (NOT TIFF_LIBRARY)
ENDIF(GDAL_HAS_TIFF)

# Check if ${GDAL_LIBRARY} has geotiff library
TRY_COMPILE(GDAL_HAS_GEOTIFF
${CMAKE_CURRENT_BINARY_DIR}/CMake
${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasGeoTiff.cxx
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GEOTIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
OUTPUT_VARIABLE OUTPUT)
IF(GDAL_HAS_GEOTIFF)
MESSAGE(STATUS "Testing if GDAL has geotiff -- yes")
SET(GEOTIFF_LIBRARY ${GDAL_LIBRARY})
ELSE(GDAL_HAS_GEOTIFF)
MESSAGE(STATUS "Testing if GDAL has geotiff -- no")
FIND_LIBRARY(GEOTIFF_LIBRARY geotiff PATHS)
IF (NOT GEOTIFF_LIBRARY)
MESSAGE(FATAL_ERROR
         "Cannot find geotiff library. Please set GEOTIFF_LIBRARY.")
ENDIF (NOT GEOTIFF_LIBRARY)
ENDIF(GDAL_HAS_GEOTIFF)

# Check if ${GDAL_LIBRARY} has jpeg library
TRY_COMPILE(GDAL_HAS_JPEG
${CMAKE_CURRENT_BINARY_DIR}/CMake
${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasJpeg.cxx
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${JPEG_INCLUDE_DIRS};${GDAL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
OUTPUT_VARIABLE OUTPUT)
IF(GDAL_HAS_JPEG)
MESSAGE(STATUS "Testing if GDAL has jpeg    -- yes")
SET(JPEG_LIBRARY ${GDAL_LIBRARY})
ELSE(GDAL_HAS_JPEG)
MESSAGE(STATUS "Testing if GDAL has jpeg    -- no")
FIND_LIBRARY(JPEG_LIBRARY jpeg PATHS)
IF (NOT JPEG_LIBRARY)
MESSAGE(FATAL_ERROR
         "Cannot find jpeg library. Please set JPEG_LIBRARY.")
ENDIF (NOT JPEG_LIBRARY)
ENDIF(GDAL_HAS_JPEG)

# Check if ${GDAL_LIBRARY} has jpeg library
SET(OGR_LIBRARY ${GDAL_LIBRARY})
TRY_COMPILE(GDAL_HAS_OGR
${CMAKE_CURRENT_BINARY_DIR}/CMake
${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasOGR.cxx
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${OGR_INCLUDE_DIRS};${GDAL_INCLUDE_DIRS}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
OUTPUT_VARIABLE OUTPUT)
IF(GDAL_HAS_OGR)
MESSAGE(STATUS "Testing if GDAL has OGR     -- yes")
ELSE(GDAL_HAS_OGR)
MESSAGE(STATUS "Testing if GDAL has OGR     -- no")
MESSAGE(${OUTPUT})
FIND_LIBRARY(OGR_LIBRARY ogr PATHS)
IF (NOT OGR_LIBRARY)
MESSAGE(FATAL_ERROR
         "Cannot find jpeg library. Please set OGR_LIBRARY.")
ENDIF (NOT OGR_LIBRARY)
ENDIF(GDAL_HAS_OGR)


#-------------------------------
# End connections whith external libraries
#-------------------------------

# The fluid-generated fltk sources have many warnings.  This macro
# will disable warnings for the generated files on some compilers.
MACRO(OTB_DISABLE_FLTK_GENERATED_WARNINGS files)
  IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
    IF(CMAKE_COMPILER_IS_GNUCXX)
      FOREACH(f ${files})
        STRING(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
        STRING(REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}")
        SET_SOURCE_FILES_PROPERTIES(${SRC} PROPERTIES COMPILE_FLAGS -w)
      ENDFOREACH(f)
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
ENDMACRO(OTB_DISABLE_FLTK_GENERATED_WARNINGS)

# Macro for adding resources to GUI application on the Mac
#
IF(APPLE AND NOT FLTK_USE_X)
    FIND_PROGRAM(ITK_APPLE_RESOURCE Rez /Developer/Tools)
    FIND_FILE(ITK_FLTK_RESOURCE mac.r /usr/local/include/FL)
    IF(NOT ITK_FLTK_RESOURCE)
      MESSAGE("Fltk resources not found, GUI application will not respond to mouse events")
    ENDIF(NOT ITK_FLTK_RESOURCE)

    MACRO(ADD_GUI_EXECUTABLE name sources)
      ADD_EXECUTABLE(${name} ${sources})
      INSTALL_TARGETS(/bin ${name})
      SET(EXEC_PATH ${EXECUTABLE_OUTPUT_PATH})
      IF(NOT EXEC_PATH)
        SET(EXEC_PATH ${CMAKE_CURRENT_BINARY_DIR})
      ENDIF(NOT EXEC_PATH)
        IF(ITK_APPLE_RESOURCE)
          ADD_CUSTOM_COMMAND(SOURCE ${name}
                             COMMAND ${ITK_APPLE_RESOURCE}
                             ARGS -t APPL ${ITK_FLTK_RESOURCE} -o
                             ${EXEC_PATH}/${name}
                             TARGET ${name})
        ENDIF(ITK_APPLE_RESOURCE)
    ENDMACRO(ADD_GUI_EXECUTABLE)
ELSE(APPLE AND NOT FLTK_USE_X)
  MACRO(ADD_GUI_EXECUTABLE name sources)
    ADD_EXECUTABLE(${name} ${sources})
    INSTALL_TARGETS(/bin ${name})
  ENDMACRO(ADD_GUI_EXECUTABLE)
ENDIF(APPLE AND NOT FLTK_USE_X)


INCLUDE_DIRECTORIES (
  ${OTB_BINARY_DIR}
  ${OTB_SOURCE_DIR}
)

INCLUDE_DIRECTORIES( ${OTB_INCLUDE_DIRS} )


#-----------------------------------------------------------------------------
# Test for some required system information.
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)

#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Output directories.


IF(NOT LIBRARY_OUTPUT_PATH)
   SET (LIBRARY_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
ENDIF(NOT LIBRARY_OUTPUT_PATH)
IF(NOT EXECUTABLE_OUTPUT_PATH)
   SET (EXECUTABLE_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)

IF(NOT OTB_NO_LIBRARY_VERSION)
   # This setting of SOVERSION assumes that any API change
   # will increment either the minor or major version number of OTB.
   SET(OTB_LIBRARY_PROPERTIES
       VERSION   "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}"
       SOVERSION "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
ENDIF(NOT OTB_NO_LIBRARY_VERSION)




MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

SET(OTB_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(OTB_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

INCLUDE_DIRECTORIES( ${OTB_LIBRARY_PATH} )

# Put "." in the rpath of all OTB shared libraries.
LINK_DIRECTORIES(.)

#-----------------------------------------------------------------------------
# Option for display all developpment message !!!
OPTION(OTB_SHOW_ALL_MSG_DEBUG "OTB developers : Show all messages (debug, dev, etc...) use only for otb developers." OFF)
MARK_AS_ADVANCED(OTB_SHOW_ALL_MSG_DEBUG)
# IF( OTB_SHOW_ALL_MSG_DEBUG )
#  ADD_DEFINITIONS(-DOTB_SHOW_ALL_MSG_DEBUG)
# ENDIF( OTB_SHOW_ALL_MSG_DEBUG )

#-----------------------------------------------------------------------------
# Advanced option to avoid recompiling utilities at each cmake reconfiguration
OPTION(OTB_DISABLE_UTILITIES_COMPILATION "Disable utilities compilation (developpers only - use at you own risk)" OFF)
MARK_AS_ADVANCED(OTB_DISABLE_UTILITIES_COMPILATION)

IF(OTB_DISABLE_UTILITIES_COMPILATION)
MESSAGE("Warning: utilities compilation is disabled.")
LINK_DIRECTORIES("${OTB_BINARY_DIR}/bin")
ENDIF(OTB_DISABLE_UTILITIES_COMPILATION)


#-----------------------------------------------------------------------------
# Option to define streaming activation in applications
# Use by otbConfigure.h.in
SET(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING 8*4000*4000 CACHE STRING "Image size to activate using streaming for applications.")
MARK_AS_ADVANCED(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING)
SET(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING 8*4000*4000 CACHE STRING "Max size buffer for streaming.")
MARK_AS_ADVANCED(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING)



#-----------------------------------------------------------------------------
# Perform a setup of OTB Wrapping. This will provide CMake options for
# individual wrapping as well as determine if CableSwig is required. If it is,
# OTB_NEED_CableSwig will be set. This file also tries to locate CableSwig by
# searching first in the source tree of OTB, and if that fails, it searches
# for a binary built of CableSwig.
#
INCLUDE(${OTB_SOURCE_DIR}/Wrapping/otbWrapSetup.cmake)


#-----------------------------------------------------------------------------
# Option for generate Patented examples  !!!
OPTION(OTB_USE_PATENTED "Build patented examples. ITK must be genereted whith patented option (ITK_USE_PATENTED = ON)." OFF)
MARK_AS_ADVANCED(OTB_USE_PATENTED)

IF(OTB_USE_VISU_GUI)
    SET(OTB_VISU_GUI_LIBRARIES "${FLTK_LIBRARIES};${OPENGL_LIBRARIES};${FLTK_PLATFORM_DEPENDENT_LIBS}")
        IF(APPLE)
            SET(OTB_VISU_GUI_LIBRARIES "${OTB_VISU_GUI_LIBRARIES};-Wl,-dylib_file,/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib")
        ENDIF(APPLE)
ELSE(OTB_USE_VISU_GUI)
        SET(OTB_VISU_GUI_LIBRARIES "")
ENDIF(OTB_USE_VISU_GUI)


#-----------------------------------------------------------------------------
# List of alls basics OTB libraries used for any applications
# Use OTB_IO_UTILITIES_DEPENDENT_LIBRARIES to LINK IO libraries into your executable
# Use OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES to LINK Utilities Algo libraries into your executable
# Use OTB_ALLS_UTILITIES_DEPENDENT_LIBRARIES to LINK Utilities Algo libraries into your executable
# => Create OTB_IO_UTILITIES_DEPENDENT_LIBRARIES

SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES "${GDAL_LIBRARY};dxf;tinyXML;otbliblas;otbossim;otbossimplugins" )

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};${OPENTHREADS_LIBRARY}")
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

# Add EXPAT library
IF( OTB_USE_EXTERNAL_EXPAT )
        SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};expat")
ELSE( OTB_USE_EXTERNAL_EXPAT  )
        SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};otbexpat")
ENDIF( OTB_USE_EXTERNAL_EXPAT )

# Add Open Jpeg library
IF( OTB_COMPILE_JPEG2000 )
     SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};otbopenjpeg")
ENDIF( OTB_COMPILE_JPEG2000 )

# => Create OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES
SET(OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES "otb6S;otbgalib;InsightJournal;otbsvm")


# => Create OTB_ALLS_UTILITIES_DEPENDENT_LIBRARIES
SET(OTB_ALLS_UTILITIES_DEPENDENT_LIBRARIES "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES} ${OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES}")


#-----------------------------------------------------------------------------
OPTION(BUILD_EXAMPLES "Build the Examples directory." OFF)


#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.

SUBDIRS(Utilities Code)

SUBDIRS(Wrapping)

IF (BUILD_EXAMPLES)
  SUBDIRS(Examples)
ENDIF (BUILD_EXAMPLES)

IF (BUILD_TESTING)
  SUBDIRS(Testing)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# => Comment for OTB 2.6.0: simplification : copy ITK values
# OTB requires special compiler flags on some platforms.
SET(OTB_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS}")
SET(OTB_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS}")
SET(OTB_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS}")

IF(MINGW)
  LINK_LIBRARIES(gdi32)
ENDIF(MINGW)

#-----------------------------------------------------------------------------
# Test for an Objective-C++ compiler.
SET(OTB_OBJCXX_COMPILER_WORKS 0)
IF(APPLE)
  TRY_COMPILE(OTB_OBJCXX_COMPILER_WORKS
              ${OTB_BINARY_DIR}/CMakeTmp
              ${OTB_SOURCE_DIR}/CMake/otbTestObjCxxCompiler.mm
              OUTPUT_VARIABLE OTB_OBJCXX_TRY_COMPILE_OUTPUT)
  IF( OTB_OBJCXX_COMPILER_WORKS )
    FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Determining if the Objective-C compiler works passed with "
        "the following output:\n${OTB_OBJCXX_TRY_COMPILE_OUTPUT}\n\n")
  ELSE( OTB_OBJCXX_COMPILER_WORKS )
    FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
      "Determining if the Objective-C compiler works failed with "
      "the following output:\n${OTB_OBJCXX_TRY_COMPILE_OUTPUT}\n\n")
  ENDIF( OTB_OBJCXX_COMPILER_WORKS )
ENDIF(APPLE)


INCLUDE(CheckTypeSize)

CHECK_TYPE_SIZE("long long" OTB_SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("__int64"   OTB_SIZEOF___INT64)
CHECK_TYPE_SIZE("long int" OTB_SIZEOF_LONG_INT)
CHECK_TYPE_SIZE("short int" OTB_SIZEOF_SHORT_INT)

#-----------------------------------------------------------------------------
# Configure the default OTB_DATA_ROOT for the location of OTB Data.
FIND_PATH(OTB_DATA_ROOT OTBData.readme $ENV{OTB_DATA_ROOT})
MARK_AS_ADVANCED(OTB_DATA_ROOT)

OPTION(OTB_DATA_USE_LARGEINPUT "Use Large inputs images test." OFF)
MARK_AS_ADVANCED(OTB_DATA_USE_LARGEINPUT)
IF(OTB_DATA_USE_LARGEINPUT)
  FIND_PATH(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
  MARK_AS_ADVANCED(OTB_DATA_LARGEINPUT_ROOT)
ENDIF(OTB_DATA_USE_LARGEINPUT)


#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/otbConfigure.h.in
               ${OTB_BINARY_DIR}/otbConfigure.h)

#-----------------------------------------------------------------------------
# The entire OTB tree should use the same include path, except for the
# Wrapping directory.

# Create the list of include directories needed for OTB header files.
INCLUDE(${OTB_SOURCE_DIR}/otbIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${OTB_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Uninstall cmake use to uninstall OTB.
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Help other projects use OTB.

# Copy the UseOTB.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/UseOTB.cmake.in
               ${OTB_BINARY_DIR}/UseOTB.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${OTB_BINARY_DIR}/OTBBuildSettings.cmake)
INSTALL_FILES(/lib/otb .cmake OTBBuildSettings)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# Create the OTBConfig.cmake file containing the OTB configuration.
INCLUDE (${OTB_SOURCE_DIR}/otbGenerateOTBConfig.cmake)

# Install some files.
INSTALL_FILES(/include/otb .h otbConfigure)
INSTALL_FILES(/lib/otb .cmake UseOTB OTBLibraryDepends OTBConfig )

#-----------------------------------------------------------------------------
# Add compiler flags OTB needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")

#-----------------------------------------------------------------------------
# Set up CPack support, to be able to distribute OTB binary packages. It will
# be particulary useful to distributed prebuilt OTB.
#

#INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_NAME "OTB" CACHE STRING "Package name")
MARK_AS_ADVANCED(CPACK_PACKAGE_NAME)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Orfeo Toolbox")
MARK_AS_ADVANCED(CPACK_PACKAGE_DESCRIPTION_SUMMARY)

SET(CPACK_PACKAGE_VERSION  "${OTB_VERSION_STRING}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${OTB_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${OTB_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${OTB_VERSION_PATCH}")

SET(CPACK_PACKAGE_CONTACT "debian@orfeo-toolbox.org" CACHE STRING  "Debian package maintainer email")
MARK_AS_ADVANCED(CPACK_PACKAGE_CONTACT)

SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Description.txt")

SET(CPACK_DEBIAN_PACKAGE_DEPENDS "lib${GDAL_LIBRARY}1-1.5.0 (>= 1.5.1-0), libfltk1.1 (>= 1.1.7-3), libcurl3 (>=7.15.5-1etch1), libfftw3-3 (>=3.1.2-3.1)" CACHE STRING "Debian package dependance" )
MARK_AS_ADVANCED(CPACK_DEBIAN_PACKAGE_DEPENDS)

SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OrfeoToolbox-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright/OTBCopyright.txt")

FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_RPM_PACKAGE_DESCRIPTION)
FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)

INCLUDE(CPack)

