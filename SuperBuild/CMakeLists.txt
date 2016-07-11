cmake_minimum_required(VERSION 2.8.11)

project(OTB-SuperBuild)

# Path to additional CMake modules
#MESSAGE(INFO "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/../CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)

option(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)

set(OTB_ADDITIONAL_CACHE "" CACHE STRING "Additional cmake option for OTB -DVAR:TYPE=VALUE ...")

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

# Setup location where source tar-balls are downloaded
#set (install_location "${CMAKE_CURRENT_BINARY_DIR}/install")
set (DOWNLOAD_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/Downloads"
     CACHE PATH "Location where source tar-balls are (to be) downloaded.")
mark_as_advanced(DOWNLOAD_LOCATION)

# General options
option(ENABLE_OTB_LARGE_INPUTS " Use the OTB-LargeInputs for testing" OFF)
option(BUILD_EXAMPLES " Build the OTB Examples directory" OFF)
option(ENABLE_MONTEVERDI " Build Monteverdi on top of OTB" OFF)

if(EXISTS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt")
  file(STRINGS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt" _CONTENT_CMAKELIST
    REGEX "project\\\(OTB\\\)")
  if(NOT "${_CONTENT_CMAKELIST}" STREQUAL "")
    # parse the version number
    file(STRINGS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt" _FULL_CONTENT_CMAKELIST)
    string(REGEX REPLACE ".*set\\(OTB_VERSION_MAJOR \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_MAJOR "${_FULL_CONTENT_CMAKELIST}")
    string(REGEX REPLACE ".*set\\(OTB_VERSION_MINOR \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_MINOR "${_FULL_CONTENT_CMAKELIST}")
    string(REGEX REPLACE ".*set\\(OTB_VERSION_PATCH \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_PATCH "${_FULL_CONTENT_CMAKELIST}")
    set(SUPERBUILD_OTB_VERSION "${SUPERBUILD_OTB_VERSION_MAJOR}.${SUPERBUILD_OTB_VERSION_MINOR}.${SUPERBUILD_OTB_VERSION_PATCH}")
    message(STATUS "OTB version : ${SUPERBUILD_OTB_VERSION}")
  endif()
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
  set(BUILD_SHARED_LIBS OFF)
  if(CMAKE_MAJOR_VERSION GREATER 2 AND CMAKE_MINOR_VERSION GREATER 2)
    set(BUILD_SHARED_LIBS ON)
  else()
    message(WARNING "BUILD_SHARED_LIBS is set to OFF with CMake < 3.3")
  endif()
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /Ox /FC /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE")
  if(MSVC_VERSION GREATER 1310)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  endif()
endif()

if(DEFINED ENV{CMAKE_PREFIX_PATH})
  set(SB_CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH};${SB_INSTALL_PREFIX}")
else()
  set(SB_CMAKE_PREFIX_PATH "${SB_INSTALL_PREFIX}")
endif()

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

#RK: not sure we need to pass CC and CXX for all autoconf build
if(UNIX)
  if(APPLE)
    set(SB_ENV_CONFIGURE_CMD env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
  else()
    set(LDLIBVAR LD_LIBRARY_PATH)
    #Do we really need to pass LD_LIBRARY_PATH here?
    set(SB_ENV_CONFIGURE_CMD env ${LDLIBVAR}=${SB_INSTALL_PREFIX}/lib:$ENV{${LDLIBVAR}} CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
    #RK: check with GP
    # set(SB_CMAKE_COMMAND env ${LDLIBVAR}=${CMAKE_INSTALL_PREFIX}/lib:$ENV{${LDLIBVAR}}  CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${CMAKE_COMMAND})
    message(STATUS "Environment setup for Configure : ${SB_ENV_CONFIGURE_CMD}")
  endif()
endif()

set(SB_CMAKE_ARGS)
list(APPEND SB_CMAKE_ARGS "-G${CMAKE_GENERATOR}")

set(SB_CMAKE_COMMAND ${CMAKE_COMMAND} ${SB_CMAKE_ARGS})

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
option(USE_SYSTEM_MUPARSER  "Use a system build of muParser" OFF)
option(USE_SYSTEM_MUPARSERX "Use a system build of  muParserX" OFF)
option(USE_SYSTEM_TINYXML  "Use a system build of TinyXML" OFF)
option(USE_SYSTEM_OPENCV  "Use a system build of OpenCV" OFF)
option(USE_SYSTEM_LIBSVM  "Use a system build of libsvm" OFF)
option(USE_SYSTEM_PCRE "Use a system build of PCRE" ON)
option(USE_SYSTEM_SWIG "Use a system build of swig" ON)
option(USE_SYSTEM_QT4 "Use a system build of QT4"  OFF)
option(USE_SYSTEM_QWT  "Use a system build of Qwt" OFF)
option(USE_SYSTEM_GLEW  "Use a system build of GLEW" OFF)
option(USE_SYSTEM_GLFW  "Use a system build of glfw" OFF)
option(USE_SYSTEM_GLUT  "Use a system build of glut" OFF)
option(USE_SYSTEM_FREETYPE "Use a system build of freetype" OFF)

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
option(OTB_USE_MAPNIK "Enable module Mapnik in OTB" OFF)
option(OTB_USE_MUPARSER "Enable module muparser in OTB" ON)
option(OTB_USE_MUPARSERX "Enable module muparserX in OTB" ON)
option(OTB_USE_OPENCV "Enable module OpenCV in OTB" ON)
option(OTB_USE_QT4 "Enable module QT4 in OTB" ON)
option(OTB_USE_SIFTFAST "Enable module Siftfast in OTB" ON)
option(OTB_USE_OPENGL "Enable module OpenGL in OTB" OFF)
option(OTB_USE_GLEW "Enable module GLEW in OTB" OFF)
option(OTB_USE_GLFW "Enable module GLFW in OTB" OFF)
option(OTB_USE_GLUT "Enable module GLUT in OTB" OFF)

if(${ENABLE_MONTEVERDI})
  set(OTB_USE_OPENGL ON)
  set(OTB_USE_GLEW ON)
endif()

# set OTB_DATA_ROOT to run test
find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB-SuperBuild_SOURCE_DIR}/../../OTB-Data)

if(ENABLE_OTB_LARGE_INPUTS)
  find_path(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
endif()

# SWIG Wrapping
option(OTB_WRAP_PYTHON "Enable python wrappers for OTB applications (requires SWIG)" OFF)
option(OTB_WRAP_JAVA "Enable java wrappers for OTB applications (requires SWIG)" OFF)

option(GENERATE_PACKAGE "Generate a portable .run package of OTB (only Unix)." OFF)
option(GENERATE_XDK "Generate an XDK. This files provides all headers and libraries to build and or develop OTB (only Unix)." OFF)

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

# Main external projet : dependencies are included (if needed) in each external project
if(ENABLE_MONTEVERDI)
  include(External_mvd)
else()
  include(External_otb)
endif()

if(UNIX OR MSVC)
  add_subdirectory(Packaging)
endif()

if(FROM_SYSTEM_LIST)
  list(SORT FROM_SYSTEM_LIST)
foreach( item ${FROM_SYSTEM_LIST})
  message(STATUS "  Using system version of ${item}")
endforeach()
endif()

if(FROM_SUPERBUILD_LIST)
  list(SORT FROM_SUPERBUILD_LIST)
foreach( item ${FROM_SUPERBUILD_LIST})
  message(STATUS "  Using SuperBuild version of ${item}")
endforeach()
endif()