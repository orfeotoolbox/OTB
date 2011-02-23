MESSAGE(STATUS "Importing GDAL...")
# Option for internal/external GDAL
OPTION(OTB_USE_EXTERNAL_GDAL "Use an outside build of GDAL. (OFF is NOT IMPLEMENTED)" ON)
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
  MESSAGE(STATUS "  GDAL includes : ${GDAL_INCLUDE_DIR}")
  MESSAGE(STATUS "  GDAL library  : ${GDAL_LIBRARY}")

  # Find gdal version
  FIND_PROGRAM(GDALCONFIG_EXECUTABLE gdal-config)
  IF (GDALCONFIG_EXECUTABLE)
      EXECUTE_PROCESS(COMMAND ${GDALCONFIG_EXECUTABLE} --version
      OUTPUT_VARIABLE GDAL_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
      MESSAGE(STATUS "  GDAL version is " ${GDAL_VERSION})
      SET(GDAL_VERSION ${GDAL_VERSION} CACHE STRING "GDAL version" FORCE)
      MARK_AS_ADVANCED(GDAL_VERSION)
  ELSE (GDALCONFIG_EXECUTABLE)    
      MESSAGE(STATUS "  gdal-config not found")
  ENDIF (GDALCONFIG_EXECUTABLE)
 
  # Detect if gdal support hdf format
  FIND_PROGRAM(GDALCONFIG_EXECUTABLE gdal-config)
  IF (GDALCONFIG_EXECUTABLE)  
     EXECUTE_PROCESS(COMMAND ${GDALCONFIG_EXECUTABLE} --formats
                     OUTPUT_VARIABLE GDAL_FORMATS
                     OUTPUT_STRIP_TRAILING_WHITESPACE
                    )
      IF ( ${GDAL_FORMATS} MATCHES "hdf" )
         SET(CHECK_GDAL_BUILDED_WITH_HDF 1 CACHE INTERNAL "GDAL_BUILDED_WITH_HDF" FORCE)
      ELSE ( ${GDAL_FORMATS} MATCHES "hdf")
         MESSAGE(WARNING "CHECK_GDAL_BUILDED_WITH_HDF test failed : your GDAL is not builded with a hdf library. So the HDF tests will be deactivated. Formats supported by your GDAL: " [ ${GDAL_FORMATS} ] )
         SET(CHECK_GDAL_BUILDED_WITH_HDF 0 CACHE INTERNAL "GDAL_BUILDED_WITH_HDF" FORCE)
      ENDIF(${GDAL_FORMATS} MATCHES "hdf")
  ELSE (GDALCONFIG_EXECUTABLE)    
    MESSAGE(WARNING "gdal-config not found")
  ENDIF (GDALCONFIG_EXECUTABLE) 

  # Find geotiff headers
  FIND_PATH(GEOTIFF_INCLUDE_DIRS geotiff.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include/geotiff /usr/include/libgeotiff)
  IF (NOT GEOTIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find geotiff headers. Please set GEOTIFF_INCLUDE_DIRS.")
  ENDIF (NOT GEOTIFF_INCLUDE_DIRS)
#  INCLUDE_DIRECTORIES(${GEOTIFF_INCLUDE_DIRS})

  # Find tiff headers
  FIND_PATH(TIFF_INCLUDE_DIRS tiffio.h $ENV{GDAL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR} /usr/include)
  IF (NOT TIFF_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR
           "Cannot find libtiff headers. Please set TIFF_INCLUDE_DIRS.")
  ENDIF (NOT TIFF_INCLUDE_DIRS)
#  INCLUDE_DIRECTORIES(${TIFF_INCLUDE_DIRS})

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
  MESSAGE(STATUS "  Testing if GDAL has tiff    -- yes")
  SET(TIFF_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_TIFF)
  MESSAGE(STATUS "  Testing if GDAL has tiff    -- no")
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
  MESSAGE(STATUS "  Testing if GDAL has geotiff -- yes")
  SET(GEOTIFF_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_GEOTIFF)
  MESSAGE(STATUS "  Testing if GDAL has geotiff -- no")
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
  MESSAGE(STATUS "  Testing if GDAL has jpeg    -- yes")
  SET(JPEG_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_JPEG)
  MESSAGE(STATUS "  Testing if GDAL has jpeg    -- no")
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
  MESSAGE(STATUS "  Testing if GDAL has OGR     -- yes")
  SET(OGR_LIBRARY ${GDAL_LIBRARY})
  ELSE(GDAL_HAS_OGR)
  MESSAGE(STATUS "  Testing if GDAL has OGR     -- no")
  MESSAGE(FATAL_ERROR "OGR MESSAGE: ${OUTPUT}")
  FIND_LIBRARY(OGR_LIBRARY ogr PATHS)
  IF (NOT OGR_LIBRARY)
  MESSAGE(FATAL_ERROR
           "Cannot find ogr library. Please set OGR_LIBRARY.")
  ENDIF (NOT OGR_LIBRARY)
  ENDIF(GDAL_HAS_OGR)

  # Check for some possible crashes when reading TIFF files, when GDAL embeds geotiff and tiff libraries
  # Known to lead to a crash with the official gdal package available in ubuntu & debian 
  INCLUDE(CheckCXXSourceRunsWithArgs)
  
  # This test is known to fail with the Debian gdal package
  SET(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
  SET(CMAKE_REQUIRED_LIBRARIES "${GEOTIFF_LIBRARY};${GDAL_LIBRARY}")
  CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALOpen.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otb_logo.tif
            CHECK_GDALOPEN_SYMBOL
            )
  IF(NOT CHECK_GDALOPEN_SYMBOL)
    MESSAGE(WARNING "CHECK_GDALOPEN_SYMBOL test failed : your platform exhibits a gdal/geotiff conflict. "
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured "
    "embeds its own libgeotiff and is compiled with --with-hide-internal-symbols=yes.\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols" )
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
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured "
    "embeds its own libgeotiff and is compiled with --with-hide-internal-symbols=yes.\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols" )
  ENDIF(NOT CHECK_XTIFFOPEN_SYMBOL)
  
  # This test is known to fail with gdal build with some versions of hdf library
  #${OTB_DATA_ROOT}/Input/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf  -> Test KO
  #${OTB_DATA_ROOT}/Input/GSSTF_NCEP.2b.2008.12.31.he5  -> Test OK
  IF(CHECK_GDAL_BUILDED_WITH_HDF)
    IF(OTB_DATA_ROOT)
      SET(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
      SET(CMAKE_REQUIRED_LIBRARIES "${GDAL_LIBRARY}")
      CHECK_CXX_SOURCE_RUNS_ARGS(
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestHDF4Open.cxx
              ${OTB_DATA_ROOT}/Input/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf 
              CHECK_HDF4OPEN_SYMBOL
              )
      IF(NOT CHECK_HDF4OPEN_SYMBOL)
        MESSAGE(WARNING "CHECK_HDF4OPEN_SYMBOL test failed : your platform exhibits a problem to read HDF4 files. So the tests with HDF4 will be deactivated" )
      ENDIF(NOT CHECK_HDF4OPEN_SYMBOL)
    ENDIF(OTB_DATA_ROOT)
  ENDIF(CHECK_GDAL_BUILDED_WITH_HDF)

ELSE(OTB_USE_EXTERNAL_GDAL)

  MESSAGE(FATAL_ERROR "OTB_USE_EXTERNAL_GDAL=OFF is not implemented. Please set OTB_USE_EXTERNAL_GDAL to ON")

ENDIF(OTB_USE_EXTERNAL_GDAL)
