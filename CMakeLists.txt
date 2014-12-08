cmake_minimum_required(VERSION 2.8.3)


project(OTB-SuperBuild)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})


include(ExternalProject)

# Call external projects
include(External_jpeg)
include(External_SQLite)
include(External_zlib)
include(External_libtiff)
include(External_proj4)
include(External_geotiff)
include(External_geos)
include(External_ITK)
include(External_ossim)
include(External_OpenJpeg)
include(External_GDAL)

# TODO

# Call OTB
include(External_OTB)
