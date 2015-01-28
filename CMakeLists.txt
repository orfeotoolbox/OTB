cmake_minimum_required(VERSION 2.8.3)


project(OTB-SuperBuild)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)
include(CTest)

# Call external projects

if(MSVC)

##TODO: Add check from ITK for long directory names

  set(SB_MAKE_CMD "nmake")
  set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
  set(SB_MSVC_VER 1600)
  if(MSVC10)
    set(SB_MSVC_VER 1600)
  else(MSVC80)
    set(SB_MSVC_VER 1400)
  endif()
else(UNIX)
  set(SB_MAKE_CMD "make")
  set(SB_CMAKE_COMMAND env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib ${CMAKE_COMMAND})
endif()

include(External_jpeg)
include(External_SQLite)
include(External_zlib)
include(External_libtiff)
include(External_libpng)
include(External_proj4)
include(External_geotiff)
include(External_geos)
include(External_fftw)
include(External_curl)
include(External_OpenJpeg)
include(External_expat)
include(External_boost)
include(External_libKML)
include(External_GDAL)
include(External_muParser)
include(External_muParserX)
include(External_TinyXML)
include(External_ITK)
include(External_OpenThreads)
include(External_ossim)
include(External_OpenCV)

include(External_qt4)

# TODO

# Call OTB

# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB-SuperBuild_SOURCE_DIR}/../OTB-Data)

include(External_OTB)

option(BUILD_ICE_APPLICATION "Build the Ice application (requires glfw and glut)" OFF)

include(External_Ice)


