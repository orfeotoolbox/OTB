PROJECT(Monteverdi2)

cmake_minimum_required(VERSION 2.6)

#----------------------------------------------------------------------------
# Modules
set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH} )
include( i18n_qt4 )

#----------------------------------------------------------------------------
# ensure WIN32 definition
if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

#----------------------------------------------------------------------------
# ensure DEBUG definitions
#if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
#  add_definitions( -D_DEBUG )
#endif()
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG" )

#----------------------------------------------------------------------------
# Monteverdi2 version number.  An even minor number corresponds to releases.
set(Monteverdi2_VERSION_MAJOR "0")
set(Monteverdi2_VERSION_MINOR "3")
set(Monteverdi2_VERSION_PATCH "0")

#----------------------------------------------------------------------------
# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(Monteverdi2_VERSION_STRING "${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}.${Monteverdi2_VERSION_PATCH}")

#----------------------------------------------------------------------------
set(Monteverdi2_FLOATING_TYPE "float" CACHE STRING "The type of data in which input images are read (double/float)." )

#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
  add_definitions(-DNOMINMAX -DOSSIM_STATIC)
endif(WIN32)

#----------------------------------------------------------------------------
# Orfeo ToolBox
find_package(OTB REQUIRED)
if(OTB_FOUND)
  include(${OTB_USE_FILE})
else(OTB_FOUND)
  message(FATAL_ERROR "OTB not found. Please set OTB_DIR")
endif(OTB_FOUND)

#----------------------------------------------------------------------------
# Qt4
find_package( OpenGL REQUIRED )
find_package( Qt4 REQUIRED QtCore QtGui QtOpenGL QtMain QtXml )
include( ${QT_USE_FILE} )

#----------------------------------------------------------------------------
# Include OTB CMake modules path.
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${OTB_SOURCE_DIR}/CMake")

#----------------------------------------------------------------------------
find_package(Mercurial QUIET)
if(Mercurial_FOUND)
  Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} Monteverdi2)
  message(STATUS "Mercurial version is ${Mercurial_VERSION_HG}.")
  message(STATUS "Repository revision is ${Monteverdi2_WC_REVISION}.")
  set(Monteverdi2_WC_REVISION ${Monteverdi2_WC_REVISION} CACHE STRING "Repository version." FORCE)
  mark_as_advanced(Monteverdi2_WC_REVISION)
  if(Monteverdi2_WC_STATUS)
    message(STATUS "Local file modifications:")
    message(STATUS ${Monteverdi2_WC_STATUS})
  else(Monteverdi2_WC_STATUS)
    message(STATUS "No files modified locally")
  endif(Monteverdi2_WC_STATUS)
  string(REGEX REPLACE "\n" ";" Monteverdi2_WC_STATUS "${Monteverdi2_WC_STATUS}")
  set(Monteverdi2_WC_STATUS ${Monteverdi2_WC_STATUS} CACHE STRING "Repository status." FORCE)
  mark_as_advanced(Monteverdi2_WC_STATUS)
endif(Mercurial_FOUND)

#----------------------------------------------------------------------------
#Build shared libraries for monteverdi
#By default, import the option from the OTB configuration
set(BUILD_SHARED_LIBS ${OTB_BUILD_SHARED_LIBS})

#----------------------------------------------------------------------------
# Configure the default OTB_DATA_ROOT for the location of OTB Data.
find_path(OTB_DATA_ROOT README-OTB-Data $ENV{OTB_DATA_ROOT})
mark_as_advanced(OTB_DATA_ROOT)

#----------------------------------------------------------------------------
option(OTB_DATA_USE_LARGEINPUT "Use Large inputs images test." OFF)
mark_as_advanced(OTB_DATA_USE_LARGEINPUT)
if(OTB_DATA_USE_LARGEINPUT)
  find_path(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
  mark_as_advanced(OTB_DATA_LARGEINPUT_ROOT)
endif(OTB_DATA_USE_LARGEINPUT)

#----------------------------------------------------------------------------
# set the Buildname for the dashboard
set(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard.")

#----------------------------------------------------------------------------
# Installation directories:
# Monteverdi2_INSTALL_BIN_DIR          - binary dir (executables)
# Monteverdi2_INSTALL_LIB_DIR          - library dir (libs)
# Monteverdi2_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# Monteverdi2_INSTALL_INCLUDE_DIR      - include dir (headers)
# Monteverdi2_INSTALL_PACKAGE_DIR      - package/export configuration files

if(NOT Monteverdi2_INSTALL_BIN_DIR)
  set(Monteverdi2_INSTALL_BIN_DIR "bin")
endif(NOT Monteverdi2_INSTALL_BIN_DIR)

if(NOT Monteverdi2_INSTALL_LIB_DIR)
  set(Monteverdi2_INSTALL_LIB_DIR "lib/otb")
endif(NOT Monteverdi2_INSTALL_LIB_DIR)

if(NOT Monteverdi2_INSTALL_DATA_DIR)
  set(Monteverdi2_INSTALL_DATA_DIR "share/otb")
endif(NOT Monteverdi2_INSTALL_DATA_DIR)

if(NOT Monteverdi2_INSTALL_INCLUDE_DIR)
  set(Monteverdi2_INSTALL_INCLUDE_DIR "include/otb")
endif(NOT Monteverdi2_INSTALL_INCLUDE_DIR)

if(NOT Monteverdi2_INSTALL_PACKAGE_DIR)
  set(Monteverdi2_INSTALL_PACKAGE_DIR ${Monteverdi2_INSTALL_LIB_DIR})
endif(NOT Monteverdi2_INSTALL_PACKAGE_DIR)

#----------------------------------------------------------------------------
include( PreventInSourceBuilds )
include( PreventInstallOnBuildTree )
include( DeployQtConf )

#----------------------------------------------------------------------------
# Configure files with settings for use by the build.
set( Monteverdi2_CONFIGURE_FILE "ConfigureMonteverdi2.h" )
configure_file(
  ${Monteverdi2_SOURCE_DIR}/${Monteverdi2_CONFIGURE_FILE}.in
  ${Monteverdi2_BINARY_DIR}/${Monteverdi2_CONFIGURE_FILE}
)

set(Monteverdi2_LIBRARY_PROPERTIES
    VERSION   "${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}.${Monteverdi2_VERSION_PATCH}"
    SOVERSION "${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}"
)

#----------------------------------------------------------------------------
# Include the Packaging subdir after every other dir
# since all other install directives must be executed before fixupbundle
#add_subdirectory(Packaging)

#-----------------------------------------------------------------------------
# Output (build) directories.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Monteverdi2_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Monteverdi2_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Monteverdi2_BINARY_DIR}/lib )

SET(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#----------------------------------------------------------------------------
# Code location
add_subdirectory( Code )
# i18n directory must be added after all source files (see add_qt1_i18n_sources).
add_subdirectory( i18n )
# CPack related files
add_subdirectory( Packaging )
# Data related files
add_subdirectory( Data )

#----------------------------------------------------------------------------
# Enable / Disable testing
option(BUILD_TESTING  "Build testing." OFF)

include(${CMAKE_ROOT}/Modules/Dart.cmake)
configure_file(CMake/CTestCustom.cmake.in CTestCustom.cmake)
mark_as_advanced(TCL_TCLSH DART_ROOT)
if(BUILD_TESTING)
  enable_testing()
  set(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard.")
  mark_as_advanced(BUILDNAME)
  add_subdirectory(Testing)
endif()

#----------------------------------------------------------------------------
# Generate Monteverdi2Config.cmake
# Save library dependencies.
# set(Monteverdi2_LIBRARY_DEPENDS_FILE ${Monteverdi2_BINARY_DIR}/Monteverdi2LibraryDepends.cmake)
# export_library_dependencies(${Monteverdi2_LIBRARY_DEPENDS_FILE})

# # Library directory.
# set(Monteverdi2_LIBRARY_DIRS ${OTB_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})
# set(Monteverdi2_USE_FILE ${Monteverdi2_BINARY_DIR}/UseMonteverdi2.cmake)
# configure_file(${Monteverdi2_SOURCE_DIR}/Monteverdi2Config.cmake.in
#                ${Monteverdi2_BINARY_DIR}/Monteverdi2Config.cmake @ONLY IMMEDIATE)
# configure_file(${Monteverdi2_SOURCE_DIR}/UseMonteverdi2.cmake.in
#                ${Monteverdi2_USE_FILE} @ONLY IMMEDIATE)

#----------------------------------------------------------------------------
# Setup valgrind option (default hard coded value is ctest is invalid).
# set(VALGRIND_COMMAND_OPTIONS  "--show-reachable=yes --workaround-gcc296-bugs=yes --num-callers=50")
