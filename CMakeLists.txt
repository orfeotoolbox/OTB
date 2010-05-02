# This is the root OTB CMakeLists file.
# $Id$
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT(OTB)

# Path to additional CMake modules
SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

INCLUDE_REGULAR_EXPRESSION("^(otb|itk|vtk|vnl|vcl|vxl|f2c|netlib|ce|itpack|DICOM|meta|png|dbh|tif|jpeg|zlib).*$")
SOURCE_GROUP("XML Files" REGULAR_EXPRESSION "[.]xml$")
INCLUDE(CMakeDependentOption)

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
SET(OTB_VERSION_MAJOR "3")
SET(OTB_VERSION_MINOR "2")
SET(OTB_VERSION_PATCH "1")


# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(OTB_VERSION_STRING "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")

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

IF(NOT OTB_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of OTB.
  SET(OTB_LIBRARY_PROPERTIES
      VERSION   "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}"
      SOVERSION "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}"
     )
ENDIF(NOT OTB_NO_LIBRARY_VERSION)

SET(OTB_BUILD_PEDANTIC OFF CACHE BOOL
    "Choose compilation in pedantic or relaxed mode")

IF(WIN32)
  IF (MSVC)
    IF(OTB_BUILD_PEDANTIC)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    ENDIF(OTB_BUILD_PEDANTIC)

    IF (MSVC80)
      ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
      ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNING)
    ENDIF(MSVC80)
  ENDIF(MSVC)
ELSE(WIN32)
  IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    IF(OTB_BUILD_PEDANTIC)
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    ENDIF(OTB_BUILD_PEDANTIC)
  ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
ENDIF(WIN32)
#-----------------------------------------------------------------------------

# Use the openjpeg library.
OPTION(OTB_USE_JPEG2000 "Use to support jpeg2000 image file format." ON)

#-----------------------------------------------------------------------------

# Use the liblas library (do not link on solaris)
OPTION(OTB_USE_LIBLAS "Use liblas library to support Lidar data format." ON)
MARK_AS_ADVANCED(OTB_USE_LIBLAS)

# FIND EXTERNAL LIBRAIRIES USE

#-------------------------------
# EXPAT Library
#-------------------------------

FIND_PACKAGE(EXPAT)
IF(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." ON)
ELSE(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." OFF)
ENDIF(EXPAT_FOUND)

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
        INCLUDE(${OTB_SOURCE_DIR}/CMake/FindOpenThreads.cmake)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/FindOpenThreads.cmake")

SET(OTB_USE_EXTERNAL_OPENTHREADS ON CACHE INTERNAL "")
IF(OPENTHREADS_FOUND)
        INCLUDE_DIRECTORIES(${OPENTHREADS_INCLUDE_DIR})
        LINK_DIRECTORIES( ${OPENTHREADS_LIBRARY} )
ELSE(OPENTHREADS_FOUND)
        ADD_DEFINITIONS(-DOTB_USE_INTERNAL_OPENTHREADS)
        SET(OTB_USE_EXTERNAL_OPENTHREADS OFF)
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
                MESSAGE(FATAL_ERROR "Impossible to find OpenGL on your system.\nCannot build OTB Visu module without OpenGL library.\nInstall OpenGL on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation !")
	    ENDIF( NOT OPENGL_FOUND )
        IF( NOT OPENGL_GLU_FOUND )
                MESSAGE(FATAL_ERROR "Impossible to find GLU library on your system.\nCannot build OTB Visu module without GLU library.\nInstall GLU library on your system OR set OTB_USE_VISU_GUI to OFF to disable OTB Visu module generation !")
        ENDIF( NOT OPENGL_GLU_FOUND )

        IF(OPENGL_INCLUDE_DIR)
                INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
        ENDIF(OPENGL_INCLUDE_DIR)

	#-------------------------------
        # Display images using textures using video card acceleration
        #-------------------------------
	OPTION(OTB_GL_USE_ACCEL "Use video card acceleration." OFF)
	MARK_AS_ADVANCED(OTB_GL_USE_ACCEL)

        #-------------------------------
        # FLTK Library
        #-------------------------------
        
        #OPTION(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." OFF)
        
        FIND_PACKAGE(FLTK)

	# If an external fltk is found, do not provide the internal fltk choice
	IF(FLTK_FOUND)
       OPTION(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." ON)
	ELSE(FLTK_FOUND)
	   OPTION(OTB_USE_EXTERNAL_FLTK "Use an outside build of FLTK." OFF)
	ENDIF(FLTK_FOUND)
                         
#        IF(FLTK_FOUND)
#           SET(OTB_USE_EXTERNAL_FLTK ON)
#        ELSE(FLTK_FOUND)
#           SET(OTB_USE_EXTERNAL_FLTK OFF)
#        ENDIF(FLTK_FOUND)

        # Option for internal/external FLTK
        MARK_AS_ADVANCED(OTB_USE_EXTERNAL_FLTK)

        IF(OTB_USE_EXTERNAL_FLTK)

                # Mingw Option doesn't exist in the FindFLTK.cmake default configuration file in CMake installationj directory.
                # Copy  FLTK_PLATFORM_DEPENDENT_LIBS from FLTK CmakeList.txt
                IF(MINGW)
                    ADD_DEFINITIONS(-DWIN32)
                    SET( FLTK_PLATFORM_DEPENDENT_LIBS ole32 uuid wsock32 gdi32 comdlg32)
                ENDIF(MINGW)

                IF(FLTK_FOUND)
                    INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
                ELSE(FLTK_FOUND)
                    MESSAGE(FATAL_ERROR
	                        "Cannot build OTB project without FLTK.  Please set FLTK_DIR  or  set OTB_USE_VISU to OFF  or  set OTB_USE_EXTERNAL_FLTK OFF to use INTERNAL FLTK set on OTB/Utilities repository.")
                ENDIF(FLTK_FOUND)

        ELSE(OTB_USE_EXTERNAL_FLTK)
                SET(FLTK_INCLUDE_DIR "")
                SET(FLTK_LIBRARIES "")
                
                SET( FLTK_PLATFORM_DEPENDENT_LIBS "")
                INCLUDE(${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake)

        ENDIF(OTB_USE_EXTERNAL_FLTK)

        SET(OTB_VISU_GUI_LIBRARIES "${FLTK_LIBRARIES};${OPENGL_LIBRARIES} ")
        SET(FLTK_FLUID_EXECUTABLE ${FLUID_COMMAND})
ENDIF(OTB_USE_VISU_GUI)

#-------------------------------
# CURL Library
#-------------------------------
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." ON)
ELSE(CURL_FOUND)
  OPTION(OTB_USE_CURL "Use curl library." OFF)
ENDIF(CURL_FOUND)

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

        # TODO add test to check if curl multi is available
        TRY_COMPILE(OTB_CURL_MULTI_AVAILABLE
                    ${CMAKE_CURRENT_BINARY_DIR}/CMake
                    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestCurlMulti.cxx
                    CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${CURL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${CURL_LIBRARY}"
                    OUTPUT_VARIABLE OUTPUT)
        IF (OTB_CURL_MULTI_AVAILABLE)
          ADD_DEFINITIONS(-DOTB_CURL_MULTI_AVAILABLE)
        ENDIF (OTB_CURL_MULTI_AVAILABLE)
ENDIF(OTB_USE_CURL)

#-------------------------------
# Mapnik Library
#-------------------------------
OPTION(OTB_USE_MAPNIK "Use mapnik library." OFF)
MARK_AS_ADVANCED(OTB_USE_MAPNIK)
IF(OTB_USE_MAPNIK)

        FIND_PATH(MAPNIK_INCLUDE_DIR mapnik/map.hpp PATHS)
        MARK_AS_ADVANCED(MAPNIK_INCLUDE_DIR)
        IF (NOT MAPNIK_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK include directory. Please set MAPNIK_INCLUDE_DIR or SET OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_INCLUDE_DIR)

        FIND_PATH(FREETYPE2_INCLUDE_DIR freetype/freetype.h PATHS /usr/include/freetype2)
        MARK_AS_ADVANCED(FREETYPE2_INCLUDE_DIR)
        IF (NOT FREETYPE2_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find FREETYPE2 include directory. Please set  or SET OTB_USE_MAPNIK OFF.")
        ENDIF (NOT FREETYPE2_INCLUDE_DIR)

        FIND_LIBRARY(MAPNIK_LIBRARY mapnik )
        MARK_AS_ADVANCED(MAPNIK_LIBRARY)
        IF (NOT MAPNIK_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find MAPNIK library. Please set MAPNIK_LIBRARY or SET OTB_USE_MAPNIK OFF.")
        ENDIF (NOT MAPNIK_LIBRARY)

        # Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_MAPNIK)

        INCLUDE_DIRECTORIES(${MAPNIK_INCLUDE_DIR})


ENDIF(OTB_USE_MAPNIK)

#-------------------------------
# Pqxx Library
#-------------------------------
OPTION(OTB_USE_PQXX "Use pqxx library (EXPERIMENTAL)." OFF)
MARK_AS_ADVANCED(OTB_USE_PQXX)
IF(OTB_USE_PQXX)

        FIND_PATH(PQXX_INCLUDE_DIR pqxx/pqxx PATHS)
        MARK_AS_ADVANCED(PQXX_INCLUDE_DIR)
        IF (NOT PQXX_INCLUDE_DIR)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQXX include directory. Please set PQXX_INCLUDE_DIR or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQXX_INCLUDE_DIR)


        FIND_LIBRARY(PQXX_LIBRARY pqxx )
        MARK_AS_ADVANCED(PQXX_LIBRARY)
        IF (NOT PQXX_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQXX library. Please set PQXX_LIBRARY or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQXX_LIBRARY)

        FIND_LIBRARY(PQ_LIBRARY pq )
        MARK_AS_ADVANCED(PQ_LIBRARY)
        IF (NOT PQ_LIBRARY)
                MESSAGE(FATAL_ERROR
                        "Cannot find PQ library. Please set PQ_LIBRARY or SET OTB_USE_PQXX OFF.")
        ENDIF (NOT PQ_LIBRARY)


# Add compiler option
        ADD_DEFINITIONS(-DOTB_USE_PQXX)

        INCLUDE_DIRECTORIES(${PQXX_INCLUDE_DIR})

ENDIF(OTB_USE_PQXX)


#-------------------------------
# Boost Library
#-------------------------------
FIND_PACKAGE(Boost)
IF(Boost_FOUND)
  OPTION(OTB_USE_EXTERNAL_BOOST "Use an outside build of Boost." ON)
ELSE(Boost_FOUND)
  OPTION(OTB_USE_EXTERNAL_BOOST "Use an outside build of Boost." OFF)
ENDIF(Boost_FOUND)
# Option for internal/external BOOST
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_BOOST)

IF (OTB_USE_MAPNIK)
  IF(NOT OTB_USE_EXTERNAL_BOOST)
    MESSAGE("You need to set OTB_USE_EXTERNAL_BOOST to ON when using MAPNIK")
  ENDIF(NOT OTB_USE_EXTERNAL_BOOST)
ENDIF(OTB_USE_MAPNIK)

IF(OTB_USE_EXTERNAL_BOOST)
  FIND_PACKAGE(Boost)

  TRY_COMPILE(BOOST_IS_COMPLETE
  ${CMAKE_CURRENT_BINARY_DIR}/CMake
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otbTestCompileBoost.cxx
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${Boost_INCLUDE_DIR}"
  OUTPUT_VARIABLE OUTPUT)
  IF(BOOST_IS_COMPLETE)
    MESSAGE(STATUS "Testing external Boost library    -- yes")
  ELSE(BOOST_IS_COMPLETE)
    MESSAGE(STATUS "Testing external Boost library    -- no")
    MESSAGE(STATUS "Force the OTB_USE_EXTERNAL_BOOST value to OFF.")
    SET(OTB_USE_EXTERNAL_BOOST OFF CACHE BOOL  "Use an outside build of Boost." FORCE )
  ENDIF(BOOST_IS_COMPLETE)

  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF(OTB_USE_EXTERNAL_BOOST)

#-------------------------------
# GDAL Library
#-------------------------------

# Option for internal/external GDAL
OPTION(OTB_USE_EXTERNAL_GDAL "Use an outside build of GDAL. (OFF is EXPERIMENTAL)" ON)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_GDAL)

IF(OTB_USE_EXTERNAL_GDAL)
  FIND_PACKAGE(GDAL)

  IF (NOT GDAL_FOUND)
    FIND_PATH(GDAL_INCLUDE_DIR gdal.h $ENV{GDAL_INCLUDE_DIR} /usr/include/gdal)
    FIND_LIBRARY(GDAL_LIBRARY NAMES gdal gdal1.5.0 gdal1.4.0 gdal1.3.2 PATHS /usr/lib/gdal)
  ENDIF(NOT GDAL_FOUND)

  IF (NOT GDAL_INCLUDE_DIR)
  MESSAGE(FATAL_ERROR
           "Cannot find GDAL include directory. Please set GDAL_INCLUDE_DIR.")
  ENDIF (NOT GDAL_INCLUDE_DIR)

  INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})

  IF (NOT GDAL_LIBRARY)
  MESSAGE(FATAL_ERROR
           "Cannot find GDAL library. Please set GDAL_LIBRARY.")
  ENDIF (NOT GDAL_LIBRARY)
  MESSAGE(STATUS "GDAL library found: ${GDAL_LIBRARY}")

  # Find geotiff headers
  FIND_PATH(GEOTIFF_INCLUDE_DIRS geotiff.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include/geotiff)
  IF (NOT GEOTIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find geotiff headers. Please set GEOTIFF_INCLUDE_DIRS.")
  ENDIF (NOT GEOTIFF_INCLUDE_DIRS)
  INCLUDE_DIRECTORIES(${GEOTIFF_INCLUDE_DIRS})

  # Find tiff headers
  FIND_PATH(TIFF_INCLUDE_DIRS tiffio.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include)
  IF (NOT TIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find libtiff headers. Please set TIFF_INCLUDE_DIRS.")
  ENDIF (NOT TIFF_INCLUDE_DIRS)
  INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

  # Find jpeg headers
  FIND_PATH(JPEG_INCLUDE_DIRS jpeglib.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include)
  IF (NOT JPEG_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find jpeg headers. Please set JPEG_INCLUDE_DIRS.")
  ENDIF (NOT JPEG_INCLUDE_DIRS)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})

  # Find ogr headers
  FIND_PATH(OGR_INCLUDE_DIRS ogrsf_frmts.h  $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include)
  IF (NOT OGR_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find ogr headers. Please set OGR_INCLUDE_DIRS.")
  ENDIF (NOT OGR_INCLUDE_DIRS)
  INCLUDE_DIRECTORIES(${OGR_INCLUDE_DIRS})

  # Check if ${GDAL_LIBRARY} has tiff library
  TRY_COMPILE(GDAL_HAS_TIFF
  ${CMAKE_CURRENT_BINARY_DIR}/CMake
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasTiff.cxx
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
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
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GEOTIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
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
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${JPEG_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
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

  # Check if ${GDAL_LIBRARY} has ogr library
  TRY_COMPILE(GDAL_HAS_OGR
  ${CMAKE_CURRENT_BINARY_DIR}/CMake
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasOGR.cxx
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${OGR_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
  OUTPUT_VARIABLE OUTPUT)
  IF(GDAL_HAS_OGR)
  MESSAGE(STATUS "Testing if GDAL has OGR     -- yes")
  SET(OGR_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_OGR)
  MESSAGE(STATUS "Testing if GDAL has OGR     -- no")
  MESSAGE(FATAL_ERROR "OGR MESSAGE: ${OUTPUT}")
  FIND_LIBRARY(OGR_LIBRARY ogr PATHS)
  IF (NOT OGR_LIBRARY)
  MESSAGE(FATAL_ERROR
           "Cannot find ogr library. Please set OGR_LIBRARY.")
  ENDIF (NOT OGR_LIBRARY)
  ENDIF(GDAL_HAS_OGR)
ELSE(OTB_USE_EXTERNAL_GDAL)
ENDIF(OTB_USE_EXTERNAL_GDAL)

#Experimental
OPTION(OTB_USE_GETTEXT "Get Text Library." ON)

IF(OTB_USE_GETTEXT)
FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND)
  FIND_LIBRARY(GETTEXT_LIBRARY gettextlib DOC "GetText library")
  IF(APPLE OR WIN32)
    FIND_LIBRARY(GETTEXT_INTL_LIBRARY intl DOC "GetText intl library")
  ENDIF(APPLE OR WIN32)

  IF(GETTEXT_LIBRARY)
      SET(OTB_I18N 1)
      MESSAGE(STATUS
              "Gettext found, configuring internationalization")
      SET(OTB_LANG $ENV{LANG} CACHE STRING "OTB internationalization (Experimental)")#might want to get the Locale from the system here
      IF(NOT OTB_LANG)
        SET(OTB_LANG "en_EN.UTF-8" CACHE STRING "OTB internationalization (Experimental)" FORCE)
      ENDIF(NOT OTB_LANG)
      SET(OTB_LANG_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/I18n)
      ADD_SUBDIRECTORY(I18n)
      FIND_PATH(GETTEXT_INCLUDE_DIR
        libintl.h
        DOC "Path to gettext include directory (where libintl.h can be found)")
      MARK_AS_ADVANCED(GETTEXT_INCLUDE_DIR)
    
      IF(GETTEXT_INCLUDE_DIR)
        INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
      ENDIF(GETTEXT_INCLUDE_DIR)
  ENDIF(GETTEXT_LIBRARY)

ELSE(GETTEXT_FOUND)
  SET(OTB_I18N 0)
  MESSAGE(STATUS
              "Gettext not found, internationalization will not be available")
ENDIF(GETTEXT_FOUND)
ENDIF(OTB_USE_GETTEXT)

#-------------------------------
# End connections whith external libraries
#-------------------------------

#-----------------------------------------------------------------------------
# Installation vars.
# OTB_INSTALL_BIN_DIR          - binary dir (executables)
# OTB_INSTALL_LIB_DIR          - library dir (libs)
# OTB_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# OTB_INSTALL_INCLUDE_DIR      - include dir (headers)
# OTB_INSTALL_PACKAGE_DIR      - package/export configuration files
# OTB_INSTALL_NO_DEVELOPMENT   - do not install development files
# OTB_INSTALL_NO_RUNTIME       - do not install runtime files
# OTB_INSTALL_NO_DOCUMENTATION - do not install documentation files

IF(NOT OTB_INSTALL_BIN_DIR)
  SET(OTB_INSTALL_BIN_DIR "/bin")
ENDIF(NOT OTB_INSTALL_BIN_DIR)

IF(NOT OTB_INSTALL_LIB_DIR)
  SET(OTB_INSTALL_LIB_DIR "/lib/otb")
ENDIF(NOT OTB_INSTALL_LIB_DIR)

IF(NOT OTB_INSTALL_DATA_DIR)
  SET(OTB_INSTALL_DATA_DIR "/share/otb")
ENDIF(NOT OTB_INSTALL_DATA_DIR)

IF(NOT OTB_INSTALL_INCLUDE_DIR)
  SET(OTB_INSTALL_INCLUDE_DIR "/include/otb")
ENDIF(NOT OTB_INSTALL_INCLUDE_DIR)

IF(NOT OTB_INSTALL_PACKAGE_DIR)
  SET(OTB_INSTALL_PACKAGE_DIR ${OTB_INSTALL_LIB_DIR}
    CACHE INTERNAL "")
ENDIF(NOT OTB_INSTALL_PACKAGE_DIR)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  SET(OTB_INSTALL_NO_DEVELOPMENT 0)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

IF(NOT OTB_INSTALL_NO_RUNTIME)
  SET(OTB_INSTALL_NO_RUNTIME 0)
ENDIF(NOT OTB_INSTALL_NO_RUNTIME)

IF(NOT OTB_INSTALL_NO_DOCUMENTATION)
  SET(OTB_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT OTB_INSTALL_NO_DOCUMENTATION)

SET(OTB_INSTALL_NO_LIBRARIES)
IF(OTB_BUILD_SHARED_LIBS)
  IF(OTB_INSTALL_NO_RUNTIME AND OTB_INSTALL_NO_DEVELOPMENT)
    SET(OTB_INSTALL_NO_LIBRARIES 1)
  ENDIF(OTB_INSTALL_NO_RUNTIME AND OTB_INSTALL_NO_DEVELOPMENT)
ELSE(OTB_BUILD_SHARED_LIBS)
  IF(OTB_INSTALL_NO_DEVELOPMENT)
    SET(OTB_INSTALL_NO_LIBRARIES 1)
  ENDIF(OTB_INSTALL_NO_DEVELOPMENT)
ENDIF(OTB_BUILD_SHARED_LIBS)

# Because INSTALL_* commands require a leading / and because INSTALL (cmake 2.4
# and newer) requires no leading / to install under INSTALL_PREFIX, we
# are stripping the leading /. In the future, there should be no leading
# / in any install directory variables
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_LIB_DIR_CM24 "${OTB_INSTALL_LIB_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_BIN_DIR_CM24 "${OTB_INSTALL_BIN_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_INCLUDE_DIR_CM24 "${OTB_INSTALL_INCLUDE_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_PACKAGE_DIR_CM24 "${OTB_INSTALL_PACKAGE_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_DOXYGEN_DIR_CM24 "${OTB_INSTALL_DOXYGEN_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_TCL_DIR_CM24 "${OTB_INSTALL_TCL_DIR}")
STRING(REGEX REPLACE "^/" "" OTB_INSTALL_JAVA_DIR_CM24 "${OTB_INSTALL_JAVA_DIR}")

# Make sure the user does not try to install on top of the build tree.
IF(WIN32 OR APPLE OR UNIX)
  STRING(TOLOWER "${CMAKE_INSTALL_PREFIX}" _PREFIX)
  STRING(TOLOWER "${OTB_BINARY_DIR}" _BUILD)
ELSE(WIN32 OR APPLE OR UNIX)
  SET(_PREFIX "${CMAKE_INSTALL_PREFIX}")
  SET(_BUILD "${OTB_BINARY_DIR}")
ENDIF(WIN32 OR APPLE OR UNIX)
IF("${_PREFIX}" STREQUAL "${_BUILD}")
  MESSAGE(FATAL_ERROR
    "The current CMAKE_INSTALL_PREFIX points at the build tree:\n"
    "  ${CMAKE_INSTALL_PREFIX}\n"
    "This is not supported."
    )
ENDIF("${_PREFIX}" STREQUAL "${_BUILD}")


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
    FIND_FILE(ITK_FLTK_RESOURCE mac.r ${FLTK_DIR}/include/FL)
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
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)


#-----------------------------------------------------------------------------
# Output directories.


#       IF(NOT LIBRARY_OUTPUT_PATH)
SET (LIBRARY_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
#       ENDIF(NOT LIBRARY_OUTPUT_PATH)
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

#-----------------------------------------------------------------------------
# Option for display all developpment message !!!
OPTION(OTB_SHOW_ALL_MSG_DEBUG "OTB developers : Show all messages (debug, dev, etc...) use only for otb developers." OFF)
MARK_AS_ADVANCED(OTB_SHOW_ALL_MSG_DEBUG)
# IF( OTB_SHOW_ALL_MSG_DEBUG )
#  ADD_DEFINITIONS(-DOTB_SHOW_ALL_MSG_DEBUG)
# ENDIF( OTB_SHOW_ALL_MSG_DEBUG )

OPTION(OTB_COMPILE_WITH_FULL_WARNING "OTB developers : Compilation with FULL warning (use only for otb developers)." OFF)
MARK_AS_ADVANCED(OTB_COMPILE_WITH_FULL_WARNING)
IF(OTB_COMPILE_WITH_FULL_WARNING)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(OTB_FULL_WARNING_LIST_PARAMETERS "-Wall -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value -Wunused-variable -Wuninitialized -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch" CACHE STRING "List of compilation parameters.")
    MARK_AS_ADVANCED(OTB_FULL_WARNING_LIST_PARAMETERS)
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ENDIF(OTB_COMPILE_WITH_FULL_WARNING)

OPTION(OTB_CPP_CHECK_SOURCE_FILE "OTB developers : Checking cpp source file (use only for otb developers)." OFF)
MARK_AS_ADVANCED(OTB_CPP_CHECK_SOURCE_FILE)
IF(OTB_CPP_CHECK_SOURCE_FILE)
  FIND_PROGRAM(OTB_CPP_CHECK_PROG "")
ENDIF(OTB_CPP_CHECK_SOURCE_FILE)

MACRO(OTB_ADD_EXECUTABLE name sources libraries)
#  MESSAGE(STATUS "name: ${name}")
#  MESSAGE(STATUS "sources: ${sources}")
#  MESSAGE(STATUS "libraries: ${libraries}")
  ADD_EXECUTABLE(${name} ${sources})
  TARGET_LINK_LIBRARIES(${name}  ${libraries})
  IF(OTB_CPP_CHECK_SOURCE_FILE)
#  SET_TARGET_PROPERTIES(otbBasicFiltersTests1  PROPERTIES "OTBBasicFilters")
#  ADD_DEPENDENCIES(otbBasicFiltersTests1  OTBBasicFilters)
    FOREACH( file ${sources})
      ADD_CUSTOM_COMMAND(   TARGET ${name}
                            PRE_BUILD
                            COMMAND ${OTB_CPP_CHECK_PROG}
                            ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
                            DEPENDS ${libraries}
                            COMMENT "============== C++ checking ${file} source file...  =================="
                            VERBATIM )
    ENDFOREACH(file)
  ENDIF(OTB_CPP_CHECK_SOURCE_FILE)
ENDMACRO(OTB_ADD_EXECUTABLE)


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
# Note: 8*4000*4000 = 128000000 (double 4000x4000 image)
SET(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING 128000000 CACHE STRING "Image size to activate using streaming for applications.")
MARK_AS_ADVANCED(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING)
SET(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING 128000000 CACHE STRING "Max size buffer for streaming.")
MARK_AS_ADVANCED(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING)



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
# # #
# # # SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES "${GDAL_LIBRARY};dxf;tinyXML;otbliblas;otbossim;otbossimplugins" )
# # #
# # # IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
# # #         SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};${OPENTHREADS_LIBRARY}")
# # # ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
# # #
# # # # Add EXPAT library
# # # IF( OTB_USE_EXTERNAL_EXPAT )
# # #         SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};expat")
# # # ELSE( OTB_USE_EXTERNAL_EXPAT  )
# # #         SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES  "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};otbexpat")
# # # ENDIF( OTB_USE_EXTERNAL_EXPAT )
# # #
# # # # Add Open Jpeg library
# # # IF( OTB_COMPILE_JPEG2000 )
# # #      SET(OTB_IO_UTILITIES_DEPENDENT_LIBRARIES "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES};otbopenjpeg")
# # # ENDIF( OTB_COMPILE_JPEG2000 )
# # #
# # # # => Create OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES
# # # SET(OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES "otb6S;otbgalib;InsightJournal;otbsvm")
# # #
# # #
# # # # => Create OTB_ALLS_UTILITIES_DEPENDENT_LIBRARIES
# # # SET(OTB_ALLS_UTILITIES_DEPENDENT_LIBRARIES "${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES} ${OTB_ALGO_UTILITIES_DEPENDENT_LIBRARIES}")
# # #

#-----------------------------------------------------------------------------
OPTION(BUILD_EXAMPLES "Build the Examples directory." OFF)


#-----------------------------------------------------------------------------
# The entire OTB tree should use the same include path

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
# Dispatch the build into the proper subdirectories.

#Note: SUBDIRS is deprecated and is replaced by ADD_SUBDIRECTORY
#SUBDIRS(Utilities Code)
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(Code)

IF (BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(Examples)
ENDIF (BUILD_EXAMPLES)

IF (BUILD_TESTING)
  ADD_SUBDIRECTORY(Testing)
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
FIND_PATH(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB_SOURCE_DIR}/../OTB-Data)
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
# The entire OTB tree should use the same include path

# Create the list of include directories needed for OTB header files.
#INCLUDE(${OTB_SOURCE_DIR}/otbIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
#INCLUDE_DIRECTORIES(
#  ${OTB_INCLUDE_DIRS_BUILD_TREE}
#  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
#  ${OTB_INCLUDE_DIRS_SYSTEM}
#)

#-----------------------------------------------------------------------------
# Uninstall cmake use to uninstall OTB.
#    CONFIGURE_FILE(
#      "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
#      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#      IMMEDIATE @ONLY)

#    ADD_CUSTOM_TARGET(uninstall
#      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Help other projects use OTB.

# Copy the UseOTB.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/UseOTB.cmake.in
               ${OTB_BINARY_DIR}/UseOTB.cmake COPYONLY IMMEDIATE)


# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${OTB_BINARY_DIR}/OTBBuildSettings.cmake)
#INSTALL_FILES(/lib/otb .cmake OTBBuildSettings)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    ${OTB_BINARY_DIR}/OTBBuildSettings.cmake
    ${OTB_BINARY_DIR}/OTBLibraryDepends.cmake
    ${OTB_BINARY_DIR}/UseOTB.cmake
    ${OTB_SOURCE_DIR}/CMake/FindOpenThreads.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR_CM24}
    COMPONENT Development
    )
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# Create the OTBConfig.cmake file containing the OTB configuration.
INCLUDE (${OTB_SOURCE_DIR}/otbGenerateOTBConfig.cmake)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${OTB_BINARY_DIR}/otbConfigure.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)


#INSTALL_FILES(/include/otb .h otbConfigure)
#INSTALL_FILES(/lib/otb .cmake UseOTB OTBLibraryDepends OTBConfig )

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

OPTION(OTB_USE_CPACK "Generate installer using CPack" OFF)
MARK_AS_ADVANCED(OTB_USE_CPACK)

IF(OTB_USE_CPACK)

    INCLUDE(InstallRequiredSystemLibraries)
    
    SET(CPACK_PACKAGE_NAME "OTB" CACHE STRING "Package name")
    MARK_AS_ADVANCED(CPACK_PACKAGE_NAME)
    
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Orfeo Toolbox")
    MARK_AS_ADVANCED(CPACK_PACKAGE_DESCRIPTION_SUMMARY)
    
    SET(CPACK_PACKAGE_VERSION  "${OTB_VERSION_STRING}")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${OTB_VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${OTB_VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${OTB_VERSION_PATCH}")
    
    SET(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org" CACHE STRING  "Orfeo toolbox contact email")
    
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Description.txt")
    
    #Debian specific
    SET(DEBIAN_PACKAGE_MAINTAINER  "debian@orfeo-toolbox.org" CACHE STRING  "Debian package maintainer email")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libgdal1-1.5.0 (>= 1.5.1-0), libfltk1.1 (>= 1.1.7-3), libcurl3 (>=7.15.5-1etch1), libfftw3-3 (>=3.1.2-3.1)" CACHE STRING "Debian package dependance" )
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" CACHE STRING "arch")
    MARK_AS_ADVANCED(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    SET(CPACK_DEBIAN_PACKAGE_NAME "libotb" CACHE STRING "Debian package name")
    
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OrfeoToolbox-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
    
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright/OTBCopyright.txt")
    
    FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_RPM_PACKAGE_DESCRIPTION)
    FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)
    
    IF(WIN32 AND NOT UNIX AND NOT CYGWIN)
    #Find gdal dll files, localized in the GDAL_LIBRARY directory
    GET_FILENAME_COMPONENT(GDAL_LIB_DIR "${GDAL_LIBRARY}" PATH )
    SET(GDAL_LIB_DIR "${GDAL_LIB_DIR}/" )
    INSTALL(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 bin
            FILES_MATCHING  PATTERN     "*.dll" )
    INSTALL(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 lib
            FILES_MATCHING  PATTERN     "*.lib" )
    ENDIF(WIN32 AND NOT UNIX AND NOT CYGWIN)
    
    INCLUDE(CPack)

ENDIF(OTB_USE_CPACK)

FILE(GLOB otbconfigfileCreation_SRCS "${OTB_SOURCE_DIR}/CMake/GenerateConfigProperties.cpp")
SET(otbconfigfile_DEFAULT "${OTB_BINARY_DIR}/otb.conf")

ADD_EXECUTABLE(GenerateConfigProperties ${otbconfigfileCreation_SRCS})

ADD_CUSTOM_COMMAND(
          TARGET GenerateConfigProperties
          POST_BUILD
          COMMAND GenerateConfigProperties
          ARGS "${otbconfigfile_DEFAULT}" "${OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING}" "${OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING}"
          COMMENT "Generating ${otbconfigfile_DEFAULT}" )
