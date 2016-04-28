# cpack has a built-in target called 'package'. In our case we create two standalone installers with
# and uses a custom target named packages.

# stop generation of msvc packages for windows. 04-apr-2016. point OTB.

##### check if standalone project ######
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)
endif()

option(GENERATE_PACKAGE "Generate OTB package" OFF)
option(GENERATE_XDK "Generate XDK for OTB development" OFF)

set(OUT_OF_SOURCE_BUILD FALSE)
if(PROJECT_NAME STREQUAL "OTBPackaging")
  set(OUT_OF_SOURCE_BUILD TRUE)
  set(GENERATE_PACKAGE ON)
endif()

if(NOT GENERATE_PACKAGE AND NOT GENERATE_XDK)
  #well. we don't have any businees here. Let's move on..
  return()
endif()

if(WIN32 OR CMAKE_CROSSCOMPILING)
  if(NOT MXE_TARGET_DIR)
    message(FATAL_ERROR "you must set MXE_TARGET_DIR")
  endif()

  if(NOT OTB_SOURCE_DIR)
    message(FATAL_ERROR "you must set OTB_SOURCE_DIR")
  endif()
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/PMacros.cmake)

if(OUT_OF_SOURCE_BUILD)
  if(BUILD_TESTING)
    include(CTest)
  endif()
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(Monteverdi_SOURCE_DIR "" CACHE PATH "monteverdi source directory")
    set(Monteverdi_BINARY_DIR "" CACHE PATH "monteverdi binary directory")
    set(MONTEVERDI_INSTALL_DIR "" CACHE PATH "monteverdi install directory")
    set(OTB_INSTALL_DIR "" CACHE PATH "otb install directory")
  else()
    set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
    set(SUPERBUILD_INSTALL_DIR "" CACHE PATH "path to superbuild install directory")
    option(ENABLE_MONTEVERDI "enable monteverdi" ON)
    set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild binary directory")
  endif()
else(OUT_OF_SOURCE_BUILD)
  # SUPERBUILD_BINARY_DIR is only set in Packaging/CMakeLists.txt and not available
  # in the parent scope like others. So we need to set it as CMAKE_BINARY_DIR
  # if doing packages from source tree.
  set(SUPERBUILD_BINARY_DIR ${CMAKE_BINARY_DIR})
  set(SUPERBUILD_INSTALL_DIR ${SB_INSTALL_PREFIX})
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(MONTEVERDI_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
  endif()
endif(OUT_OF_SOURCE_BUILD)

get_filename_component(SUPERBUILD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)
get_filename_component(OTB_SOURCE_DIR ${SUPERBUILD_SOURCE_DIR} PATH)

if(WIN32 OR CMAKE_CROSSCOMPILING)
  foreach(req
      Monteverdi_SOURCE_DIR
      Monteverdi_BINARY_DIR
      MONTEVERDI_INSTALL_DIR
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req}")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)
else() #Unix like
  foreach(req
      SUPERBUILD_BINARY_DIR
      SUPERBUILD_INSTALL_DIR
      SUPERBUILD_SOURCE_DIR
      OTB_SOURCE_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req}")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)
endif()


if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(CMAKE_INSTALL_PREFIX "${MONTEVERDI_INSTALL_DIR}")
else()
  set(CMAKE_INSTALL_PREFIX "${SUPERBUILD_INSTALL_DIR}")
endif()

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
    get_version("${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake" "ITK_SB_VERSION" ITK_SB_VERSION)
    get_version("${OTB_SOURCE_DIR}/CMakeLists.txt" "OTB_VERSION_" OTB_VERSION_STRING)
    set(PACKAGE_VERSION_STRING ${get_version_OTB_VERSION_STRING})
  endif(UNIX)
endif() #(WIN32 OR CMAKE_CROSSCOMPILING)

include(${SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)

set(EXTRA_CACHE_CONFIG
  "set(CMAKE_CROSSCOMPILING ON)
   set(MXE_ARCH \"${MXE_ARCH}\")
   set(MXE_MXEROOT \"${MXE_MXEROOT}\")")

#guess install directory from OTB_MODULES_DIR
#copy of cmake variables not needed.
if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(Monteverdi_SOURCE_DIR       "${Monteverdi_SOURCE_DIR}")
  set(Monteverdi_BINARY_DIR       "${Monteverdi_BINARY_DIR}")
  set(MONTEVERDI_INSTALL_DIR      "${MONTEVERDI_INSTALL_DIR}")
  set(OTB_INSTALL_DIR             "${OTB_INSTALL_DIR}")
  set(DEPENDENCIES_INSTALL_DIR    "${MXE_MXEROOT}/usr/${MXE_ARCH}-w64-mingw32.shared")
  set(QT_PLUGINS_DIR              "${DEPENDENCIES_INSTALL_DIR}/qt/plugins")
  set(QT_TRANSLATIONS_DIR         "${DEPENDENCIES_INSTALL_DIR}/qt/translations")
  set(ITK_VERSION_STRING)
  else() #unixes
    set(Monteverdi_SOURCE_DIR     "${SUPERBUILD_BINARY_DIR}/MVD/src/MVD")
    set(Monteverdi_BINARY_DIR     "${SUPERBUILD_BINARY_DIR}/MVD/build")
    set(MONTEVERDI_INSTALL_DIR    "${SUPERBUILD_INSTALL_DIR}")
    set(QT_PLUGINS_DIR            "${SUPERBUILD_INSTALL_DIR}/plugins")
    set(QT_TRANSLATIONS_DIR       "${SUPERBUILD_INSTALL_DIR}/translations")
    set(OTB_INSTALL_DIR           "${SUPERBUILD_BINARY_DIR}/OTB/build")
    set(DEPENDENCIES_INSTALL_DIR  "${SUPERBUILD_INSTALL_DIR}")
    set(ITK_VERSION_STRING        "${get_version_ITK_SB_VERSION}")
    set(EXTRA_CACHE_CONFIG)
  endif()

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
