MESSAGE(STATUS "Importing GDAL...")
# Option for internal/external GDAL
OPTION(OTB_USE_EXTERNAL_GDAL "Use an outside build of GDAL. (OFF is NOT IMPLEMENTED)" ON)
MARK_AS_ADVANCED(OTB_USE_EXTERNAL_GDAL)

IF(OTB_USE_EXTERNAL_GDAL)

  FIND_PACKAGE(GDAL)
  MARK_AS_ADVANCED(GDAL_INCLUDE_DIR)
  MARK_AS_ADVANCED(GDAL_LIBRARY)
  MARK_AS_ADVANCED(GDAL_CONFIG)

  IF (NOT GDAL_FOUND)
     MESSAGE(FATAL_ERROR
             "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
  ENDIF(NOT GDAL_FOUND)

  INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})

  MESSAGE(STATUS "  GDAL includes : ${GDAL_INCLUDE_DIR}")
  MESSAGE(STATUS "  GDAL library  : ${GDAL_LIBRARY}")

  IF(UNIX)
    IF (NOT GDAL_CONFIG)
      MESSAGE(FATAL_ERROR
              "Cannot find gdal-config. Set GDAL_CONFIG")
    ENDIF(NOT GDAL_FOUND)
   
    EXECUTE_PROCESS(COMMAND ${GDAL_CONFIG} --version
                    OUTPUT_VARIABLE GDAL_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
   
    # Detect if gdal support hdf format
    EXECUTE_PROCESS(COMMAND ${GDAL_CONFIG} --formats
                    OUTPUT_VARIABLE GDAL_FORMATS
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
    IF ( ${GDAL_FORMATS} MATCHES "hdf" )
       SET(CHECK_GDAL_BUILDED_WITH_HDF 1 CACHE INTERNAL "GDAL_BUILDED_WITH_HDF" FORCE)
    ELSE ( ${GDAL_FORMATS} MATCHES "hdf")
       MESSAGE(STATUS "CHECK_GDAL_BUILDED_WITH_HDF test failed : GDAL is not built with hdf support. So the HDF tests will be deactivated. Formats supported by your GDAL: " [ ${GDAL_FORMATS} ] )
       SET(CHECK_GDAL_BUILDED_WITH_HDF 0 CACHE INTERNAL "GDAL_BUILDED_WITH_HDF" FORCE)
    ENDIF(${GDAL_FORMATS} MATCHES "hdf")
  ELSE (UNIX)
  	# For WIN32 platform, GDAL is automatically built with HDF support.
    SET(CHECK_GDAL_BUILDED_WITH_HDF 1 CACHE INTERNAL "GDAL_BUILDED_WITH_HDF" FORCE)
    # GDAL_INCLUDE_DIR has already been checked here
    FILE(READ "${GDAL_INCLUDE_DIR}/gdal_version.h" _gdal_version_h_CONTENTS)
    STRING(REGEX REPLACE ".*# *define GDAL_RELEASE_NAME *\"([0-9.]+)\".*" "\\1" GDAL_VERSION "${_gdal_version_h_CONTENTS}")
  ENDIF (UNIX)

  MESSAGE(STATUS "  GDAL version is " ${GDAL_VERSION})
  SET(GDAL_VERSION ${GDAL_VERSION} CACHE STRING "GDAL version" FORCE)
  MARK_AS_ADVANCED(GDAL_VERSION)

  # Decompose GDAL version
  STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" GDAL_VERSION_MAJOR "${GDAL_VERSION}")
  STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" GDAL_VERSION_MINOR "${GDAL_VERSION}")
  STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" GDAL_VERSION_PATCH "${GDAL_VERSION}")
  SET(GDAL_VERSION_MAJOR_STRING "${GDAL_VERSION_MAJOR}" CACHE STRING "Major Version Number of GDAL package found")
  SET(GDAL_VERSION_MINOR_STRING "${GDAL_VERSION_MINOR}" CACHE STRING "Minor Version Number of GDAL package found")
  SET(GDAL_VERSION_PATCH_STRING "${GDAL_VERSION_PATCH}" CACHE STRING "Patch Version Number of GDAL package found")
  MARK_AS_ADVANCED(GDAL_VERSION_MAJOR_STRING)
  MARK_AS_ADVANCED(GDAL_VERSION_MINOR_STRING)
  MARK_AS_ADVANCED(GDAL_VERSION_PATCH_STRING)
  
  MATH(EXPR GDAL_NUM_VERSION "((${GDAL_VERSION_MAJOR})*100+${GDAL_VERSION_MINOR})*100+${GDAL_VERSION_PATCH}")
  # MESSAGE(STATUS "GDAL Num version: ${GDAL_NUM_VERSION}")
  # ADD_DEFINITIONS(-DGDAL_VERSION=${GDAL_NUM_VERSION})

  # Find geotiff headers
  FIND_PATH(GEOTIFF_INCLUDE_DIRS geotiff.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${OSGEO4W_INCLUDE} /usr/include/geotiff /usr/include/libgeotiff)
  MARK_AS_ADVANCED(GEOTIFF_INCLUDE_DIRS)
  IF (NOT GEOTIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find geotiff headers. Please set GEOTIFF_INCLUDE_DIRS.")
  ENDIF (NOT GEOTIFF_INCLUDE_DIRS)
#  INCLUDE_DIRECTORIES(${GEOTIFF_INCLUDE_DIRS})

  # Find tiff headers
  FIND_PATH(TIFF_INCLUDE_DIRS tiffio.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${OSGEO4W_INCLUDE} /usr/include)
  MARK_AS_ADVANCED(TIFF_INCLUDE_DIRS)
  IF (NOT TIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find libtiff headers. Please set TIFF_INCLUDE_DIRS.")
  ENDIF (NOT TIFF_INCLUDE_DIRS)
#  INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

  # Find jpeg headers
  FIND_PATH(JPEG_INCLUDE_DIRS jpeglib.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${OSGEO4W_INCLUDE} /usr/include)
  MARK_AS_ADVANCED(JPEG_INCLUDE_DIRS)
  IF (NOT JPEG_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find jpeg headers. Please set JPEG_INCLUDE_DIRS.")
  ENDIF (NOT JPEG_INCLUDE_DIRS)
  INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})

  # Find ogr headers
  FIND_PATH(OGR_INCLUDE_DIRS ogrsf_frmts.h  $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} ${OSGEO4W_INCLUDE} /usr/include)
  MARK_AS_ADVANCED(OGR_INCLUDE_DIRS)
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
    MESSAGE(STATUS "  Testing if GDAL exposes tiff library symbols      -- yes")
    SET(TIFF_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_TIFF)
    MESSAGE(STATUS "  Testing if GDAL exposes tiff library symbols      -- no")
    FIND_LIBRARY(TIFF_LIBRARY NAMES tiff libtiff_i PATHS  ${OSGEO4W_LIB})
    MARK_AS_ADVANCED(TIFF_LIBRARY)
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
    MESSAGE(STATUS "  Testing if GDAL exposes geotiff library symbols   -- yes")
    SET(GEOTIFF_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_GEOTIFF)
    MESSAGE(STATUS "  Testing if GDAL exposes geotiff library symbols   -- no")
    FIND_LIBRARY(GEOTIFF_LIBRARY NAMES geotiff geotiff_i PATHS ${OSGEO4W_LIB})
    MARK_AS_ADVANCED(GEOTIFF_LIBRARY)
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
    MESSAGE(STATUS "  Testing if GDAL exposes jpeg library symbols      -- yes")
    SET(JPEG_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_JPEG)
    MESSAGE(STATUS "  Testing if GDAL exposes jpeg library symbols      -- no")
    FIND_LIBRARY(JPEG_LIBRARY NAMES jpeg jpeg_i PATHS ${OSGEO4W_LIB})
    MARK_AS_ADVANCED(JPEG_LIBRARY)
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
    MESSAGE(STATUS "  Testing if GDAL has OGR                           -- yes")
    SET(OGR_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_OGR)
    MESSAGE(STATUS "  Testing if GDAL has OGR                           -- no")
    MESSAGE(FATAL_ERROR "OGR MESSAGE: ${OUTPUT}")
    FIND_LIBRARY(OGR_LIBRARY ogr PATHS)
    MARK_AS_ADVANCED(OGR_LIBRARY)
    IF (NOT OGR_LIBRARY)
      MESSAGE(FATAL_ERROR
              "Cannot find ogr library. Please set OGR_LIBRARY.")
    ENDIF (NOT OGR_LIBRARY)
  ENDIF(GDAL_HAS_OGR)

  
  # Check for some possible crashes when reading TIFF files, when GDAL embeds geotiff and tiff libraries
  # Known to lead to a crash with the official gdal package available in ubuntu & debian 
  INCLUDE(CheckCXXSourceRunsWithArgs)
  
  IF(NOT WIN32)
  # This test is known to fail with the Debian gdal package
  # On Windows, the try-compile are always run in Debug, leading to runtime issues, so we disable them
  SET(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
  SET(CMAKE_REQUIRED_LIBRARIES "${GEOTIFF_LIBRARY};${GDAL_LIBRARY}")
  CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALOpen.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otb_logo.tif
            CHECK_GDALOPEN_SYMBOL
            )
  IF(NOT CHECK_GDALOPEN_SYMBOL)
    MESSAGE(WARNING "CHECK_GDALOPEN_SYMBOL test failed : your platform exhibits a gdal/geotiff conflict.\n"
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured\n"
    "embeds its own version of libgeotiff and is ABI incompatible with the system libgeotiff\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols\n"
    "and pointing GEOTIFF_INCLUDE_DIRS to the geotiff headers included in gdal sources" )
  ENDIF(NOT CHECK_GDALOPEN_SYMBOL)
  
  # This test is known to fail with the Ubuntu gdal package
  SET(CMAKE_REQUIRED_INCLUDES ${GEOTIFF_INCLUDE_DIRS};${TIFF_INCLUDE_DIRS})
  SET(CMAKE_REQUIRED_LIBRARIES "${GDAL_LIBRARY};${GEOTIFF_LIBRARY};${TIFF_LIBRARY}")
  CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestXTIFFOpen.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otb_logo.tif
            CHECK_XTIFFOPEN_SYMBOL
            )
  IF(NOT CHECK_XTIFFOPEN_SYMBOL)
    MESSAGE(WARNING "CHECK_XTIFFOPEN_SYMBOL test failed : your platform exhibits a gdal/geotiff conflict. "
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured"
    "embeds its own version of libgeotiff and is ABI incompatible with the system libgeotiff\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols "
    "and pointing GEOTIFF_INCLUDE_DIRS to the geotiff headers included in gdal sources" )
  ENDIF(NOT CHECK_XTIFFOPEN_SYMBOL)
  ENDIF(NOT WIN32)

  # This test is known to fail with gdal build with some versions of hdf library
  #${OTB_DATA_ROOT}/Input/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf  -> Test KO
  #${OTB_DATA_ROOT}/Input/GSSTF_NCEP.2b.2008.12.31.he5  -> Test OK
  #${CMAKE_CURRENT_SOURCE_DIR}/CMake/example.hdf -> Test should be KO in some special cases
  IF(CHECK_GDAL_BUILDED_WITH_HDF)
    SET(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
    SET(CMAKE_REQUIRED_LIBRARIES "${GDAL_LIBRARY}")
    CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestHDF4Open.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestHDF4Open.hdf 
            CHECK_HDF4OPEN_SYMBOL
            )
    IF(NOT CHECK_HDF4OPEN_SYMBOL)
      MESSAGE(STATUS "CHECK_HDF4OPEN_SYMBOL test failed : your platform exhibits a problem to read HDF4 files. So the tests with HDF4 will be deactivated" )
    ENDIF(NOT CHECK_HDF4OPEN_SYMBOL)
  ENDIF(CHECK_GDAL_BUILDED_WITH_HDF)

ELSE(OTB_USE_EXTERNAL_GDAL)

  MESSAGE(FATAL_ERROR "OTB_USE_EXTERNAL_GDAL=OFF is not implemented. Please set OTB_USE_EXTERNAL_GDAL to ON")

ENDIF(OTB_USE_EXTERNAL_GDAL)
