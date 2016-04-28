# cpack has a built-in target called 'package'. In our case we create two standalone installers with
# and uses a custom target named packages.

# stop generation of msvc packages for windows. 04-apr-2016. point OTB.

##### check if standalone project ######
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)
endif()

set(OUT_OF_SOURCE_BUILD FALSE)
if(PROJECT_NAME STREQUAL "OTBPackaging")
  set(OUT_OF_SOURCE_BUILD TRUE)
endif()

option(GENERATE_PACKAGE "Generate OTB package" ON)
option(GENERATE_XDK "Generate XDK for OTB development" OFF)

if(NOT GENERATE_PACKAGE AND NOT GENERATE_XDK)
  #well. we don't have any businees here. Let's move on..
  return()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/PMacros.cmake)

if(OUT_OF_SOURCE_BUILD)
  if(BUILD_TESTING)
    include(CTest)
  endif()
  set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
  # From parent scope (SuperBuild) we have
  # ENABLE_MONTEVERDI, DOWNLOAD_LOCATION, SB_INSTALL_PREFIX
  option(ENABLE_MONTEVERDI "enable monteverdi" ON)
  set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild binary directory")
  set(SB_INSTALL_PREFIX "" CACHE PATH "path to superbuild install directory")
else()
  # SUPERBUILD_BINARY_DIR is only set in Packaging/CMakeLists.txt and not available
  # in the parent scope like others. So we need to set it as CMAKE_BINARY_DIR
  # if doing packages from source tree.
  set(SUPERBUILD_BINARY_DIR ${CMAKE_BINARY_DIR})
endif()

if(NOT WIN32)
  #set CMAKE_INSTALL_PREFIX and PKG_INSTALL_PREFIX to SB_INSTALL_PREFIX for convenience.
  set(CMAKE_INSTALL_PREFIX "${SB_INSTALL_PREFIX}")
endif()

#used in PackageHelper.cmake (don't change)
set(PKG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

set(PACKAGE_NAME OTB)
set(PACKAGE_LONG_NAME OrfeoToolBox)
if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(PACKAGE_PLATFORM_NAME "win")
  if(MXE_TARGET_DIR MATCHES "i686")
    set(MXE_ARCH "i686")
    set(PACKAGE_ARCH "32")
  elseif(MXE_TARGET_DIR MATCHES "x86_64")
    set(MXE_ARCH "x86_64")
    set(PACKAGE_ARCH "64") #helper for ARCHIVE_NAME
  endif()
  get_filename_component(MXE_MXEROOT ${MXE_TARGET_DIR} PATH)
  get_filename_component(MXE_MXEROOT ${MXE_MXEROOT} PATH)
  set(PACKAGE_VERSION_STRING ${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH})
else() #(WIN32 OR CMAKE_CROSSCOMPILING)
  if(UNIX)
    set(PACKAGE_ARCH 32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(PACKAGE_ARCH 64)
    endif()
    if(APPLE)
      set(PACKAGE_PLATFORM_NAME "Darwin")
    else()
      set(PACKAGE_PLATFORM_NAME "Linux")
    endif()
    get_version("${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake" "ITK_SB_VERSION" ITK_SB_VERSION)
    get_version("${OTB_SOURCE_DIR}/CMakeLists.txt" "OTB_VERSION_" OTB_VERSION_STRING)
    set(PACKAGE_VERSION_STRING ${get_version_OTB_VERSION_STRING})
  endif(UNIX)
endif() #(WIN32 OR CMAKE_CROSSCOMPILING)

get_filename_component(OTB_SUPERBUILD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)
get_filename_component(OTB_SOURCE_DIR ${OTB_SUPERBUILD_SOURCE_DIR} PATH)

set(PACKAGE_LIST OTB)
if(GENERATE_XDK)
  list(APPEND PACKAGE_LIST XDK)
endif()

set(OTB_SUPERBUILD_SOURCE_DIR ${OTB_SOURCE_DIR}/SuperBuild)
include(${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)

set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/${pkg}-PACKAGE)

macro_setup_cmake_sources(OTB)
if(GENERATE_XDK)
  macro_setup_cmake_sources(XDK)
endif()

set(PACKAGE-configure_DEPENDS PACKAGE-TOOLS)
macro_update_dependencies_list(PACKAGE-configure_DEPENDS)

macro_create_targets_for_package(OTB)
if(GENERATE_XDK)
macro_create_targets_for_package(XDK)
endif()
