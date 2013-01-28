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
# Monteverdi2 version number.  An even minor number corresponds to releases.
set(Monteverdi2_VERSION_MAJOR "0")
set(Monteverdi2_VERSION_MINOR "1")
set(Monteverdi2_VERSION_PATCH "0")

#----------------------------------------------------------------------------
# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(Monteverdi2_VERSION_STRING "${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}.${Monteverdi2_VERSION_PATCH}")

#----------------------------------------------------------------------------
set(Monteverdi2_FLOATING_TYPE "double" CACHE STRING "The type of data in which input images are read (double/float)." )

#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
add_definitions(-DNOMINMAX -DOSSIM_STATIC)
endif(WIN32)

#----------------------------------------------------------------------------
# Link to the Orfeo ToolBox
find_package(OTB
  REQUIRED
  #No need to specify default (system) path.
  #PATHS /usr/lib/otb
)
if(OTB_FOUND)
  #message( "OTB_USE_FILE='${OTB_USE_FILE}'" )
  include(${OTB_USE_FILE})
else(OTB_FOUND)
  message(
    FATAL_ERROR
    "OTB not found. Please set OTB_DIR")
endif(OTB_FOUND)

#----------------------------------------------------------------------------
# Qt4 settings
find_package( Qt4 REQUIRED QtCore QtGui QtOpenGL)

# setup Qt environement
set( QT_USE_QTCORE TRUE )
set( QT_USE_QTGUI TRUE )
set( QT_USE_QTOPENGL TRUE)

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
# Include directories
set(Monteverdi2_INCLUDE_DIRS
  # ${Monteverdi2_SOURCE_DIR}/Code/
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  ${Monteverdi2_SOURCE_DIR}/Code/Application
  #
  ${Monteverdi2_BINARY_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Application
  )

include_directories(${Monteverdi2_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

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

#----------------------------------------------------------------------------
# Configure files with settings for use by the build.
set( Monteverdi2_CONFIGURE_FILE "ConfigureMonteverdi2.h" )
configure_file(
  ${Monteverdi2_SOURCE_DIR}/${Monteverdi2_CONFIGURE_FILE}.in
  ${Monteverdi2_BINARY_DIR}/${Monteverdi2_CONFIGURE_FILE}
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

#----------------------------------------------------------------------------
# Enable / Disable testing
option(BUILD_TESTING  "Build testing." OFF)

INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
CONFIGURE_FILE(CMake/CTestCustom.cmake.in CTestCustom.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard.")
  MARK_AS_ADVANCED(BUILDNAME)
  ADD_SUBDIRECTORY(Testing)
ENDIF(BUILD_TESTING)

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
