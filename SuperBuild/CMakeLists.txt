#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

cmake_minimum_required(VERSION 3.10.2)

project(OTB-SuperBuild)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/../CMake
    ${CMAKE_MODULE_PATH}
    )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(WARNING "The support of debug build in SuperBuild is incomplete, some dependencies such as NetCDF don't support it.")
endif()

include(ExternalProject)

include(OTBCheckTargetSystemArch)

set(LINUX FALSE)
if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
  set(LINUX TRUE)
endif()

option(BUILD_SHARED_LIBS "Build Superbuild with shared libraries." ON)
mark_as_advanced(BUILD_SHARED_LIBS)
option(BUILD_GEOS_STATIC_LIBS "Build GEOS with static libraries." OFF)
mark_as_advanced(BUILD_GEOS_STATIC_LIBS)
option(BUILD_ALWAYS "Always build or install OTB." ON)
option(WITH_REMOTE_MODULES "Build with specific list of remote modules." ${OTB_BUILD_RemoteModules})
option(OTB_USE_OPENMP "use openmp" OFF)
set(OTB_ADDITIONAL_CACHE "" CACHE STRING "Additional cmake options for OTB with the syntax (semi-colon used as a separator): -D<var>:<type>=<value>;-D<var>:<type>=<value>")

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

if(NOT DOWNLOAD_LOCATION)
  set(DOWNLOAD_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/Downloads")
endif()

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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE /D_ENABLE_EXTENDED_ALIGNED_STORAGE")
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

# This is needed for gdal 2.4 in order to find OpenJPEG
list(APPEND SB_ENV_CONFIGURE_CMD "PKG_CONFIG_PATH=${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")

list(REMOVE_DUPLICATES SB_ENV_CONFIGURE_CMD)

message(STATUS "Environment setup for CMake (SB_CMAKE_COMMAND) : ${SB_CMAKE_COMMAND}")

if(WIN32)
  # Force all dll in the bin folder
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin)
endif()

set(FROM_SYSTEM_LIST)
set(FROM_SUPERBUILD_LIST)
set(USE_SYSTEM_PCRE ON)
set(USE_SYSTEM_SWIG ON)
option(USE_SYSTEM_OPENSSL OFF)

# Modular building options
option(OTB_BUILD_Core "Enable building only core modules" ON)
option(OTB_BUILD_FeaturesExtraction "Enable building image processing module" OFF)
option(OTB_BUILD_Hyperspectral "Enable building Hyperspectral module" OFF)
option(OTB_BUILD_Learning "Enable building of the machine learning module" OFF)
option(OTB_BUILD_Miscellaneous "Enable building of the miscellaneous algorithms module" OFF)
option(OTB_BUILD_SAR "Enable building of SAR module" OFF)
option(OTB_BUILD_Segmentation "Enable building of segmentation module" OFF)
option(OTB_BUILD_StereoProcessing "Enable building of Stereo module" OFF)
option(OTB_BUILD_RemoteModules "Enable building of Remote modules" OFF)

# Dependencies options
option(OTB_USE_6S "Enable module 6S in OTB" ON)
option(OTB_USE_CURL "Enable module Curl in OTB" ON)
option(OTB_USE_SIFTFAST "Enable module Siftfast in OTB" ON)
option(OTB_USE_GSL "Enable module GSL in OTB" ON)
option(OTB_USE_LIBKML "Enable module libKML in OTB" OFF)
#option() does not change the value in cache so we have to set the option via the set CACHE FORCE method
if(OTB_BUILD_Learning MATCHES ON)
  set(OTB_USE_LIBSVM ON CACHE BOOL "Enable module LibSVM in OTB" FORCE)
  set(OTB_USE_OPENCV ON CACHE BOOL "Enable module OPENCV in OTB" FORCE)
  set(OTB_USE_SHARK ON CACHE BOOL "Enable module Shark in OTB" FORCE)
else()
  set(OTB_USE_LIBSVM OFF CACHE BOOL "Enable module LibSVM in OTB" FORCE)
  set(OTB_USE_OPENCV OFF CACHE BOOL "Enable module OPENCV in OTB" FORCE)
  set(OTB_USE_SHARK OFF CACHE BOOL "Enable module Shark in OTB" FORCE)
endif()
if(OTB_BUILD_FeaturesExtraction MATCHES ON)
  set(OTB_USE_MUPARSER ON CACHE BOOL "Enable module MuParser in OTB" FORCE)
  set(OTB_USE_MUPARSERX ON CACHE BOOL "Enable module MuParserX in OTB" FORCE)
else()
  set(OTB_USE_MUPARSER OFF CACHE BOOL "Enable module MuParser in OTB" FORCE)
  set(OTB_USE_MUPARSERX OFF CACHE BOOL "Enable module MuParserX in OTB" FORCE)
endif()


# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT
  NAMES README-OTB-Data
  HINTS ${OTB-SuperBuild_SOURCE_DIR}/../Data
  )

# SWIG Wrapping
option(OTB_WRAP_PYTHON "Enable python wrappers for OTB applications (requires SWIG)" ON)
option(OTB_WRAP_QGIS "Enable QGIS descriptor compilation" ON)

configure_file(${CMAKE_SOURCE_DIR}/CMake/CTestCustom.cmake.in
  ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)

# Python
find_package( Python REQUIRED COMPONENTS Interpreter Development)
if(Python_FOUND)
  message(STATUS "Python Library directory : ${Python_LIBRARY_DIRS}")
  message(STATUS "Python Headers directory : ${Python_INCLUDE_DIRS}")
endif()
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

message(STATUS "OTB_TARGET_SYSTEM_ARCH_IS_X64  = ${OTB_TARGET_SYSTEM_ARCH_IS_X64}")
message(STATUS "DOWNLOAD_LOCATION              = ${DOWNLOAD_LOCATION}")
message(STATUS "CMAKE_INSTALL_PREFIX           = ${CMAKE_INSTALL_PREFIX}")
message(STATUS "SUPERBUILD_OTB_VERSION         = ${SUPERBUILD_OTB_VERSION}")
message("\nCMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator\n")
