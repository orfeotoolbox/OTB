cmake_minimum_required(VERSION 2.8.3)


project(OTB-SuperBuild)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)
include(CTest)

# General options
option(ENABLE_QT4 " Use QT4 in the SuperBuild" ON)
option(ENABLE_OTB_LARGE_INPUTS " Use the OTB-LargeInputs for testing" OFF)


if(WIN32)
  if(MSVC)
    set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
  endif()      
else(UNIX)
  set(SB_CMAKE_COMMAND env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib  ${CMAKE_COMMAND})
endif()
# Call external projects

#TODO: enforce check from ITK on directory length

include(External_zlib)
include(External_jpeg)
include(External_expat)
include(External_libpng)
include(External_libtiff)
include(External_proj4)
include(External_geotiff)
include(External_SQLite)
include(External_geos)
include(External_OpenJpeg)
include(External_boost)
include(External_libKML)
include(External_curl)
include(External_GDAL)
include(External_fftw)
include(External_ITK)
include(External_OpenThreads)
include(External_ossim)
include(External_muParser)
include(External_muParserX)
include(External_TinyXML)
include(External_OpenCV)

if(ENABLE_QT4)
  include(External_qt4)
endif()

# TODO

# Call OTB

# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB-SuperBuild_SOURCE_DIR}/../OTB-Data)

if(ENABLE_OTB_LARGE_INPUTS)
  find_path(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
endif()

include(External_OTB)

option(BUILD_ICE_APPLICATION "Build the Ice application (requires glfw and glut)" OFF)

include(External_Ice)

if(ENABLE_QT4)
  include(External_Monteverdi2)
endif()

#include(External_Monteverdi)