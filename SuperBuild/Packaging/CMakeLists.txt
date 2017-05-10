#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

##### check if standalone project ######
set(OUT_OF_SOURCE_BUILD FALSE)
if(NOT PROJECT_NAME)
  cmake_minimum_required(VERSION 2.6)
  include(CMakeParseArguments)
  project(OTBPackaging)

  option(GENERATE_PACKAGE "Generate OTB package" ON)
  
  set(OUT_OF_SOURCE_BUILD TRUE)

  if(BUILD_TESTING)
    include(CTest)
  endif()
  
  if(NOT PACKAGE_OTB_SRC_DIR)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${OTBPackaging_SOURCE_DIR} PATH)
    get_filename_component(PACKAGE_OTB_SRC_DIR ${PACKAGE_OTB_SRC_DIR} PATH)
  endif()
  
  option(OTB_WRAP_PYTHON "enable python wrapping" ON)

  set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
  set(SUPERBUILD_INSTALL_DIR "" CACHE PATH "path to superbuild install directory")
  set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild download archives")

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

if(NOT GENERATE_PACKAGE)
  #well. we don't have any businees here. Let's move on..
  return()
endif()

if(OTB_WRAP_PYTHON AND NOT PYTHON_EXECUTABLE)
  find_package ( PythonInterp REQUIRED )
  mark_as_advanced(PYTHON_INCLUDE_DIR)
  mark_as_advanced(PYTHON_LIBRARY)
endif()

set(LINUX FALSE)
if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
  set(LINUX TRUE)
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

if(OTB_WRAP_PYTHON)
  set(required_vars PYTHON_EXECUTABLE)
endif()

foreach(req
    ${required_vars}
    SUPERBUILD_BINARY_DIR
    SUPERBUILD_INSTALL_DIR
    SUPERBUILD_SOURCE_DIR
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

set(CMAKE_INSTALL_PREFIX "${SUPERBUILD_INSTALL_DIR}")

set(PACKAGE_LONG_NAME OrfeoToolBox)

# Make up part of output package file
set(NAME_SUFFIX "" CACHE STRING "extra suffix for package name")

set(PACKAGE_NAME OTB)

if(NAME_SUFFIX)
  set(PACKAGE_NAME OTB${NAME_SUFFIX})
endif()

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


# get itk version from External_itk.cmake if not on mingw
# because mxe has different version
get_version(
  INPUT_FILE   "${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake"
  MATCH_STRINGS "SB_ITK_VERSION_MAJOR" "SB_ITK_VERSION_MINOR"
  PREFIX PKG
  )
set(PKG_ITK_SB_VERSION "${PKG_SB_ITK_VERSION_MAJOR}.${PKG_SB_ITK_VERSION_MINOR}")

#get version of OTB from source_dir/CMakeLists.txt
get_version(
  INPUT_FILE   "${PACKAGE_OTB_SRC_DIR}/CMakeLists.txt"
  MATCH_STRINGS "OTB_VERSION_MAJOR" "OTB_VERSION_MINOR" "OTB_VERSION_PATCH"
  PREFIX PKG
  )

set(PKG_OTB_VERSION_STRING ${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}.${PKG_OTB_VERSION_PATCH})

set(OTB_BINARY_DIR            "${SUPERBUILD_BINARY_DIR}/OTB/build")
set(OTB_INSTALL_DIR           "${SUPERBUILD_INSTALL_DIR}")
set(DEPENDENCIES_INSTALL_DIR  "${SUPERBUILD_INSTALL_DIR}")
set(QT_PLUGINS_DIR            "${SUPERBUILD_INSTALL_DIR}/plugins")
set(QT_TRANSLATIONS_DIR       "${SUPERBUILD_INSTALL_DIR}/translations")


#create cmake project and custom targets. c.f CreateCMakeProjects.cmake
CreateCMakeProjects()
