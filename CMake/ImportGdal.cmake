message(STATUS "Importing GDAL...")
# Option for internal/external GDAL
option(OTB_USE_EXTERNAL_GDAL "Use an outside build of GDAL. (OFF is NOT IMPLEMENTED)" ON)
mark_as_advanced(OTB_USE_EXTERNAL_GDAL)

if(OTB_USE_EXTERNAL_GDAL)

  find_package(GDAL)
  mark_as_advanced(GDAL_INCLUDE_DIR)
  mark_as_advanced(GDAL_LIBRARY)
  mark_as_advanced(GDAL_CONFIG)

  if(NOT GDAL_FOUND)
     message(FATAL_ERROR "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
  endif()

  message(STATUS "  GDAL includes : ${GDAL_INCLUDE_DIR}")
  message(STATUS "  GDAL library  : ${GDAL_LIBRARY}")

  if(UNIX)
    if(NOT GDAL_CONFIG)
      message(FATAL_ERROR "Cannot find gdal-config. Set GDAL_CONFIG")
    endif()

    execute_process(COMMAND ${GDAL_CONFIG} --version
                    OUTPUT_VARIABLE GDAL_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Detect if gdal support hdf format
    execute_process(COMMAND ${GDAL_CONFIG} --formats
                    OUTPUT_VARIABLE GDAL_FORMATS
                    OUTPUT_STRIP_TRAILING_WHITESPACE )
    if( ${GDAL_FORMATS} MATCHES "hdf" )
       set(CHECK_GDAL_BUILT_WITH_HDF 1 CACHE INTERNAL "GDAL_BUILT_WITH_HDF" FORCE)
    else()
       message(STATUS "CHECK_GDAL_BUILT_WITH_HDF test failed : GDAL is not built with hdf support. So the HDF tests will be deactivated. Formats supported by your GDAL: " [ ${GDAL_FORMATS} ] )
       set(CHECK_GDAL_BUILT_WITH_HDF 0 CACHE INTERNAL "GDAL_BUILT_WITH_HDF" FORCE)
    endif()
    
    # Detect if gdal supports JPEG2000 format
    if ((${GDAL_FORMATS} MATCHES "openjpeg") OR 
        (${GDAL_FORMATS} MATCHES "jp2kak") OR
        (${GDAL_FORMATS} MATCHES "ecw"))
      message(STATUS "  GDAL is built with a suitable Jpeg2000 driver (OpenJpeg, Kakadu, ECW).")
    else()
      message(WARNING "GDAL has no suitable Jpeg2000 driver (compatible drivers are : OpenJpeg, Kakadu, ECW).")
    endif()
    
  else()
    # For WIN32 platform, we suppose that GDAL is built with HDF support (case with OSGeo4W)
    set(CHECK_GDAL_BUILT_WITH_HDF 1 CACHE INTERNAL "GDAL_BUILT_WITH_HDF" FORCE)
    # GDAL_INCLUDE_DIR has already been checked here
    file(READ "${GDAL_INCLUDE_DIR}/gdal_version.h" _gdal_version_h_CONTENTS)
    string(REGEX REPLACE ".*# *define GDAL_RELEASE_NAME *\"([0-9.]+)\".*" "\\1" GDAL_VERSION "${_gdal_version_h_CONTENTS}")
  endif()

  message(STATUS "  GDAL version is " ${GDAL_VERSION})
  set(GDAL_VERSION ${GDAL_VERSION} CACHE STRING "GDAL version" FORCE)
  mark_as_advanced(GDAL_VERSION)

  # Decompose GDAL version
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" GDAL_VERSION_MAJOR "${GDAL_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" GDAL_VERSION_MINOR "${GDAL_VERSION}")
  string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" GDAL_VERSION_PATCH "${GDAL_VERSION}")
  set(GDAL_VERSION_MAJOR_STRING "${GDAL_VERSION_MAJOR}" CACHE STRING "Major Version Number of GDAL package found")
  set(GDAL_VERSION_MINOR_STRING "${GDAL_VERSION_MINOR}" CACHE STRING "Minor Version Number of GDAL package found")
  set(GDAL_VERSION_PATCH_STRING "${GDAL_VERSION_PATCH}" CACHE STRING "Patch Version Number of GDAL package found")
  mark_as_advanced(GDAL_VERSION_MAJOR_STRING)
  mark_as_advanced(GDAL_VERSION_MINOR_STRING)
  mark_as_advanced(GDAL_VERSION_PATCH_STRING)

  math(EXPR GDAL_NUM_VERSION "((${GDAL_VERSION_MAJOR})*100+${GDAL_VERSION_MINOR})*100+${GDAL_VERSION_PATCH}")

  # Find geotiff headers
  find_path(GEOTIFF_INCLUDE_DIRS geotiff.h
            HINTS
              ${GDAL_INCLUDE_DIR}
            PATH_SUFFIXES
              geotiff
              libgeotiff)
  mark_as_advanced(GEOTIFF_INCLUDE_DIRS)
  if(NOT GEOTIFF_INCLUDE_DIRS)
    message(FATAL_ERROR "Cannot find geotiff headers. Please set GEOTIFF_INCLUDE_DIRS.")
  endif()

  # Find tiff headers
  find_path(TIFF_INCLUDE_DIRS tiffio.h
              HINTS ${GDAL_INCLUDE_DIR})
  mark_as_advanced(TIFF_INCLUDE_DIRS)
  if(NOT TIFF_INCLUDE_DIRS)
    message(FATAL_ERROR "Cannot find libtiff headers. Please set TIFF_INCLUDE_DIRS.")
  endif()

  # Find jpeg headers
  find_path(JPEG_INCLUDE_DIRS jpeglib.h
              HINTS ${GDAL_INCLUDE_DIR})
  find_path(JPEG_INCLUDE_DIRS jpeglib.h ${GDAL_INCLUDE_DIR})
  mark_as_advanced(JPEG_INCLUDE_DIRS)
  if(NOT JPEG_INCLUDE_DIRS)
    message(FATAL_ERROR "Cannot find jpeg headers. Please set JPEG_INCLUDE_DIRS.")
  endif()

  # Find ogr headers
  find_path(OGR_INCLUDE_DIRS ogrsf_frmts.h
              HINTS ${GDAL_INCLUDE_DIR})
  mark_as_advanced(OGR_INCLUDE_DIRS)
  if(NOT OGR_INCLUDE_DIRS)
    message(FATAL_ERROR "Cannot find ogr headers. Please set OGR_INCLUDE_DIRS.")
  endif()

  # Check if ${GDAL_LIBRARY} has tiff library
  try_compile(GDAL_HAS_TIFF
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasTiff.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${TIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
              OUTPUT_VARIABLE OUTPUT)
  if(GDAL_HAS_TIFF)
    message(STATUS "  Testing if GDAL exposes tiff library symbols      -- yes")
    set(TIFF_LIBRARY ${GDAL_LIBRARY})
  else()
    message(STATUS "  Testing if GDAL exposes tiff library symbols      -- no")
    find_library(TIFF_LIBRARY NAMES tiff libtiff_i)
    mark_as_advanced(TIFF_LIBRARY)
    if(NOT TIFF_LIBRARY)
      message(FATAL_ERROR "Cannot find tiff library. Please set TIFF_LIBRARY.")
    endif()
  endif()

  # Check if ${GDAL_LIBRARY} has geotiff library
  try_compile(GDAL_HAS_GEOTIFF
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasGeoTiff.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GEOTIFF_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
              OUTPUT_VARIABLE OUTPUT)
  if(GDAL_HAS_GEOTIFF)
    message(STATUS "  Testing if GDAL exposes geotiff library symbols   -- yes")
    set(GEOTIFF_LIBRARY ${GDAL_LIBRARY})
  else()
    message(STATUS "  Testing if GDAL exposes geotiff library symbols   -- no")
    find_library(GEOTIFF_LIBRARY NAMES geotiff geotiff_i)
    mark_as_advanced(GEOTIFF_LIBRARY)
    if(NOT GEOTIFF_LIBRARY)
      message(FATAL_ERROR "Cannot find geotiff library. Please set GEOTIFF_LIBRARY.")
    endif()
  endif()

  # Check if ${GDAL_LIBRARY} has jpeg library
  try_compile(GDAL_HAS_JPEG
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasJpeg.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${JPEG_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
              OUTPUT_VARIABLE OUTPUT)
  if(GDAL_HAS_JPEG)
    message(STATUS "  Testing if GDAL exposes jpeg library symbols      -- yes")
    set(JPEG_LIBRARY ${GDAL_LIBRARY})
  else()
    message(STATUS "  Testing if GDAL exposes jpeg library symbols      -- no")
    find_library(JPEG_LIBRARY NAMES jpeg jpeg_i)
    mark_as_advanced(JPEG_LIBRARY)
    if(NOT JPEG_LIBRARY)
      message(FATAL_ERROR "Cannot find jpeg library. Please set JPEG_LIBRARY.")
    endif()
  endif()

  # Check if ${GDAL_LIBRARY} has ogr library
  try_compile(GDAL_HAS_OGR
              ${CMAKE_CURRENT_BINARY_DIR}/CMake
              ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALHasOGR.cxx
              CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${OGR_INCLUDE_DIRS};${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
              OUTPUT_VARIABLE OUTPUT)
  if(GDAL_HAS_OGR)
    message(STATUS "  Testing if GDAL has OGR                           -- yes")
    set(OGR_LIBRARY ${GDAL_LIBRARY})
  else()
    message(STATUS "  Testing if GDAL has OGR                           -- no")
    message(FATAL_ERROR "OGR MESSAGE: ${OUTPUT}")
    find_library(OGR_LIBRARY ogr PATHS)
    mark_as_advanced(OGR_LIBRARY)
    if(NOT OGR_LIBRARY)
      message(FATAL_ERROR "Cannot find ogr library. Please set OGR_LIBRARY.")
    endif()
  endif()


  # Check for some possible crashes when reading TIFF files, when GDAL embeds geotiff and tiff libraries
  # Known to lead to a crash with the official gdal package available in ubuntu & debian
  include(CheckCXXSourceRunsWithArgs)

  if(NOT WIN32)
  # This test is known to fail with the Debian gdal package
  # On Windows, the try-compile are always run in Debug, leading to runtime issues, so we disable them
  set(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
  set(CMAKE_REQUIRED_LIBRARIES "${GEOTIFF_LIBRARY};${GDAL_LIBRARY}")
  CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestGDALOpen.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otb_logo.tif
            CHECK_GDALOPEN_SYMBOL
            )
  if(NOT CHECK_GDALOPEN_SYMBOL)
    message(WARNING "CHECK_GDALOPEN_SYMBOL test failed : your platform exhibits a gdal/geotiff conflict.\n"
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured\n"
    "embeds its own version of libgeotiff and is ABI incompatible with the system libgeotiff\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols\n"
    "and pointing GEOTIFF_INCLUDE_DIRS to the geotiff headers included in gdal sources" )
  endif()

  # This test is known to fail with the Ubuntu gdal package
  set(CMAKE_REQUIRED_INCLUDES ${GEOTIFF_INCLUDE_DIRS};${TIFF_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_LIBRARIES "${GDAL_LIBRARY};${GEOTIFF_LIBRARY};${TIFF_LIBRARY}")
  CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestXTIFFOpen.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/otb_logo.tif
            CHECK_XTIFFOPEN_SYMBOL
            )
  if(NOT CHECK_XTIFFOPEN_SYMBOL)
    message(WARNING "CHECK_XTIFFOPEN_SYMBOL test failed : your platform exhibits a gdal/geotiff conflict. "
    "Opening a TIF file may generate a crash. This is most probably because the gdal library you configured"
    "embeds its own version of libgeotiff and is ABI incompatible with the system libgeotiff\n"
    "You might consider building GDAL yourself without using --with-hide-internal-symbols "
    "and pointing GEOTIFF_INCLUDE_DIRS to the geotiff headers included in gdal sources" )
  endif()
  endif()

  # This test is known to fail with gdal build with some versions of hdf library
  #  ${OTB_DATA_ROOT}/Input/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf  -> Test KO
  #  ${OTB_DATA_ROOT}/Input/GSSTF_NCEP.2b.2008.12.31.he5  -> Test OK
  #  ${CMAKE_CURRENT_SOURCE_DIR}/CMake/example.hdf -> Test should be KO in some special cases
  if(CHECK_GDAL_BUILT_WITH_HDF)
    set(CMAKE_REQUIRED_INCLUDES ${GDAL_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES "${GDAL_LIBRARY}")
    CHECK_CXX_SOURCE_RUNS_ARGS(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestHDF4Open.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/TestHDF4Open.hdf
            CHECK_HDF4OPEN_SYMBOL
            )
    if(NOT CHECK_HDF4OPEN_SYMBOL)
      message(STATUS "CHECK_HDF4OPEN_SYMBOL test failed : your platform exhibits a problem to read HDF4 files. So the tests with HDF4 will be deactivated" )
    endif()
  endif()

else()

  message(FATAL_ERROR "OTB_USE_EXTERNAL_GDAL=OFF is not implemented. Please set OTB_USE_EXTERNAL_GDAL to ON")

endif()
