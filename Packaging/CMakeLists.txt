#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

cmake_minimum_required(VERSION 3.3.0)

project(OTBPackaging)

include(CMakeParseArguments)
include(CMakeDetermineSystem)
include(GetPrerequisites)
include(CTest)

#setting this variable prints a lot of debug information
set(PKG_DEBUG OFF)
#set( PKG_DEBUG 1)

set(CMAKE_BUILD_TYPE    Release)

#user-defined variables and option
option(OTB_WRAP_PYTHON "include python bindings in package" ON)
set(SUPERBUILD_BINARY_DIR "" CACHE PATH "path to superbuild binary directory")
set(SUPERBUILD_INSTALL_DIR "" CACHE PATH "path to superbuild install directory")
set(DOWNLOAD_LOCATION "" CACHE PATH "path to superbuild download archives")

#variables based on above user-defined variables
message( "OTB_BINARY_DIR=${OTB_BINARY_DIR}" )
if ( NOT OTB_BINARY_DIR )
  set(OTB_BINARY_DIR            "${SUPERBUILD_BINARY_DIR}/OTB/build")
endif()
set(QT_TRANSLATIONS_DIR       "${SUPERBUILD_INSTALL_DIR}/translations")
get_filename_component(PACKAGE_OTB_SRC_DIR ${OTBPackaging_SOURCE_DIR} PATH)
set(SUPERBUILD_SOURCE_DIR     "${PACKAGE_OTB_SRC_DIR}/SuperBuild")


set(LINUX FALSE)
if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
  set(LINUX TRUE)
endif()

find_program(FILE_COMMAND NAMES  "file")
find_program(READLINK NAMES "readlink")
find_program(ZIP_EXECUTABLE NAMES 7z 7za)

if ( NOT CMAKE_INSTALL_PREFIX )
  set(CMAKE_INSTALL_PREFIX "${SUPERBUILD_INSTALL_DIR}")
endif()

#set CMAKE_MODULE_PATH to be the current source directory.
# This will allow including all cmake files without full path and .cmake extension!
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#include superbuild_macro for get_version()
include(${SUPERBUILD_SOURCE_DIR}/CMake/SuperBuild_Macro.cmake)

#include superbuild_macro for OTB_TARGET_SYSTEM_ARCH_IS_X64 value
include(${PACKAGE_OTB_SRC_DIR}/CMake/OTBCheckTargetSystemArch.cmake)

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

set(PACKAGE_LONG_NAME OrfeoToolBox)
set(NAME_SUFFIX "" CACHE STRING "extra suffix for package name")
set(PACKAGE_NAME OTB)
if(APPLE)
  set(PACKAGE_PLATFORM_NAME "Darwin")
elseif(WIN32)
  set(PACKAGE_PLATFORM_NAME "Win")
elseif(LINUX)
  set(PACKAGE_PLATFORM_NAME "Linux")
endif()
set(PACKAGE_ARCH "32")
if(OTB_TARGET_SYSTEM_ARCH_IS_X64)
set(PACKAGE_ARCH "64")
endif()

# This directory is important.
# We stage/keep files that goes into final package in this directory
set(PKG_STAGE_DIR ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH}${NAME_SUFFIX})

set(PATCHELF_PROGRAM "${CMAKE_BINARY_DIR}/PATCHELF/src/patchelf/src/patchelf")
include(External_patchelf)

include(PackageGlobals)

# Note that MANY or ALL of function use global variables set in this CMakeLists.txt
# The order in which the functions in these "included" files are called is only important.
# We tried not to share variables which are created inside functions or macro. In such cases,
# function return the value. 

include(setif_value_in_list)
include(get_variables_ending_with)
include(clear_cmakecache_variables)
include(check_cmake_variables)
include(configure_loader)
include(search_library)
include(prepare_search_dirs)
include(prepare_file_list)
include(process_file_recurse)
include(install_without_message)
include(install_rule)
include(isfile_symlink)
include(detect_using_file_command)
include(install_otbapp_wrapper_scripts)
include(cleanup_package)

# below functions include many install commands which are installing
# files under PKG_STAGE_DIR
include(install_include_dirs)
include(install_importlibs)
include(install_python_bindings)
include(install_share_dirs)
include(install_cmake_files)
include(install_qtdev_files)
include(install_vstudio_files)
include(installer_files)
include(create_package)

configure_loader(LOADER_PROGRAM LOADER_ARGS LOADER_REGEX)


# creation of package is different from windows and unix like
# WE KEEP THIS VARIABLE HERE BECAUSE IT IS USED IN
# cleanup_package, create_package, testing.cmake
# and check_cmake_variables

# THIS IS JUST TO AVOID DUPLICATION
if(WIN32)
  set(PACKAGE_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PKG_STAGE_DIR}.zip")
else()
  set(PACKAGE_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PKG_STAGE_DIR}.run")
endif()

check_cmake_variables()

cleanup_package()

set(HAVE_QT FALSE CACHE INTERNAL "HAVE_QT")
if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/otbApplicationLauncherQt${EXE_EXT}")
  set(HAVE_QT TRUE)
endif()

set(HAVE_MVD FALSE  CACHE INTERNAL "HAVE_MVD")
if(EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/monteverdi${EXE_EXT}")
  set(HAVE_MVD TRUE)
endif()

set(HAVE_PYTHON FALSE CACHE INTERNAL "Python wrappings")
if(EXISTS "${SUPERBUILD_INSTALL_DIR}/lib/otb/python/_otbApplication${PYMODULE_EXT}")
  set(HAVE_PYTHON TRUE)
endif()

#only for *nix
if(UNIX)
  file(WRITE ${CMAKE_BINARY_DIR}/make_symlinks   "#!/bin/sh\n")
endif()

#remove all *_USED|*_RESOLVED variables from CMakeCache.txt
clear_cmakecache_variables()

set(PKG_SEARCHDIRS)
prepare_search_dirs(PKG_SEARCHDIRS)

set(PKG_PEFILES)
prepare_file_list(PKG_PEFILES)
foreach(pfile ${PKG_PEFILES})
  process_file_recurse(${pfile})
endforeach()

install_include_dirs()

install_importlibs()

install_python_bindings()

install_java_bindings()

install_share_dirs()

install_cmake_files()

install_qtdev_files()

install_vstudio_files()

installer_files()

create_package()

if(BUILD_TESTING)
  enable_testing()
  include(testing.cmake)
endif()




#TODO: dummy targets. to be removed after merge
add_custom_target(
    PACKAGE-OTB
    COMMAND
    "${CMAKE_COMMAND}" --build . --target install
    WORKING_DIRECTORY
    "${CMAKE_BINARY_DIR}"
    COMMENT "dummy target PACKAGE-OTB"
    )
