cmake_minimum_required(VERSION 2.8.11)

project(OTB-SuperBuild)

# Path to additional CMake modules
#MESSAGE(INFO "${CMAKE_SOURCE_DIR}")
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/../CMake
    ${CMAKE_MODULE_PATH})

include(ExternalProject)

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

# check if superbuild is in OTB source or in stand-alone mode
set(SUPERBUILD_IN_OTB_TREE OFF)
if(EXISTS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt")
  file(STRINGS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt" _CONTENT_CMAKELIST
    REGEX "project\\\(OTB\\\)")
  if(NOT "${_CONTENT_CMAKELIST}" STREQUAL "")
    set(SUPERBUILD_IN_OTB_TREE ON)
    # parse the version number
    file(STRINGS "${CMAKE_SOURCE_DIR}/../CMakeLists.txt" _FULL_CONTENT_CMAKELIST)
    string(REGEX REPLACE ".*set\\(OTB_VERSION_MAJOR \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_MAJOR "${_FULL_CONTENT_CMAKELIST}")
    string(REGEX REPLACE ".*set\\(OTB_VERSION_MINOR \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_MINOR "${_FULL_CONTENT_CMAKELIST}")
    string(REGEX REPLACE ".*set\\(OTB_VERSION_PATCH \"([0-9]+)\"\\).*" "\\1" SUPERBUILD_OTB_VERSION_PATCH "${_FULL_CONTENT_CMAKELIST}")
    set(SUPERBUILD_OTB_VERSION "${SUPERBUILD_OTB_VERSION_MAJOR}.${SUPERBUILD_OTB_VERSION_MINOR}.${SUPERBUILD_OTB_VERSION_PATCH}")
    message(STATUS "Found OTB version : ${SUPERBUILD_OTB_VERSION}")
  endif()
endif()
message(STATUS "Check if superbuild is in OTB tree : ${SUPERBUILD_IN_OTB_TREE}")

set(SB_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(SB_BUILD_SHARED_LIBS ON)

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


if(WIN32)
  if(MSVC)
    set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
     set(SB_BUILD_SHARED_LIBS OFF)
    set(SB_PRIOR_INCLUDE_FLAGS "/I ${CMAKE_INSTALL_PREFIX}/include")
  endif()
else(UNIX)
  if(APPLE)
    set(LDLIBVAR DYLD_LIBRARY_PATH)
    set(SB_CMAKE_COMMAND ${CMAKE_COMMAND})
    set(SB_ENV_CONFIGURE_CMD env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
  else()
    set(LDLIBVAR LD_LIBRARY_PATH)
    set(SB_ENV_CONFIGURE_CMD env ${LDLIBVAR}=${SB_INSTALL_PREFIX}/lib:$ENV{${LDLIBVAR}} CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
    set(SB_CMAKE_COMMAND env ${LDLIBVAR}=${CMAKE_INSTALL_PREFIX}/lib:$ENV{${LDLIBVAR}}  CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${CMAKE_COMMAND})
    message(STATUS "Environment setup for Configure : ${SB_ENV_CONFIGURE_CMD}")
  endif()
  set(SB_PRIOR_INCLUDE_FLAGS "-I${CMAKE_INSTALL_PREFIX}/include")
endif()

# Call external projects

#TODO: enforce check from ITK on directory length
# Setup third parties
SETUP_SYSTEM_LIBRARY(PROJECT ZLIB DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT JPEG DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT EXPAT DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT PNG DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT TIFF DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT PROJ DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT GEOTIFF DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT SQLITE DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT GEOS DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT OPENJPEG DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT BOOST DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT LIBKML DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT CURL DEFAULT ON)
if(WIN32)
  SETUP_SYSTEM_LIBRARY(PROJECT OPENSSL DEFAULT OFF)
else()
  SETUP_SYSTEM_LIBRARY(PROJECT OPENSSL DEFAULT ON)
endif()
SETUP_SYSTEM_LIBRARY(PROJECT GDAL DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT FFTW DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT ITK DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT OPENTHREADS DEFAULT OFF)
if(WIN32)
  SETUP_SYSTEM_LIBRARY(PROJECT OSSIM DEFAULT ON)
else()
  SETUP_SYSTEM_LIBRARY(PROJECT OSSIM DEFAULT OFF)
endif()
SETUP_SYSTEM_LIBRARY(PROJECT MUPARSER DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT MUPARSERX DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT TINYXML DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT OPENCV DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT LIBSVM DEFAULT OFF)
SETUP_SYSTEM_LIBRARY(PROJECT PCRE DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT SWIG DEFAULT ON)

SETUP_SYSTEM_LIBRARY(PROJECT QT4 DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT QWT DEFAULT ON)

SETUP_SYSTEM_LIBRARY(PROJECT GLEW DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT GLFW DEFAULT ON)
SETUP_SYSTEM_LIBRARY(PROJECT GLUT DEFAULT ON)

# Call OTB
option(OTB_USE_6S "Enable module 6S in OTB" ON)
option(OTB_USE_CURL "Enable module Curl in OTB" ON)
option(OTB_USE_LIBKML "Enable module libKML in OTB" ON)
option(OTB_USE_LIBSVM "Enable module libSVM in OTB" ON)
option(OTB_USE_MAPNIK "Enable module Mapnik in OTB" OFF)
option(OTB_USE_MUPARSER "Enable module muparser in OTB" ON)
option(OTB_USE_MUPARSERX "Enable module muparserX in OTB" ON)
option(OTB_USE_OPENCV "Enable module OpenCV in OTB" ON)
option(OTB_USE_OPENJPEG "Enable module openjpeg in OTB" OFF)
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

option(GENERATE_XDK "Generate an .run with all dependencies. supported only on Linux" OFF)

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

#check compiler is gnu?
if(UNIX AND NOT APPLE)
  if(GENERATE_XDK)
    include(GenerateXDK)
  endif()

  include(External_package)

endif(UNIX AND NOT APPLE)
