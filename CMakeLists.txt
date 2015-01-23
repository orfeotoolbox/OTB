cmake_minimum_required(VERSION 2.8.3)


project(OTB-SuperBuild)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)
include(CTest)

# Call external projects


include(External_jpeg)
include(External_SQLite)
include(External_zlib)
include(External_libtiff)
include(External_proj4)
include(External_geotiff)
include(External_geos)
include(External_fftw)
include(External_OpenJpeg)
include(External_muParser)
include(External_muParserX)
include(External_TinyXML)
include(External_expat)
include(External_GDAL)
include(External_ITK)
include(External_ossim)
include(External_libKML)
include(External_openthread)
include(External_boost)
include(External_OpenCV)
include(External_curl)

# TODO

# Call OTB

# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB-SuperBuild_SOURCE_DIR}/../OTB-Data)

include(External_OTB)

# DEBUG:
# impose to build OTB after all dependencies
message(STATUS "OTB DEPENDENCIES ${OTB_DEPS}")

#add_dependencies(OTB ${OTB_DEPS})

option(BUILD_ICE_APPLICATION "Build the Ice application (requires glfw and glut)" OFF)

include(External_Ice)


