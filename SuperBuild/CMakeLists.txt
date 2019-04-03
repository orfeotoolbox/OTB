#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.3.0)

project(OTB-SuperBuild)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Path to additional CMake modules
#MESSAGE(INFO "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/../CMake
    ${CMAKE_MODULE_PATH}
    )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(WARNING "The support of debug build in SuperBuild is incomplete, some dependencies such as NetCDF don't support it.")
endif()

include(ExternalProject)

# Not needed as OTB Superbuild requires c++14 since version 6.2
# include(OTBCheckCpp11Keywords)

include(OTBCheckTargetSystemArch)

set(LINUX FALSE)
if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
  set(LINUX TRUE)
endif()

option(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)
option(BUILD_STATIC_LIBS "Build with static libraries." OFF)
option(BUILD_ALWAYS "Always build or install OTB." ON)
option(WITH_REMOTE_MODULES "Build with specific list of remote modules." OFF)
option(OTB_USE_OPENMP "use openmp" OFF)
set(OTB_ADDITIONAL_CACHE "" CACHE STRING "Additional cmake options for OTB with the syntax (semi-colon used as a separator): -D<var>:<type>=<value>;-D<var>:<type>=<value>")

if(APPLE)
  if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
  endif()
  set(CMAKE_MACOSX_RPATH TRUE)
endif()

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

# TODO BUILD_TESTING is created by default by CTest.cmake, and is ON by default.
option(BUILD_TESTING "Build the testing tree." OFF)

include(CTest)
include(SuperBuild_Macro)

# Configure location where source tar-balls are downloaded
find_path(DOWNLOAD_LOCATION
  NAMES OTBSuperBuild.readme
  HINTS $ENV{DOWNLOAD_LOCATION} "${CMAKE_CURRENT_BINARY_DIR}/Downloads"
  )

# General options
option(OTB_DATA_USE_LARGEINPUT "Use Large inputs images test." OFF)
mark_as_advanced(OTB_DATA_USE_LARGEINPUT)
if(OTB_DATA_USE_LARGEINPUT)
  find_path(OTB_DATA_LARGEINPUT_ROOT
    NAMES OTBData.readme
    HINTS $ENV{OTB_DATA_LARGEINPUT_ROOT}
  )
  mark_as_advanced(OTB_DATA_LARGEINPUT_ROOT)
endif()

option(BUILD_EXAMPLES " Build the OTB Examples directory" OFF)

option(USE_DEFAULT_INSTALL_PREFIX "Install superbuild to default prefix /usr/local for unix" OFF)

#get version of OTB from source_dir/CMakeLists.txt
get_version(
  INPUT_FILE   "${CMAKE_SOURCE_DIR}/../CMakeLists.txt"
  MATCH_STRINGS "OTB_VERSION_MAJOR" "OTB_VERSION_MINOR" "OTB_VERSION_PATCH"
  PREFIX SUPERBUILD
  )

set(SUPERBUILD_OTB_VERSION "${SUPERBUILD_OTB_VERSION_MAJOR}.${SUPERBUILD_OTB_VERSION_MINOR}.${SUPERBUILD_OTB_VERSION_PATCH}")

message("OTB version : ${SUPERBUILD_OTB_VERSION}")

# Special setting for low kernel versions
if(UNIX)
  option(USE_LOW_KERNEL_VERSION "Try to keep the minimal kernel version as low as possible" ON)
endif()


# Default install prefix
get_filename_component(PARENT_OF_BINARY_DIR ${CMAKE_BINARY_DIR} PATH)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT USE_DEFAULT_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX
    "${PARENT_OF_BINARY_DIR}/superbuild_install"
    CACHE
    PATH
    "default install path for superbuild"
    FORCE )
endif()

set(SB_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

file(TO_NATIVE_PATH "${SB_INSTALL_PREFIX}" SB_INSTALL_PREFIX_NATIVE)

# Check if CMAKE_INSTALL_PREFIX is set by default
if(WIN32)
  if(CMAKE_INSTALL_PREFIX STREQUAL "C:/Program Files (x86)/OTB-SuperBuild"
     OR CMAKE_INSTALL_PREFIX STREQUAL "C:/Program Files/OTB-SuperBuild")
    message(WARNING
      "The CMAKE_INSTALL_PREFIX variable seems to be set by default : "
      "${CMAKE_INSTALL_PREFIX}. Be aware that this directory will be used"
      " during the build (even without calling the install target). Please "
      "make sure you want to use this directory as the SuperBuild output.")
  endif()
elseif(UNIX)
  if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
    message(WARNING
      "The CMAKE_INSTALL_PREFIX variable seems to be set by default : "
      "${CMAKE_INSTALL_PREFIX}. Be aware that this directory will be used"
      " during the build (even without calling the install target). Please "
      "make sure you want to use this directory as the SuperBuild output.")
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMake build type is none. Setting it to Release")
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE")
  if(MSVC_VERSION GREATER 1310)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  endif()
endif()

if(DEFINED ENV{CMAKE_PREFIX_PATH})
  set(SB_CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH};${SB_INSTALL_PREFIX}")
else()
  set(SB_CMAKE_PREFIX_PATH "${SB_INSTALL_PREFIX}")
endif()

list(REMOVE_DUPLICATES SB_CMAKE_PREFIX_PATH)

set(SB_CMAKE_CACHE_ARGS)
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_PREFIX_PATH:PATH=${SB_CMAKE_PREFIX_PATH}")

if(APPLE)
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_MACOSX_RPATH:BOOL=${CMAKE_MACOSX_RPATH}")
endif()

if(UNIX)
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_SKIP_BUILD_RPATH:BOOL=${CMAKE_SKIP_BUILD_RPATH}")
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=${CMAKE_BUILD_WITH_INSTALL_RPATH}")
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_RPATH:PATH=${CMAKE_INSTALL_RPATH}")
endif()

foreach(cmake_var
    CMAKE_C_FLAGS
    CMAKE_CXX_FLAGS
    CMAKE_EXE_LINKER_FLAGS
    CMAKE_SHARED_LINKER_FLAGS
    CMAKE_MODULE_LINKER_FLAGS
    CMAKE_STATIC_LINKER_FLAGS
    )
  if(${cmake_var})
    list(APPEND SB_CMAKE_CACHE_ARGS "-D${cmake_var}:STRING=${${cmake_var}}")
    list(APPEND SB_CMAKE_CACHE_ARGS "-D${cmake_var}:STRING=${${cmake_var}}")
  endif()
endforeach()

if(CMAKE_VERBOSE_MAKEFILE)
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON")
endif()

set(SB_CMAKE_ARGS "-G${CMAKE_GENERATOR}")

set(SB_CONFIGURE_ARGS)

list(APPEND SB_CONFIGURE_ARGS "--prefix=${CMAKE_INSTALL_PREFIX}")

if(BUILD_SHARED_LIBS)
  list(APPEND SB_CONFIGURE_ARGS "--enable-static=no")
  list(APPEND SB_CONFIGURE_ARGS "--enable-shared=yes")
else()
  list(APPEND SB_CONFIGURE_ARGS "--enable-static=yes")
  list(APPEND SB_CONFIGURE_ARGS "--enable-shared=no")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  list(APPEND SB_CONFIGURE_ARGS "--enable-debug=yes")
else()
  list(APPEND SB_CONFIGURE_ARGS "--enable-debug=no")
endif()

if(UNIX)
  set(SB_CMAKE_COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${CMAKE_COMMAND} ${SB_CMAKE_ARGS})
  set(SB_ENV_CONFIGURE_CMD env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
  message(STATUS "Environment setup for Configure (SB_ENV_CONFIGURE_CMD): ${SB_ENV_CONFIGURE_CMD}")
else()
  #windows
  set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
endif()

if(CMAKE_C_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "CFLAGS=${CMAKE_C_FLAGS}")
endif()
if(CMAKE_CXX_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "CXXFLAGS=${CMAKE_CXX_FLAGS}")
endif()
if(CMAKE_EXE_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_EXE_LINKER_FLAGS}")
endif()
if(CMAKE_SHARED_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
endif()
if(CMAKE_MODULE_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_MODULE_LINKER_FLAGS}")
endif()

list(REMOVE_DUPLICATES SB_ENV_CONFIGURE_CMD)

message(STATUS "Environment setup for CMake (SB_CMAKE_COMMAND) : ${SB_CMAKE_COMMAND}")

set(FROM_SYSTEM_LIST)
set(FROM_SUPERBUILD_LIST)

#TODO: enforce check from ITK on directory length
# Setup third parties
option(USE_SYSTEM_ZLIB "Use a system build of zlib" OFF)
option(USE_SYSTEM_JPEG "Use a system build of JPEG" OFF)
option(USE_SYSTEM_EXPAT "Use a system build of expat" OFF)
option(USE_SYSTEM_PNG "Use a system build of libpng" OFF)
option(USE_SYSTEM_TIFF "Use a system build of libtiff" OFF)
option(USE_SYSTEM_PROJ  "Use a system build of Proj.4" OFF)
option(USE_SYSTEM_GEOTIFF  "Use a system build of GeoTIFF" OFF)
option(USE_SYSTEM_SQLITE  "Use a system build of sqlite" OFF)
option(USE_SYSTEM_GEOS "Use a system build of GEOS" OFF)
option(USE_SYSTEM_OPENJPEG "Use a system build of OpenJPEG" OFF)
option(USE_SYSTEM_BOOST "Use a system build of Boost" OFF)
option(USE_SYSTEM_LIBKML  "Use a system build of LibKML" OFF)
option(USE_SYSTEM_CURL  "Use a system build of cURL" OFF)
option(USE_SYSTEM_OPENSSL  "Use a system build of OpenSSL" OFF)
option(USE_SYSTEM_GDAL "Use a system build of GDAL" OFF)
option(USE_SYSTEM_FFTW "Use a system build of FFTW" OFF)
option(USE_SYSTEM_ITK "Use a system build of ITK" OFF)
option(USE_SYSTEM_OPENTHREADS  "Use a system build of OpenThreads" OFF)
option(USE_SYSTEM_OSSIM  "Use a system build of OSSIM" OFF)
option(USE_SYSTEM_MUPARSER  "Use a system build of muParser" OFF)
option(USE_SYSTEM_MUPARSERX "Use a system build of  muParserX" OFF)
option(USE_SYSTEM_TINYXML  "Use a system build of TinyXML" OFF)
option(USE_SYSTEM_OPENCV  "Use a system build of OpenCV" OFF)
option(USE_SYSTEM_LIBSVM  "Use a system build of libsvm" OFF)
option(USE_SYSTEM_PCRE "Use a system build of PCRE" ON)
option(USE_SYSTEM_SWIG "Use a system build of swig" ON)
option(USE_SYSTEM_QT "Use a system build of QT5"  OFF)
option(USE_SYSTEM_QWT  "Use a system build of Qwt" OFF)
option(USE_SYSTEM_GLEW  "Use a system build of GLEW" OFF)
option(USE_SYSTEM_GLFW  "Use a system build of glfw" OFF)
option(USE_SYSTEM_GLUT  "Use a system build of glut" OFF)
option(USE_SYSTEM_FREETYPE "Use a system build of freetype" OFF)
option(USE_SYSTEM_SHARK "Use a system build of Shark" OFF)
option(USE_SYSTEM_HDF4 "Use a system build of HDF4" OFF)
option(USE_SYSTEM_HDF5 "Use a system build of HDF5" OFF)
option(USE_SYSTEM_NETCDF "Use a system build of NetCDF" OFF)
option(USE_SYSTEM_GSL "Use a system build of GSL" OFF)

# Call OTB
option(OTB_USE_6S "Enable module 6S in OTB" ON)
option(OTB_USE_CURL "Enable module Curl in OTB" ON)
#RK: disabled libkml. problems on windows msvc
if(MSVC)
  option(OTB_USE_LIBKML "Enable module libKML in OTB" OFF)
else()
  option(OTB_USE_LIBKML "Enable module libKML in OTB" ON)
endif()
option(OTB_USE_LIBSVM "Enable module libSVM in OTB" ON)
option(OTB_USE_MUPARSER "Enable module muparser in OTB" ON)
option(OTB_USE_MUPARSERX "Enable module muparserX in OTB" ON)
option(OTB_USE_OPENCV "Enable module OpenCV in OTB" ON)
option(OTB_USE_QT "Enable module QT5 in OTB" ON)
option(OTB_USE_SIFTFAST "Enable module Siftfast in OTB" ON)
option(OTB_USE_OPENGL "Enable module OpenGL in OTB" ON)
option(OTB_USE_GLEW "Enable module GLEW in OTB" ON)

#Cannot activate GLUT on OSX. See Mantis issue #1194")
# do not activate GLFW by default on osx. see failing test
# https://dash.orfeo-toolbox.org/testDetails.php?test=59844554&build=287120
if(APPLE)
  option(OTB_USE_GLUT "Enable module GLUT in OTB" OFF)
  option(OTB_USE_GLFW "Enable module GLFW in OTB" OFF)
else()
  option(OTB_USE_GLUT "Enable module GLUT in OTB" ON)
  option(OTB_USE_GLFW "Enable module GLFW in OTB" ON)
endif()

option(OTB_USE_SHARK "Enable module Shark in OTB" ON)
include (CMakeDependentOption)
CMAKE_DEPENDENT_OPTION( OTB_USE_QWT "Enable module QWT in OTB" ON "OTB_USE_QT" OFF)
option(OTB_USE_GSL "Enable module GSL in OTB" ON)

option(OTB_USE_MPI "Enable MPI in OTB" OFF)
option(OTB_USE_SPTW "Enable Simple Parallel Tiff Writer in OTB" OFF)

# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT README-OTB-Data
  PATHS
  $ENV{OTB_DATA_ROOT}
  ${OTB-SuperBuild_SOURCE_DIR}/../../OTB-Data)

# SWIG Wrapping
option(OTB_WRAP_PYTHON "Enable python wrappers for OTB applications (requires SWIG)" ON)
option(OTB_WRAP_JAVA "Enable java wrappers for OTB applications (requires SWIG)" OFF)

configure_file(${CMAKE_SOURCE_DIR}/CMake/CTestCustom.cmake.in
  ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)

# do a system checkup
if (NOT EXISTS "${CMAKE_BINARY_DIR}/CMake/SystemCheckup")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/CMake/SystemCheckup")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR}/CMake/SystemCheckup
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/CMake/SystemCheckup
                OUTPUT_QUIET
                ERROR_QUIET)
file(STRINGS "${CMAKE_BINARY_DIR}/CMake/SystemCheckup/SystemCheckupResult.txt" _SYSTEM_CHECKUP_CONTENT)
message(STATUS "|----------------------------------------------------------")
message(STATUS "|----------------- System checkup results -----------------")
message(STATUS "| The following libraries have been found on your system.")
message(STATUS "| You can choose to use them (instead of superbuild versions)")
message(STATUS "| by setting the corresponding option USE_SYSTEM_XXX.")
message(STATUS "|----------------------------------------------------------")
foreach(lib ${_SYSTEM_CHECKUP_CONTENT})
  message(STATUS "| ${lib}")
  message(STATUS "|----------------------------------------------------------")
endforeach()

# Python
# This should set the following variables (and more):
#   -PYTHON_EXECUTABLE
#   -PYTHON_INCLUDE_DIR
#   -PYTHON_LIBRARY
set ( Python_ADDITIONAL_VERSIONS "3;2" )
set ( PythonInterp_FIND_REQUIRED ${OTB_WRAP_PYTHON} )
set ( PythonLibs_FIND_REQUIRED ${OTB_WRAP_PYTHON} )
find_package( PythonInterp )
if ( OTB_WRAP_PYTHON AND ( ${PYTHON_VERSION_MAJOR} EQUAL 2 ) )
    message (WARNING "Python3 not found. There is no longer support of \
    wrapping in python2 in OTB, but it can still be used.")
endif()
set (Python_ADDITIONAL_VERSIONS "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
find_package ( PythonLibs )

include(External_otb)

if(FROM_SYSTEM_LIST)
  list(SORT FROM_SYSTEM_LIST)
  foreach( item ${FROM_SYSTEM_LIST})
    message(STATUS "Using system version of ${item}")
  endforeach()
endif()

if(FROM_SUPERBUILD_LIST)
  list(SORT FROM_SUPERBUILD_LIST)
  foreach( item ${FROM_SUPERBUILD_LIST})
    message(STATUS "Using SuperBuild version of ${item}")
  endforeach()
endif()

message(STATUS "OTB_TARGET_SYSTEM_ARCH=${OTB_TARGET_SYSTEM_ARCH}")
message(STATUS "OTB_TARGET_SYSTEM_ARCH_IS_X64=${OTB_TARGET_SYSTEM_ARCH_IS_X64}")

if(DOWNLOAD_LOCATION)
  message(STATUS "DOWNLOAD_LOCATION=${DOWNLOAD_LOCATION}")
  message(STATUS "Source archives for dependencies will be taken from '${DOWNLOAD_LOCATION}'")
else()
  message(STATUS "DOWNLOAD_LOCATION is not set. We will download all source archives during build!")
endif()

message(STATUS "SuperBuild will be installed to ${CMAKE_INSTALL_PREFIX}")
message(STATUS "To install to a different directory, re-run cmake -DCMAKE_INSTALL_PREFIX=/your/preferred/path")
