find_package ( GDAL REQUIRED )
mark_as_advanced(GDAL_INCLUDE_DIR)
mark_as_advanced(GDAL_LIBRARY)
mark_as_advanced(GDAL_CONFIG)

if(NOT GDAL_FOUND)
  message(FATAL_ERROR "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
endif()

#Check if GDAL is compatible with OTB using a bunch of cmake try_run and try_compile. (Default value is ON.)
set(GDAL_CONFIG_CHECKING ON CACHE BOOL "Tests to check gdal config." FORCE)
mark_as_advanced(GDAL_CONFIG_CHECKING)

if(NOT GDAL_CONFIG_CHECKING)
  return()
endif()

set(MIN_MAJOR_VERSION 1)
set(MIN_MINOR_VERSION 10)

# Ensure that the temporary dir always exists before starting tests
if(NOT EXISTS ${TEMP})
  FILE(MAKE_DIRECTORY ${TEMP})
endif()


#------------------- Helper Macro ---------------------
macro(gdal_try_run msg_type var source_file)
message(STATUS "Performing Test ${var}")
set(${var})
try_run(RUN_${var} COMPILE_${var} ${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/${source_file}
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-w" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT_${var}
RUN_OUTPUT_VARIABLE RUN_OUTPUT_${var}
ARGS ${ARGN}
)

if(NOT COMPILE_${var})
  message(FATAL_ERROR "Compiling Test ${var} - Failed \n
COMPILE_OUTPUT_${var}: '${COMPILE_OUTPUT_${var}}'")
endif()
if(RUN_${var})
  set(${var} FALSE)
  #if msg_type is STATUS (ie "okay" if test is failing),
  #then we don't need to give an run-output and exit status.
  if("${msg_type}" STREQUAL "STATUS")
    message(${msg_type} "Performing Test ${var} - Failed")
  else()
    message(${msg_type} "Performing Test ${var} - Failed \n
            Exit status: '${RUN_${var}}' \n
            RUN_OUTPUT_${var}: '${RUN_OUTPUT_${var}}'")
  endif()
else()
  set(${var} TRUE)
  message(STATUS "Performing Test ${var} - Success")
endif()
unset(RUN_OUTPUT_${var})
unset(COMPILE_OUTPUT_${var})
unset(COMPILE_${var})
endmacro()

#------------------- TESTS ---------------------
# Version of GDAL
gdal_try_run(FATAL_ERROR GDAL_VERSION gdalVersionTest.cxx ${TEMP}/gdalVersion.txt ${MIN_MAJOR_VERSION} ${MIN_MINOR_VERSION})

if(EXISTS "${TEMP}/gdalVersion.txt")
  file(READ "${TEMP}/gdalVersion.txt" _GDAL_VERSION_STRING)
  #can't we use GDAL_VERSION_NUM ?
  string(SUBSTRING ${_GDAL_VERSION_STRING} 0 2 VER2)
  if("${VER2}" STREQUAL "2.")
    set(OTB_USE_GDAL_20 true CACHE INTERNAL "True if GDAL >= 2.0.0 has been detected" FORCE )
  else()
    set(OTB_USE_GDAL_20 false CACHE INTERNAL "True if GDAL >= 2.0.0 has been detected" FORCE )
  endif()
  set(GDAL_VERSION_STRING "${_GDAL_VERSION_STRING}" CACHE INTERNAL "" FORCE)
else()
  message(FATAL_ERROR "${TEMP}/gdalVersion.txt does not exist. Cannot continue.")
endif()

#check OGR
#gdal_try_run(FATAL_ERROR GDAL_HAS_OGR gdalOGRTest.cxx)
try_compile(COMPILE_GDAL_HAS_OGR ${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOGRTest.cxx
CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-w" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}"
OUTPUT_VARIABLE COMPILE_OUTPUT_GDAL_HAS_OGR
)
if(NOT COMPILE_GDAL_HAS_OGR)
 message(FATAL_ERROR "Performing Test COMPILE_GDAL_HAS_OGR - Failed\n COMPILE_OUTPUT:${COMPILE_OUTPUT_GDAL_HAS_OGR}\n")
  else()
 message(STATUS "Performing Test COMPILE_GDAL_HAS_OGR - Success")
endif()

# check formats TIFF, GeoTIFF, JPEG, JPEG2000, HDF5
# Note : exact format names can be found here http://www.gdal.org/formats_list.html

gdal_try_run(STATUS GDAL_FORMATS_LIST gdalFormatsListTest.c ${TEMP}/gdalFormatsList.csv)

gdal_try_run(FATAL_ERROR GDAL_HAS_JPEG gdalFormatsTest.c JPEG)

gdal_try_run(FATAL_ERROR GDAL_HAS_GTiff gdalFormatsTest.c GTiff)

gdal_try_run(FATAL_ERROR GDAL_CAN_CREATE_GTiff gdalCreateTest.cxx GTiff ${TEMP}/testImage.gtif )

gdal_try_run(FATAL_ERROR GDAL_CAN_CREATE_GTiff_BIGTIFF gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.bigtif GTiff "BIGTIFF=YES")

gdal_try_run(FATAL_ERROR GDAL_CAN_CREATE_JPEG gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.jpeg JPEG)

set(JPEG2000_DRIVER_USED)
gdal_try_run(STATUS GDAL_HAS_JP2OpenJPEG gdalFormatsTest.c JP2OpenJPEG)
if (GDAL_HAS_JP2OpenJPEG)
  set(JPEG2000_DRIVER_USED "OpenJPEG")
  gdal_try_run(STATUS GDAL_CAN_CREATE_JP2OpenJPEG gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2OpenJPEG)
endif()

gdal_try_run(STATUS GDAL_HAS_JP2KAK gdalFormatsTest.c JP2KAK)
if (GDAL_HAS_JP2KAK)
  set(JPEG2000_DRIVER_USED "Kakadu")
  gdal_try_run(STATUS GDAL_CAN_CREATE_JP2KAK gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2KAK)
endif()

gdal_try_run(STATUS GDAL_HAS_JP2ECW gdalFormatsTest.c JP2ECW)
if (GDAL_HAS_JP2ECW)
  set(JPEG2000_DRIVER_USED "ECW")
  gdal_try_run(STATUS GDAL_CAN_CREATE_JP2ECW gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2ECW)
endif()

gdal_try_run(STATUS GDAL_HAS_JPEG2000 gdalFormatsTest.c JPEG2000)
if (GDAL_HAS_JPEG2000)
    set(JPEG2000_DRIVER_USED "JPEG2000")
  gdal_try_run(STATUS GDAL_CAN_CREATE_JPEG2000 gdalCreateCopyTest.cxx ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JPEG2000)
endif()

gdal_try_run(STATUS GDAL_HAS_HDF5 gdalFormatsTest.c HDF5)
gdal_try_run(STATUS GDAL_HAS_HDF4 gdalFormatsTest.c HDF4)

#check some vector formats
gdal_try_run(FATAL_ERROR GDAL_HAS_SQLite gdalFormatsTest.c SQLite)
gdal_try_run(FATAL_ERROR GDAL_HAS_VRT gdalFormatsTest.c VRT)
gdal_try_run(FATAL_ERROR GDAL_HAS_KML gdalFormatsTest.c KML)
gdal_try_run(STATUS GDAL_HAS_LIBKML gdalFormatsTest.c LIBKML)

#------------------- TESTS (END)---------------------

#FOR UNIX SYSTEMS ONLY
if(UNIX)
  # Prepare bash script
  configure_file(${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest.sh.in ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh @ONLY)
  execute_process(COMMAND chmod u+x ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)
  execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)
  gdal_try_run(FATAL_ERROR GDAL_SYMBOLS gdalSymbolsTest.cxx ARGS ${TEMP}/gdalSymbols.txt)
endif()

if (NOT JPEG2000_DRIVER_USED)
  message(STATUS "No Jpeg2000 gdal driver found. Supported drivers are : OpenJpeg, Kakadu, ECW.")
elseif("${JPEG2000_DRIVER_USED}" STREQUAL "JPEG2000")
  message(STATUS "Jasper JPEG2000 gdal driver is available. This will be skipped by OTB. supported drivers are OpenJPEG, ECW, Kakadu")
  unset(JPEG2000_DRIVER_USED)
else()
  message(STATUS "GDAL driver used for JPEG2000 dataset is '${JPEG2000_DRIVER_USED}'")
endif()