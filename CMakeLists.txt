PROJECT(Monteverdi2)

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
# Monteverdi2 version number.  An even minor number corresponds to releases.
set(Monteverdi2_VERSION_MAJOR "0")
set(Monteverdi2_VERSION_MINOR "9")
set(Monteverdi2_VERSION_PATCH "0")

#----------------------------------------------------------------------------
# Monteverdi2 data version number.  An even minor number corresponds to releases.
set(Monteverdi2_DATA_VERSION_MAJOR "1")
set(Monteverdi2_DATA_VERSION_MINOR "0")
set(Monteverdi2_DATA_VERSION_PATCH "0")

#----------------------------------------------------------------------------
# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(Monteverdi2_VERSION_STRING "${Monteverdi2_VERSION_MAJOR}.${Monteverdi2_VERSION_MINOR}.${Monteverdi2_VERSION_PATCH}")
#----------------------------------------------------------------------------
set(Monteverdi2_DATA_VERSION_STRING "${Monteverdi2_DATA_VERSION_MAJOR}.${Monteverdi2_DATA_VERSION_MINOR}.${Monteverdi2_DATA_VERSION_PATCH}")

#----------------------------------------------------------------------------
set(Monteverdi2_FLOATING_TYPE "float" CACHE STRING "The type of data in which input images are read (double/float)." )

#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
  if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DNOMINMAX -DOSSIM_STATIC)
  endif()
endif()

#----------------------------------------------------------------------------
# ITK
# find_package(ITK REQUIRED)
# if(ITK_FOUND)
#   MESSAGE(STATUS "ITK found")
#   include(${ITK_USE_FILE})
# else(ITK_FOUND)
#   message(FATAL_ERROR "ITK not found. Please set ITK_DIR")
# endif(ITK_FOUND)

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
  OTBStatistics)

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
# Ice
find_package(OTBIce REQUIRED)
find_package(GLEW REQUIRED)
# message( "ICE_INCLUDE_DIRS: ${ICE_INCLUDE_DIRS}" )
# message( "ICE_LIBRARIES: ${ICE_LIBRARIES}" )

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

option(Monteverdi2_USE_CPACK "Generate installer using CPack" OFF)
mark_as_advanced(Monteverdi2_USE_CPACK)

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

if(APPLE AND Monteverdi2_USE_CPACK)
  set(Monteverdi2_INSTALL_BIN_DIR "MacOS")
  set(Monteverdi2_INSTALL_DATA_DIR "Resources/otb")
endif(APPLE AND Monteverdi2_USE_CPACK)

#----------------------------------------------------------------------------
include( PreventInSourceBuilds )
include( PreventInstallOnBuildTree )
include( DeployQtConf )

#function to create installer for applications
include( CPackFunctions )

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

if(WIN32 AND Monteverdi2_USE_CPACK)
  add_custom_target(packages
    COMMAND ${CMAKE_CPACK_COMMAND}
    "--config" "${CMAKE_BINARY_DIR}/CPackConfig-Monteverdi2.cmake" "--verbose"
    DEPENDS PACKAGE_Mapla)

  add_custom_target(PACKAGE_Mapla
    COMMAND ${CMAKE_CPACK_COMMAND}
    "--config" "${CMAKE_BINARY_DIR}/CPackConfig-Mapla.cmake" "--verbose"
    DEPENDS PACKAGE_Mv2)

  add_custom_target(PACKAGE_Mv2
    COMMAND ${CMAKE_CPACK_COMMAND}
    "--config" "${CMAKE_BINARY_DIR}/CPackConfig-Mv2.cmake" "--verbose")

endif(WIN32 AND Monteverdi2_USE_CPACK)

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

if(WIN32 AND CMAKE_CROSSCOMPILING)
  if(DEFINED MXE_TARGET_DIR)
    message(STATUS "MXE_TARGET_DIR = ${MXE_TARGET_DIR}")

    if(MXE_TARGET_DIR MATCHES "i686")
      set(mxearch x86)
      set(archive_name ${CMAKE_PROJECT_NAME}-${Monteverdi2_VERSION_STRING}-win32)
    elseif(MXE_TARGET_DIR MATCHES "x86_64")
      set(mxearch x64)
      set(archive_name ${CMAKE_PROJECT_NAME}-${Monteverdi2_VERSION_STRING}-win64)
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_INSTALL_PREFIX}/${archive_name}")

    get_filename_component(mxeroot ${MXE_TARGET_DIR} PATH)
    get_filename_component(mxeroot ${mxeroot} PATH)

    #should we add otbiceviewer.exe?

    set(pefiles
      ${CMAKE_INSTALL_PREFIX}/bin/monteverdi2.exe
      ${CMAKE_INSTALL_PREFIX}/bin/mapla.exe)



    file(WRITE "${CMAKE_BINARY_DIR}/_mingw/CMakeLists.txt"
      "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       set(Monteverdi2_SOURCE_DIR \"${Monteverdi2_SOURCE_DIR}\")
       set(OTB_MODULES_DIR \"${OTB_MODULES_DIR}\")
       set(QT_PLUGINS_DIR \"${QT_PLUGINS_DIR}\")
       set(Monteverdi2_BINARY_DIR \"${CMAKE_BINARY_DIR}\")
       set(CMAKE_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\")
       set(Monteverdi2_INSTALL_DATA_DIR \"${Monteverdi2_INSTALL_DATA_DIR}\")
        include(${CMAKE_SOURCE_DIR}/CMake/MinGWPackage.cmake)
        include(${CMAKE_SOURCE_DIR}/CMake/CPackFunctions.cmake)
        package_mingw(
        ARCH \"${mxearch}\"
        MXEROOT \"${mxeroot}\"
        PREFIX_DIR \"${archive_name}\"
        PEFILES \"${pefiles}\"
        NEEDS_OTB_APPS TRUE,
        SEARCHDIRS \"\")")

    string(REPLACE "${CMAKE_PROJECT_NAME}" "Mv2" archive_name_mv2 ${archive_name})
    set(pefiles_mv2 ${CMAKE_INSTALL_PREFIX}/bin/mv2.exe)


    file(WRITE "${CMAKE_BINARY_DIR}/_mingw_mv2/CMakeLists.txt"
      "cmake_minimum_required(VERSION 2.6)
       include(CMakeParseArguments)
       set(Monteverdi2_SOURCE_DIR \"${Monteverdi2_SOURCE_DIR}\")
       set(OTB_MODULES_DIR \"${OTB_MODULES_DIR}\")
       set(QT_PLUGINS_DIR \"${QT_PLUGINS_DIR}\")
       set(Monteverdi2_BINARY_DIR \"${CMAKE_BINARY_DIR}\")
       set(CMAKE_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\")
       set(Monteverdi2_INSTALL_DATA_DIR \"${Monteverdi2_INSTALL_DATA_DIR}\")
        include(${CMAKE_SOURCE_DIR}/CMake/MinGWPackage.cmake)
        include(${CMAKE_SOURCE_DIR}/CMake/CPackFunctions.cmake)
        package_mingw(
        ARCH \"${mxearch}\"
        MXEROOT \"${mxeroot}\"
        PREFIX_DIR \"${archive_name_mv2}\"
        PEFILES \"${pefiles_mv2}\"
        SEARCHDIRS \"\")")

    add_custom_target(configure-mingw_mv2
      COMMAND ${CMAKE_COMMAND}
      "${CMAKE_BINARY_DIR}/_mingw_mv2"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_mingw_mv2")

    add_custom_target(prepare-mingw_mv2
      COMMAND ${CMAKE_COMMAND}
      "--build" "${CMAKE_BINARY_DIR}/_mingw_mv2" "--target" "install"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_mingw_mv2"
      DEPENDS configure-mingw_mv2)

    find_program(ZIP_EXECUTABLE zip)

    if(ZIP_EXECUTABLE)
      add_custom_target(package-mingw_mv2
        COMMAND ${ZIP_EXECUTABLE} "-r" "${CMAKE_BINARY_DIR}/${archive_name_mv2}.zip" "${archive_name_mv2}"
        WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
        DEPENDS prepare-mingw_mv2)
    else()
      message(FATAL_ERROR "Cannot find zip executable. Please add it to your path")
    endif()

    add_custom_target(configure-mingw
      COMMAND ${CMAKE_COMMAND}
      "${CMAKE_BINARY_DIR}/_mingw"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_mingw"
      DEPENDS package-mingw_mv2)

    add_custom_target(prepare-mingw
      COMMAND ${CMAKE_COMMAND}
      "--build" "${CMAKE_BINARY_DIR}/_mingw" "--target" "install"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/_mingw"
      DEPENDS configure-mingw)

    find_program(ZIP_EXECUTABLE zip)

    if(ZIP_EXECUTABLE)
      add_custom_target(package-mingw
        COMMAND ${ZIP_EXECUTABLE} "-r" "${CMAKE_BINARY_DIR}/${archive_name}.zip" "${archive_name}"
        WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
        DEPENDS prepare-mingw)
    else()
      message(FATAL_ERROR "Cannot find zip executable. Please add it to your path")
    endif()

  else(DEFINED MXE_TARGET_DIR)
    message(WARNING "MXE_TARGET_DIR must be defined and contains location of mxe root directory")
  endif(DEFINED MXE_TARGET_DIR)

endif(WIN32 AND CMAKE_CROSSCOMPILING)
