# cpack has a built-in target called 'package'. In our case we create two standalone installers with
# and uses a custom target named packages.

# stop generation of msvc packages for windows. 04-apr-2016. point OTB.

##### check if standalone project ######
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)

  option(GENERATE_PACKAGE "Generate OTB package" OFF)
  option(GENERATE_XDK "Generate XDK for OTB development" OFF)
endif()

if(PROJECT_NAME STREQUAL "OTB-SuperBuild")
  if(NOT PACKAGE_OTB_SRC_DIR)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${OTB-SuperBuild_SOURCE_DIR} PATH)
  endif()
endif()

set(OUT_OF_SOURCE_BUILD FALSE)
if(PROJECT_NAME STREQUAL "OTBPackaging")
  set(OUT_OF_SOURCE_BUILD TRUE)
  set(GENERATE_PACKAGE ON)
  if(NOT PACKAGE_OTB_SRC_DIR)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${OTBPackaging_SOURCE_DIR} PATH)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${PACKAGE_OTB_SRC_DIR} PATH)
  endif()
  
  option(OTB_WRAP_PYTHON "enable python wrapping" ON)
  if(OTB_WRAP_PYTHON AND NOT PYTHON_EXECUTABLE)
    find_package ( PythonInterp REQUIRED )
  endif()
endif() #if(PROJECT_NAME STREQUAL ..

if(NOT GENERATE_PACKAGE AND NOT GENERATE_XDK)
  #well. we don't have any businees here. Let's move on..
  return()
endif()

if(WIN32 AND NOT MSVC)
  if(NOT MXE_TARGET_DIR)
    message(FATAL_ERROR "you must set MXE_TARGET_DIR")
  endif()
endif()

if(NOT PACKAGE_OTB_SRC_DIR)
  message(FATAL_ERROR "you must set PACKAGE_OTB_SRC_DIR")
endif()
set(SUPERBUILD_SOURCE_DIR ${PACKAGE_OTB_SRC_DIR}/SuperBuild)

include(${CMAKE_CURRENT_SOURCE_DIR}/PMacros.cmake)

if(OUT_OF_SOURCE_BUILD)
  if(BUILD_TESTING)
    include(CTest)
  endif()
  if(WIN32 AND NOT MSVC)
    set(OTB_BINARY_DIR "" CACHE PATH "OTB binary directory")
    set(OTB_INSTALL_DIR "" CACHE PATH "OTB install directory")
  else()
    set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
    set(SUPERBUILD_INSTALL_DIR "" CACHE PATH "path to superbuild install directory")
    set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild binary directory")
  endif()
else(OUT_OF_SOURCE_BUILD)
  # SUPERBUILD_BINARY_DIR is only set in Packaging/CMakeLists.txt and not available
  # in the parent scope like others. So we need to set it as CMAKE_BINARY_DIR
  # if doing packages from source tree.
  set( SUPERBUILD_BINARY_DIR  ${CMAKE_BINARY_DIR}  )
  set( SUPERBUILD_INSTALL_DIR ${SB_INSTALL_PREFIX} )
  set( OTB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
  #if(WIN32 AND NOT MSVC)
    #get_filename_component(OTB_INSTALL_DIR ${OTB_MODULES_DIR} PATH)
    #get_filename_component(OTB_INSTALL_DIR ${OTB_INSTALL_DIR} PATH)
    #get_filename_component(OTB_INSTALL_DIR ${OTB_INSTALL_DIR} PATH)
    #get_filename_component(OTB_INSTALL_DIR ${OTB_INSTALL_DIR} PATH)
  #endif()
endif(OUT_OF_SOURCE_BUILD)

if(WIN32)
  set(ZIP_EXE_NAMES 7z 7za)
  find_program(ZIP_EXECUTABLE NAMES ${ZIP_EXE_NAMES})
    if(NOT ZIP_EXECUTABLE)
    message(FATAL_ERROR "Cannot find 7z executable. searched names ${ZIP_EXE_NAMES}. Please add it to your path")
  endif()
endif()

if(WIN32 AND NOT MSVC)
  foreach(req
      PACKAGE_OTB_SRC_DIR
      OTB_BINARY_DIR
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
      PACKAGE_OTB_SRC_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req}")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)
endif()

set(EXTRA_CACHE_CONFIG)

if(WIN32 AND NOT MSVC)
  set(CMAKE_INSTALL_PREFIX "${OTB_INSTALL_DIR}")
else()
  set(CMAKE_INSTALL_PREFIX "${SUPERBUILD_INSTALL_DIR}")
endif()

set(PACKAGE_NAME OTB)
set(PACKAGE_LONG_NAME OrfeoToolBox)

if(WIN32 AND NOT MSVC)
  if(MXE_TARGET_DIR MATCHES "i686")
    set(MXE_ARCH "i686")
    set(PACKAGE_ARCH "32")
  elseif(MXE_TARGET_DIR MATCHES "x86_64")
    set(MXE_ARCH "x86_64")
    set(PACKAGE_ARCH "64") #helper for ARCHIVE_NAME
  endif()
  get_filename_component(MXE_MXEROOT ${MXE_TARGET_DIR} PATH)
  get_filename_component(MXE_MXEROOT ${MXE_MXEROOT} PATH)
  set(EXTRA_CACHE_CONFIG
    "set(CMAKE_CROSSCOMPILING TRUE)
     set(WIN32 TRUE)
     set(MXE_ARCH \"${MXE_ARCH}\")
     set(MXE_MXEROOT \"${MXE_MXEROOT}\")"
    )
else() #(WIN32 AND NOT MSVC)
  set(PACKAGE_ARCH 32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PACKAGE_ARCH 64)
  endif()
endif()

if(APPLE)
  set(PACKAGE_PLATFORM_NAME "Darwin")
elseif(WIN32)
  set(PACKAGE_PLATFORM_NAME "win")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(PACKAGE_PLATFORM_NAME "Linux")
endif()

#get itk version from External_itk.cmake if not on mingw
if(NOT MINGW)
  get_version(
    INPUT_FILE   "${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake"
    MATCH_STRINGS "ITK_SB_VERSION"
    PREFIX PKG
    )
endif()

#get version of OTB from source_dir/CMakeLists.txt
get_version(
  INPUT_FILE   "${PACKAGE_OTB_SRC_DIR}/CMakeLists.txt"
  MATCH_STRINGS "OTB_VERSION_MAJOR" "OTB_VERSION_MINOR" "OTB_VERSION_PATCH"
  PREFIX PKG
  )

set(PKG_OTB_VERSION_STRING
  ${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}.${PKG_OTB_VERSION_PATCH})

if(MINGW)
  set(OTB_BINARY_DIR              "${OTB_BINARY_DIR}")
  set(OTB_INSTALL_DIR             "${OTB_INSTALL_DIR}")
  set(DEPENDENCIES_INSTALL_DIR    "${MXE_MXEROOT}/usr/${MXE_ARCH}-w64-mingw32.shared")
  set(QT_PLUGINS_DIR              "${DEPENDENCIES_INSTALL_DIR}/qt/plugins")
  set(QT_TRANSLATIONS_DIR         "${DEPENDENCIES_INSTALL_DIR}/qt/translations")
else() #unixes
  set(OTB_BINARY_DIR            "${SUPERBUILD_BINARY_DIR}/OTB/build")
  set(OTB_INSTALL_DIR           "${SUPERBUILD_INSTALL_DIR}")
  set(DEPENDENCIES_INSTALL_DIR  "${SUPERBUILD_INSTALL_DIR}")
  set(QT_PLUGINS_DIR            "${SUPERBUILD_INSTALL_DIR}/plugins")
  set(QT_TRANSLATIONS_DIR       "${SUPERBUILD_INSTALL_DIR}/translations")
endif()

macro_setup_cmake_project(OTB)
if(GENERATE_XDK)
  macro_setup_cmake_project(XDK)
endif()
