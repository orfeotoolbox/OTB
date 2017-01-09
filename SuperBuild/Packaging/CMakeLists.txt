# cpack has a built-in target called 'package'. In our case we create two standalone installers with
# and uses a custom target named packages.

# stop generation of msvc packages for windows. 04-apr-2016. point OTB.

##### check if standalone project ######
set(OUT_OF_SOURCE_BUILD FALSE)
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)

  option(GENERATE_PACKAGE "Generate OTB package" ON)
  option(GENERATE_XDK "Generate XDK for OTB development" OFF)
  
  set(OUT_OF_SOURCE_BUILD TRUE)
  
  if(BUILD_TESTING)
    include(CTest)
  endif()
  
  if(NOT PACKAGE_OTB_SRC_DIR)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${OTBPackaging_SOURCE_DIR} PATH)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${PACKAGE_OTB_SRC_DIR} PATH)
  endif()
  
  option(OTB_WRAP_PYTHON "enable python wrapping" ON)
  if(OTB_WRAP_PYTHON AND NOT PYTHON_EXECUTABLE)
    find_package ( PythonInterp REQUIRED )
    mark_as_advanced(PYTHON_INCLUDE_DIR)
    mark_as_advanced(PYTHON_LIBRARY)
  endif()

 if(OTB_MINGW)
    set(OTB_BINARY_DIR "" CACHE PATH "OTB binary directory")
    set(OTB_INSTALL_DIR "" CACHE PATH "OTB install directory")
  else()
    set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
    set(SUPERBUILD_INSTALL_DIR "" CACHE PATH "path to superbuild install directory")
    set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild download archives")
  endif()
  
else() #if(NOT PROJECT_NAME)

  # SUPERBUILD_BINARY_DIR is only set in Packaging/CMakeLists.txt and not available
  # in the parent scope like others. So we need to set it as CMAKE_BINARY_DIR
  # if doing packages from source tree.
  set( SUPERBUILD_BINARY_DIR   ${CMAKE_BINARY_DIR}     )
  set( SUPERBUILD_INSTALL_DIR  ${SB_INSTALL_PREFIX}    )
  set( OTB_INSTALL_DIR         ${CMAKE_INSTALL_PREFIX} )
  
  if(NOT PACKAGE_OTB_SRC_DIR)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${OTB-SuperBuild_SOURCE_DIR} PATH)
  endif()
  
endif()

if(NOT GENERATE_PACKAGE AND NOT GENERATE_XDK)
  #well. we don't have any businees here. Let's move on..
  return()
endif()

set(LINUX FALSE)
set(OTB_MINGW FALSE)
if(WIN32 AND NOT MSVC)
  set(OTB_MINGW TRUE)
else()
  if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
    set(LINUX TRUE)
  endif()
endif()

find_program(FILE_COMMAND "file")
mark_as_advanced(FILE_COMMAND)

set(SUPERBUILD_SOURCE_DIR ${PACKAGE_OTB_SRC_DIR}/SuperBuild)

include(${SUPERBUILD_SOURCE_DIR}/CMake/SuperBuild_Macro.cmake)
include(${PACKAGE_OTB_SRC_DIR}/CMake/OTBCheckTargetSystemArch.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CreateCMakeProjects.cmake)

if(WIN32)
  set(ZIP_EXE_NAMES 7z 7za)
  find_program(ZIP_EXECUTABLE NAMES ${ZIP_EXE_NAMES})
  if(NOT ZIP_EXECUTABLE)
    message(FATAL_ERROR "Cannot find 7z executable. searched names ${ZIP_EXE_NAMES}. Please add it to your path")
  endif()
endif()

if(OTB_MINGW)
  set(required_vars
  OTB_BINARY_DIR
  OTB_INSTALL_DIR
  MXE_TARGET_DIR
  )
else()
    set(required_vars
  SUPERBUILD_BINARY_DIR
  SUPERBUILD_INSTALL_DIR
  SUPERBUILD_SOURCE_DIR
  )
endif()

foreach(req
  ${required_vars}
  PACKAGE_OTB_SRC_DIR
  OTB_TARGET_SYSTEM_ARCH
  OTB_TARGET_SYSTEM_ARCH_IS_X64
 )
  if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
    message(FATAL_ERROR "you must set ${req}")
  endif()
  set(vars "${vars}  ${req}=[${${req}}]\n")
endforeach(req)

message("\n${vars}\n")

set(PKG_DEBUG OFF)

set(EXTRA_CACHE_CONFIG)

set(CMAKE_INSTALL_PREFIX "${SUPERBUILD_INSTALL_DIR}")

set(PACKAGE_LONG_NAME OrfeoToolBox)

# Make up part of output package file
set(PACKAGE_NAME OTB)
if(APPLE)
  set(PACKAGE_PLATFORM_NAME "Darwin")
elseif(WIN32)
  set(PACKAGE_PLATFORM_NAME "win")
elseif(LINUX)
  set(PACKAGE_PLATFORM_NAME "Linux")
endif()

set(PACKAGE_ARCH "32")
if(OTB_TARGET_SYSTEM_ARCH_IS_X64)
set(PACKAGE_ARCH "64")
endif()

if(OTB_MINGW)
  get_filename_component(MXE_MXEROOT ${MXE_TARGET_DIR} PATH)
  get_filename_component(MXE_MXEROOT ${MXE_MXEROOT} PATH)
  set(EXTRA_CACHE_CONFIG
    "set(CMAKE_CROSSCOMPILING TRUE)
     set(WIN32 TRUE)
     set(MINGW TRUE)
     set(MXE_MXEROOT \"${MXE_MXEROOT}\")"
    )
  set(CMAKE_INSTALL_PREFIX "${OTB_INSTALL_DIR}")  
endif()

# get itk version from External_itk.cmake if not on mingw
# because mxe has different version
if(NOT OTB_MINGW)
  get_version(
    INPUT_FILE   "${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake"
    MATCH_STRINGS "SB_ITK_VERSION_MAJOR" "SB_ITK_VERSION_MINOR"
    PREFIX PKG
    )
endif()

set(PKG_ITK_SB_VERSION "${PKG_SB_ITK_VERSION_MAJOR}.${PKG_SB_ITK_VERSION_MINOR}")

#get version of OTB from source_dir/CMakeLists.txt
get_version(
  INPUT_FILE   "${PACKAGE_OTB_SRC_DIR}/CMakeLists.txt"
  MATCH_STRINGS "OTB_VERSION_MAJOR" "OTB_VERSION_MINOR" "OTB_VERSION_PATCH"
  PREFIX PKG
  )

set(PKG_OTB_VERSION_STRING
  ${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}.${PKG_OTB_VERSION_PATCH})

if(OTB_MINGW)
  set(OTB_BINARY_DIR              "${OTB_BINARY_DIR}")
  set(OTB_INSTALL_DIR             "${OTB_INSTALL_DIR}")
  set(DEPENDENCIES_INSTALL_DIR    "${MXE_MXEROOT}/usr/${OTB_TARGET_SYSTEM_ARCH}-w64-mingw32.shared")
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
