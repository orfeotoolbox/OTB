PROJECT(Monteverdi)

cmake_minimum_required(VERSION 2.6)

# CMP0040 : from CMake 3.0, old behaviour is more convenient
if(POLICY CMP0040)
  cmake_policy(SET CMP0040 OLD)
endif()

#----------------------------------------------------------------------------
# Modules
set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH} )
include( i18n_qt4 )
#include( FindQwt )

#----------------------------------------------------------------------------
# ensure WIN32 definition
if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

if(APPLE)
set(CMAKE_MACOSX_RPATH 1)
endif()


#----------------------------------------------------------------------------
# ensure DEBUG definitions
#if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
#  add_definitions( -D_DEBUG )
#endif()
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG" )

#----------------------------------------------------------------------------
# Monteverdi version number.  An even minor number corresponds to releases.
set(Monteverdi_VERSION_MAJOR "3")
set(Monteverdi_VERSION_MINOR "1")
set(Monteverdi_VERSION_PATCH "0")
set(Monteverdi_VERSION_SUFFIX "")

#----------------------------------------------------------------------------
# Monteverdi data version number.  An even minor number corresponds to releases.
set(Monteverdi_DATA_VERSION_MAJOR "1")
set(Monteverdi_DATA_VERSION_MINOR "0")
set(Monteverdi_DATA_VERSION_PATCH "0")

#----------------------------------------------------------------------------
# Monteverdi UI version.
#
# Used to identify UI version for Qt Save/Restore layout. Must be incremented
# each time a widget is added to the UI.
set(Monteverdi_UI_VERSION "1")

#----------------------------------------------------------------------------
# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(
  Monteverdi_VERSION_STRING
  "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}.${Monteverdi_VERSION_PATCH}${Monteverdi_VERSION_SUFFIX}"
)

#----------------------------------------------------------------------------
set(
  Monteverdi_DATA_VERSION_STRING
  "${Monteverdi_DATA_VERSION_MAJOR}.${Monteverdi_DATA_VERSION_MINOR}.${Monteverdi_DATA_VERSION_PATCH}"
)

#----------------------------------------------------------------------------
set(Monteverdi_FLOATING_TYPE "float" CACHE STRING "The type of data in which input images are read (double/float)." )

#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
  if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DNOMINMAX -DOSSIM_STATIC)
  endif()
endif()

#----------------------------------------------------------------------------
# Orfeo ToolBox
#should we follow modularization here or import all OTB Modules?

#NOTE: If any header from an OTB Module which not listed below is added,
#then below line MUST be updated. At the sametime this ensure only the
#components/modules listed below are linked via OTB_LIBRARIES
find_package(OTB REQUIRED
  COMPONENTS
  OTBApplicationEngine
  OTBQtWidget
  OTBImageIO
  OTBVectorDataIO
  OTBTestKernel
  OTBCarto
  OTBProjection
  OTBStatistics
  OTBIce)

if(OTB_FOUND)
  include(${OTB_USE_FILE})
else(OTB_FOUND)
  message(FATAL_ERROR "OTB not found. Please set OTB_DIR")
endif(OTB_FOUND)

#----------------------------------------------------------------------------
# OpenGL (needed by Qt4)
find_package( OpenGL REQUIRED )

#----------------------------------------------------------------------------
# Qt4
if(CMAKE_VERSION VERSION_GREATER 2.8.11)
  # Disable automatic linking to qtmain on windows
  # as in previous versions of cmake
  cmake_policy(SET CMP0020 OLD)
endif()

set(QT_USE_IMPORTED_TARGETS ON)
find_package( Qt4 REQUIRED QtCore QtGui QtOpenGL QtMain QtSql QtXml )
include( ${QT_USE_FILE} )
if( QT_FOUND )
  # message( "Qt plugins dir: '${QT_PLUGINS_DIR}'" )
  # message( "Qt translations dir: '${QT_TRANSLATIONS_DIR}'" )
endif( QT_FOUND )

#----------------------------------------------------------------------------
# Qwt
find_package(Qwt REQUIRED)
if(NOT QWT_MAJOR_VERSION EQUAL 5)
  message(FATAL_ERROR "Only Qwt 5 is supported. Found version ${QWT_VERSION_STRING}")
endif()

if(WIN32)
  add_definitions(-DQWT_DLL)
endif(WIN32)

#----------------------------------------------------------------------------
# Include OTB CMake modules path.
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${OTB_SOURCE_DIR}/CMake")

# #----------------------------------------------------------------------------
# find_package(Mercurial QUIET)
# if(Mercurial_FOUND)
#   Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} Monteverdi)
#   message(STATUS "Mercurial version is ${Mercurial_VERSION_HG}.")
#   message(STATUS "Repository revision is ${Monteverdi_WC_REVISION}.")
#   set(Monteverdi_WC_REVISION ${Monteverdi_WC_REVISION} CACHE STRING "Repository version." FORCE)
#   mark_as_advanced(Monteverdi_WC_REVISION)
#   if(Monteverdi_WC_STATUS)
#     message(STATUS "Local file modifications:")
#     message(STATUS ${Monteverdi_WC_STATUS})
#   else(Monteverdi_WC_STATUS)
#     message(STATUS "No files modified locally")
#   endif(Monteverdi_WC_STATUS)
#   string(REGEX REPLACE "\n" ";" Monteverdi_WC_STATUS "${Monteverdi_WC_STATUS}")
#   set(Monteverdi_WC_STATUS ${Monteverdi_WC_STATUS} CACHE STRING "Repository status." FORCE)
#   mark_as_advanced(Monteverdi_WC_STATUS)
# endif(Mercurial_FOUND)

#----------------------------------------------------------------------------
#Build shared libraries for monteverdi
#By default, import the option from the OTB configuration

set(BUILD_SHARED_LIBS ${OTB_BUILD_SHARED})
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

option(Monteverdi_USE_CPACK "Generate installer using CPack" OFF)
mark_as_advanced(Monteverdi_USE_CPACK)

#----------------------------------------------------------------------------
# Installation directories:
# Monteverdi_INSTALL_BIN_DIR          - binary dir (executables)
# Monteverdi_INSTALL_LIB_DIR          - library dir (libs)
# Monteverdi_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# Monteverdi_INSTALL_INCLUDE_DIR      - include dir (headers)
# Monteverdi_INSTALL_PACKAGE_DIR      - package/export configuration files

if(NOT Monteverdi_INSTALL_BIN_DIR)
  set(Monteverdi_INSTALL_BIN_DIR "bin")
endif(NOT Monteverdi_INSTALL_BIN_DIR)

if(NOT Monteverdi_INSTALL_LIB_DIR)
  set(Monteverdi_INSTALL_LIB_DIR "lib/otb")
endif(NOT Monteverdi_INSTALL_LIB_DIR)

if(NOT Monteverdi_INSTALL_DATA_DIR)
  set(Monteverdi_INSTALL_DATA_DIR "share/otb")
endif(NOT Monteverdi_INSTALL_DATA_DIR)

if(NOT Monteverdi_INSTALL_INCLUDE_DIR)
  set(Monteverdi_INSTALL_INCLUDE_DIR "include/otb")
endif(NOT Monteverdi_INSTALL_INCLUDE_DIR)

if(NOT Monteverdi_INSTALL_PACKAGE_DIR)
  set(Monteverdi_INSTALL_PACKAGE_DIR ${Monteverdi_INSTALL_LIB_DIR})
endif(NOT Monteverdi_INSTALL_PACKAGE_DIR)

if(APPLE AND Monteverdi_USE_CPACK)
  set(Monteverdi_INSTALL_BIN_DIR "MacOS")
  set(Monteverdi_INSTALL_DATA_DIR "Resources/otb")
endif(APPLE AND Monteverdi_USE_CPACK)

#----------------------------------------------------------------------------
include( PreventInSourceBuilds )
include( PreventInstallOnBuildTree )
include( DeployQtConf )

#function to create installer for applications
include( CPackFunctions )

#----------------------------------------------------------------------------
# Configure files with settings for use by the build.
set( Monteverdi_CONFIGURE_FILE "ConfigureMonteverdi.h" )
configure_file(
  ${Monteverdi_SOURCE_DIR}/${Monteverdi_CONFIGURE_FILE}.in
  ${Monteverdi_BINARY_DIR}/${Monteverdi_CONFIGURE_FILE}
)

set(Monteverdi_LIBRARY_PROPERTIES
    VERSION   "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}.${Monteverdi_VERSION_PATCH}"
    SOVERSION "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}"
)

#----------------------------------------------------------------------------
# Include the Packaging subdir after every other dir
# since all other install directives must be executed before fixupbundle
#add_subdirectory(Packaging)

#-----------------------------------------------------------------------------
# Output (build) directories.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Monteverdi_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Monteverdi_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Monteverdi_BINARY_DIR}/lib )

SET(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#----------------------------------------------------------------------------
# Code location
add_subdirectory( Code )

# i18n directory must be added after all source files (see add_qt1_i18n_sources).
add_subdirectory( i18n )

# CPack related files
#We use functions from CMake/CPackFunctions.cmake after OTB 5.0.0
#add_subdirectory( Packaging )

# Data related files
add_subdirectory( Data )

# SQL related files.
add_subdirectory( Sql )


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

############## Packaging stuff for Windows ####################
#cpack has a built-in target called 'package'. In our case we create two standalone installers with
#and uses a custom target named packages.
if(WIN32)
  #RK: two packages
  create_cpack_config()
  if(Monteverdi_USE_CPACK)
    add_custom_target(packages
    COMMAND ${CMAKE_COMMAND} "--build" "${CMAKE_BINARY_DIR}" "--target" "install"
    COMMAND ${CMAKE_CPACK_COMMAND} "--config" "${CMAKE_BINARY_DIR}/CPackConfig-Monteverdi.cmake" "--verbose")

    #   add_custom_target(PACKAGE_Mapla
    #     COMMAND ${CMAKE_CPACK_COMMAND}
    #     "--config" "${CMAKE_BINARY_DIR}/CPackConfig-Mapla.cmake" "--verbose"
    #     DEPENDS PACKAGE_Monteverdi)

  endif() # (Monteverdi_USE_CPACK)

  if(CMAKE_CROSSCOMPILING)
 
    #print MXE_TARGET_DIR for the sake of debugging. This is visible on on dashboard log
    if(DEFINED MXE_TARGET_DIR)
      message(STATUS "MXE_TARGET_DIR = ${MXE_TARGET_DIR}")
    endif() # (DEFINED MXE_TARGET_DIR)

    #add custom target that create mapla and monteverdi2 archives.
    add_custom_target(packages
      COMMAND ${CMAKE_COMMAND} -E touch mingw-package
      WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
      DEPENDS
      create-Monteverdi-mingw-archive
      create-Mapla-mingw-archive)

  endif() # (CMAKE_CROSSCOMPILING)
endif() # (WIN32)

#For MacOS and Windows platform, we create a custom target called packages to generate packages
#For linux: the packages are provided via respective package management tool. (deb, rpm etc..)
