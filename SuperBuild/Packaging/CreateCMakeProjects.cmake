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

function(CreateCMakeProjects)

  message(STATUS "Configuring package")

  set(PATCHELF_PROGRAM)
  set(MAKESELF_SCRIPT)
  include(${SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)
  
  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE)
  
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build")
  
  set(archive_name ${PACKAGE_NAME}-${PKG_OTB_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  if(WIN32)
    set(PACKAGE_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${archive_name}.zip")
  else()
    set(PACKAGE_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${archive_name}.run")
  endif()

    
  #set archive name inside loop
  file(WRITE "${PACKAGE_PROJECT_DIR}/src/CMakeLists.txt"
  "cmake_minimum_required(VERSION 3.2)
   include(CMakeParseArguments)
   include(CMakeDetermineSystem)
   set(CMAKE_BUILD_TYPE              Release)
   set(PACKAGE_OTB_SRC_DIR           \"${PACKAGE_OTB_SRC_DIR}\")
   set(SUPERBUILD_BINARY_DIR         \"${SUPERBUILD_BINARY_DIR}\")
   set(OTB_BINARY_DIR                \"${OTB_BINARY_DIR}\")
   set(OTB_INSTALL_DIR               \"${OTB_INSTALL_DIR}\")
   set(QT_PLUGINS_DIR                \"${QT_PLUGINS_DIR}\")
   set(QT_TRANSLATIONS_DIR           \"${QT_TRANSLATIONS_DIR}\")
   set(DEPENDENCIES_INSTALL_DIR      \"${DEPENDENCIES_INSTALL_DIR}\")
   set(PACKAGE_SUPPORT_FILES_DIR     \"${PACKAGE_OTB_SRC_DIR}/SuperBuild/Packaging/Files\")
   set(CMAKE_INSTALL_PREFIX          \"${CMAKE_INSTALL_PREFIX}\")
   set(PKG_ITK_SB_VERSION            \"${PKG_ITK_SB_VERSION}\")
   set(PKG_OTB_VERSION_MAJOR         \"${PKG_OTB_VERSION_MAJOR}\")
   set(PKG_OTB_VERSION_MINOR         \"${PKG_OTB_VERSION_MINOR}\")
   set(PKG_OTB_VERSION_PATCH         \"${PKG_OTB_VERSION_PATCH}\")
   set(PKG_OTB_VERSION_STRING        \"${PKG_OTB_VERSION_STRING}\")
   set(PYTHON_EXECUTABLE             \"${PYTHON_EXECUTABLE}\")
   set(PATCHELF_PROGRAM              \"${PATCHELF_PROGRAM}\")
   set(OTB_TARGET_SYSTEM_ARCH        ${OTB_TARGET_SYSTEM_ARCH})
   set(OTB_TARGET_SYSTEM_ARCH_IS_X64 ${OTB_TARGET_SYSTEM_ARCH_IS_X64})   
   set(OTB_WRAP_PYTHON               ${OTB_WRAP_PYTHON})
   set(PKG_DEBUG                     ${PKG_DEBUG})
   set(FILE_COMMAND                  \"${FILE_COMMAND}\")
   include(${SUPERBUILD_SOURCE_DIR}/Packaging/PackageMacros.cmake)
   include(${SUPERBUILD_SOURCE_DIR}/Packaging/PackageHelper.cmake)
   include(${SUPERBUILD_SOURCE_DIR}/Packaging/InstallSupportFiles.cmake)
   include(${SUPERBUILD_SOURCE_DIR}/Packaging/PackageGlobals.cmake)
   macro_super_package(STAGE_DIR \"${archive_name}\")"
  )

if(APPLE)
  set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Macx}.txt)
elseif(LINUX) #not osx
  set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Linux.txt)
elseif(WIN32) #windows
  set(README_FILE ${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst/Installation_Windows.txt)
endif()

configure_file(
  "${README_FILE}"
  ${PACKAGE_PROJECT_DIR}/src/README
  )


#we assume the otb is built correctly with superbuild
if(OUT_OF_SOURCE_BUILD)
  add_custom_target(PACKAGE-check
    COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-check")
else(OUT_OF_SOURCE_BUILD)
  add_custom_target(PACKAGE-check
    COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-check"
    DEPENDS OTB
    WORKING_DIRECTORY ${SUPERBUILD_BINARY_DIR}
    )      
endif(OUT_OF_SOURCE_BUILD)


add_dependencies(PACKAGE-check PACKAGE-TOOLS)


#configure
add_custom_target(PACKAGE-configure
  COMMAND ${CMAKE_COMMAND} -E remove "${PACKAGE_OUTPUT_FILE}"
  COMMAND ${CMAKE_COMMAND} -E make_directory  "${PACKAGE_PROJECT_DIR}/build"
  WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}"
  COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}"
  "${PACKAGE_PROJECT_DIR}/src"
  WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
  DEPENDS PACKAGE-check
  )

#build
add_custom_target(PACKAGE-build
  COMMAND ${CMAKE_COMMAND}
  "--build" "${PACKAGE_PROJECT_DIR}/build" "--target" "install"
  WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
  DEPENDS PACKAGE-configure
  )

#create package
# creation of package is different from windows and unix like
# WORKING_DIRECTORY must be CMAKE_INSTALL_PREFIX and not
# CMAKE_CURRENT_BINARY_DIR like in unix
if(WIN32)
  add_custom_target(PACKAGE
    COMMAND ${ZIP_EXECUTABLE} "a" "-r" "-y"
    "${PACKAGE_OUTPUT_FILE}" "${archive_name}/*"
    WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
    DEPENDS PACKAGE-build
    COMMENT "Creating ${PACKAGE_OUTPUT_FILE}"
    )
else()
  add_custom_target(PACKAGE
    COMMAND ${MAKESELF_SCRIPT}
    "--quiet"
    "--target"
    "${archive_name}"
    "${CMAKE_INSTALL_PREFIX}/${archive_name}"
    "${archive_name}.run"
    "${PACKAGE_LONG_NAME} ${PKG_OTB_VERSION_STRING}"
    "./pkgsetup"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS PACKAGE-build
    COMMENT "Creating ${PACKAGE_OUTPUT_FILE}"
    )
endif()

#clean
add_custom_target(PACKAGE-clean
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS"
  COMMAND ${CMAKE_COMMAND} -E remove "${PACKAGE_OUTPUT_FILE}"
  COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )


endfunction()

