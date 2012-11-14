PROJECT(Monteverdi2)

cmake_minimum_required(VERSION 2.6)

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
set(Monteverdi2_VERSION_STRING "${Monteverdi_VERSION_MAJOR}.${Monteverdi_VERSION_MINOR}.${Monteverdi_VERSION_PATCH}")

#----------------------------------------------------------------------------
set(Monteverdi2_FLOATING_TYPE "float" CACHE STRING "The type of data in which input images are read (double/float)." )

#----------------------------------------------------------------------------
#Prevents conflicts with visual min/max function
#Add the OSSIMSTATIC flag to prevent from including dclspec(dllimport) from ossim
if(WIN32)
add_definitions(-DNOMINMAX -DOSSIM_STATIC)
endif(WIN32)

#----------------------------------------------------------------------------
# ensure WIN32 definition
if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

#----------------------------------------------------------------------------
# Link to the Orfeo ToolBox
find_package(OTB PATHS /usr/lib/otb)
if(OTB_FOUND)
  include(${OTB_USE_FILE})
else(OTB_FOUND)
  message(
    FATAL_ERROR
    "OTB not found. Please set OTB_DIR")
endif(OTB_FOUND)

#----------------------------------------------------------------------------
# Qt4 settings
find_package( Qt4 REQUIRED QtCore QtGui)
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
# Add binary dir to build paths in order to include Qt4 wrapped files.
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
# Choose Qt4 libraries.
set( QT_USE_QTCORE TRUE )
set( QT_USE_QTGUI TRUE )

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
# Configure files with settings for use by the build.
#configure_file(${Monteverdi2_SOURCE_DIR}/ConfigureMonteverdi2.h.in
#               ${Monteverdi2_BINARY_DIR}/ConfigureMonteverdi2.h)

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
  ${OTB_INCLUDE_DIRS}
  ${Monteverdi2_SOURCE_DIR}/Code
  )

#----------------------------------------------------------------------------
# Installation vars:
#
# Monteverdi2_INSTALL_BIN_DIR          - binary dir (executables)
# Monteverdi2_INSTALL_LIB_DIR          - library dir (libs)
# Monteverdi2_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# Monteverdi2_INSTALL_INCLUDE_DIR      - include dir (headers)
# Monteverdi2_INSTALL_PACKAGE_DIR      - package/export configuration files
# Monteverdi2_INSTALL_NO_DEVELOPMENT   - do not install development files
# Monteverdi2_INSTALL_NO_RUNTIME       - do not install runtime files
# Monteverdi2_INSTALL_NO_DOCUMENTATION - do not install documentation files

if(NOT Monteverdi2_INSTALL_BIN_DIR)
  set(Monteverdi2_INSTALL_BIN_DIR "/bin")
endif(NOT Monteverdi2_INSTALL_BIN_DIR)

if(NOT Monteverdi2_INSTALL_LIB_DIR)
  set(Monteverdi2_INSTALL_LIB_DIR "/lib/otb")
endif(NOT Monteverdi2_INSTALL_LIB_DIR)

if(NOT Monteverdi2_INSTALL_DATA_DIR)
  set(Monteverdi2_INSTALL_DATA_DIR "/share/otb")
endif(NOT Monteverdi2_INSTALL_DATA_DIR)

if(NOT Monteverdi2_INSTALL_INCLUDE_DIR)
  set(Monteverdi2_INSTALL_INCLUDE_DIR "/include/otb")
endif(NOT Monteverdi2_INSTALL_INCLUDE_DIR)

if(NOT Monteverdi2_INSTALL_PACKAGE_DIR)
  set(Monteverdi2_INSTALL_PACKAGE_DIR ${Monteverdi2_INSTALL_LIB_DIR}
    CACHE INTERNAL "")
endif(NOT Monteverdi2_INSTALL_PACKAGE_DIR)

if(NOT Monteverdi2_INSTALL_NO_DEVELOPMENT)
  set(Monteverdi2_INSTALL_NO_DEVELOPMENT 0)
endif(NOT Monteverdi2_INSTALL_NO_DEVELOPMENT)

if(NOT Monteverdi2_INSTALL_NO_RUNTIME)
  set(Monteverdi2_INSTALL_NO_RUNTIME 0)
endif(NOT Monteverdi2_INSTALL_NO_RUNTIME)

if(NOT Monteverdi2_INSTALL_NO_DOCUMENTATION)
  set(Monteverdi2_INSTALL_NO_DOCUMENTATION 0)
endif(NOT Monteverdi2_INSTALL_NO_DOCUMENTATION)

set(Monteverdi2_INSTALL_NO_LIBRARIES)
if(Monteverdi2_BUILD_SHARED_LIBS)
  if(Monteverdi2_INSTALL_NO_RUNTIME AND Monteverdi2_INSTALL_NO_DEVELOPMENT)
    set(Monteverdi2_INSTALL_NO_LIBRARIES 1)
  endif(Monteverdi2_INSTALL_NO_RUNTIME AND Monteverdi2_INSTALL_NO_DEVELOPMENT)
else(Monteverdi2_BUILD_SHARED_LIBS)
  if(Monteverdi2_INSTALL_NO_DEVELOPMENT)
    set(Monteverdi2_INSTALL_NO_LIBRARIES 1)
  endif(Monteverdi2_INSTALL_NO_DEVELOPMENT)
endif(Monteverdi2_BUILD_SHARED_LIBS)

#----------------------------------------------------------------------------
# Because INSTALL_* commands require a leading / and because INSTALL (cmake 2.4
# and newer) requires no leading / to install under INSTALL_PREFIX, we
# are stripping the leading /. In the future, there should be no leading
# / in any install directory variables
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_LIB_DIR_CM24 "${Monteverdi2_INSTALL_LIB_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_BIN_DIR_CM24 "${Monteverdi2_INSTALL_BIN_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_INCLUDE_DIR_CM24 "${Monteverdi2_INSTALL_INCLUDE_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_PACKAGE_DIR_CM24 "${Monteverdi2_INSTALL_PACKAGE_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_DOXYGEN_DIR_CM24 "${Monteverdi2_INSTALL_DOXYGEN_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_TCL_DIR_CM24 "${Monteverdi2_INSTALL_TCL_DIR}")
string(REGEX REPLACE "^/" "" Monteverdi2_INSTALL_JAVA_DIR_CM24 "${Monteverdi2_INSTALL_JAVA_DIR}")

#----------------------------------------------------------------------------
# Make sure the user does not try to install on top of the build tree.
if(WIN32 OR APPLE OR UNIX)
  string(TOLOWER "${CMAKE_INSTALL_PREFIX}" _PREFIX)
  string(TOLOWER "${Monteverdi2_BINARY_DIR}" _BUILD)
else(WIN32 OR APPLE OR UNIX)
  set(_PREFIX "${CMAKE_INSTALL_PREFIX}")
  set(_BUILD "${Monteverdi2_BINARY_DIR}")
endif(WIN32 OR APPLE OR UNIX)
if("${_PREFIX}" STREQUAL "${_BUILD}")
  message(FATAL_ERROR
    "The current CMAKE_INSTALL_PREFIX points at the build tree:\n"
    "  ${CMAKE_INSTALL_PREFIX}\n"
    "This is not supported."
    )
endif("${_PREFIX}" STREQUAL "${_BUILD}")

#----------------------------------------------------------------------------
set(Monteverdi2_INCLUDE_DIRS ${Monteverdi2_INCLUDE_DIRS} ${Monteverdi2_SOURCE_DIR}/Code/Application)

#----------------------------------------------------------------------------
include_directories(${Monteverdi2_INCLUDE_DIRS})

#----------------------------------------------------------------------------
# Code location
add_subdirectory(Code)

#----------------------------------------------------------------------------
# Include the Packaging subdir after every other dir
# since all other install directives must be executed before fixupbundle
#add_subdirectory(Packaging)

#-----------------------------------------------------------------------------
# Output directories.
set(LIBRARY_OUTPUT_PATH ${Monteverdi2_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
set(EXECUTABLE_OUTPUT_PATH ${Monteverdi2_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
#set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

# Enable / Disable testing
#option(BUILD_TESTING  "Build testing." OFF)

#INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
#CONFIGURE_FILE(CMake/CTestCustom.cmake.in CTestCustom.cmake)
#MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
#IF(BUILD_TESTING)
#  ENABLE_TESTING()
#  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard.")
#  MARK_AS_ADVANCED(BUILDNAME)
#  ADD_SUBDIRECTORY(Testing)
#ENDIF(BUILD_TESTING)

#----------------------------------------------------------------------------
# Generate Monteverdi2Config.cmake

# Save library dependencies.
set(Monteverdi2_LIBRARY_DEPENDS_FILE ${Monteverdi2_BINARY_DIR}/Monteverdi2LibraryDepends.cmake)
export_library_dependencies(${Monteverdi2_LIBRARY_DEPENDS_FILE})

# Library directory.
set(Monteverdi2_LIBRARY_DIRS ${OTB_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})

#set(Monteverdi2_USE_FILE ${Monteverdi2_BINARY_DIR}/UseMonteverdi2.cmake)

#configure_file(${Monteverdi2_SOURCE_DIR}/Monteverdi2Config.cmake.in
#               ${Monteverdi2_BINARY_DIR}/Monteverdi2Config.cmake @ONLY IMMEDIATE)
#configure_file(${Monteverdi2_SOURCE_DIR}/UseMonteverdi2.cmake.in
#               ${Monteverdi2_USE_FILE} @ONLY IMMEDIATE)


#----------------------------------------------------------------------------
# Setup valgrind option (default hard coded value is ctest is invalid).
#set(VALGRIND_COMMAND_OPTIONS  "--show-reachable=yes --workaround-gcc296-bugs=yes --num-callers=50")
