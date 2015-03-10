cmake_minimum_required(VERSION 2.8.3)


project(OTB-SuperBuild)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)
include(CTest)
include(SuperBuild_Macro)

# General options
option(ENABLE_QT4 " Use QT4 in the SuperBuild" ON)
option(ENABLE_OTB_LARGE_INPUTS " Use the OTB-LargeInputs for testing" OFF)
option(BUILD_ICE "Clone and build ICE" OFF)
option(BUILD_MONTEVERDI2 "Clone and build Monteverdi2" OFF)
option(BUILD_MONTEVERDI "Clone and build Monteverdi" OFF)

set(SB_BUILD_SHARED_LIBS ON)

if(WIN32)
  if(MSVC)
    set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
     set(SB_BUILD_SHARED_LIBS OFF)
  endif()      
else(UNIX)
  if(APPLE)
    set(LDLIBVAR DYLD_LIBRARY_PATH)
  else()
    set(LDLIBVAR LD_LIBRARY_PATH)
  endif()
  set(SB_CMAKE_COMMAND env ${LDLIBVAR}=${CMAKE_INSTALL_PREFIX}/lib  ${CMAKE_COMMAND})
endif()
# Call external projects

#TODO: enforce check from ITK on directory length

include(External_zlib)
include(External_jpeg)
include(External_expat)
include(External_png)
include(External_tiff)
include(External_proj)
include(External_geotiff)
include(External_sqlite)
include(External_geos)
include(External_openjpeg)
include(External_boost)
include(External_libkml)
include(External_curl)
include(External_gdal)
include(External_fftw)
include(External_itk)
include(External_openthreads)
include(External_ossim)
include(External_muparser)
include(External_muparserx)
include(External_tinyxml)
include(External_opencv)
include(External_pcre)
include(External_swig)

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

# SWIG Wrapping
option(OTB_WRAP_PYTHON "Enable python wrappers for OTB applications (requires SWIG)" OFF)
option(OTB_WRAP_JAVA "Enable java wrappers for OTB applications (requires SWIG)" OFF)

include(External_otb)

if(BUILD_ICE)
  option(BUILD_ICE_APPLICATION "Build the Ice application (requires glfw and glut)" OFF)
  include(External_ice)
endif()

if(BUILD_MONTEVERDI2)
  include(External_monteverdi2)
endif()

if(BUILD_MONTEVERDI)
  include(External_monteverdi)
endif()
