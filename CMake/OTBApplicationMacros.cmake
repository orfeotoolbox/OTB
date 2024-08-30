#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

macro(otb_create_application)
   cmake_parse_arguments(APPLICATION  "" "NAME;BUILD_PATH;INSTALL_PATH" "SOURCES;INCLUDE_DIRS;LINK_LIBRARIES" ${ARGN} )

   set( APPLICATION_TARGET_NAME otbapp_${APPLICATION_NAME} )

   # Build the library as a MODULE (shared lib even if OTB is built statically)
   include_directories(${APPLICATION_INCLUDE_DIRS})
   add_library(${APPLICATION_TARGET_NAME} MODULE ${APPLICATION_SOURCES})
   target_link_libraries(${APPLICATION_TARGET_NAME} ${APPLICATION_LINK_LIBRARIES})
   if(otb-module)
     otb_module_target_label(${APPLICATION_TARGET_NAME})
   endif()

   # Setup build output location
   # Do not output in the standard lib folder where all shared libs goes.
   # This is to avoid the application factory to look into each and every shared lib
   # for itkLoad symbol
   if(otb-module)
     set_property(TARGET ${APPLICATION_TARGET_NAME} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/otb/applications)
   endif()

   # Remove the usual 'lib' prefix to make it clear it is a plugin
   # and not a shared library to link against
   set_property(TARGET ${APPLICATION_TARGET_NAME} PROPERTY PREFIX "")

   # When called from the OTB build system, use OTB_INSTALL_APP_DIR
   if (NOT APPLICATION_INSTALL_PATH AND OTB_INSTALL_APP_DIR)
     set(APPLICATION_INSTALL_PATH ${OTB_INSTALL_APP_DIR})
   endif()

   if (APPLICATION_INSTALL_PATH)
     if(otb-module)
       install(TARGETS ${APPLICATION_TARGET_NAME}
               EXPORT ${${otb-module}-targets}
               LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH}
               COMPONENT ${${otb-module}_COMPONENT})
     else()
       install(TARGETS ${APPLICATION_TARGET_NAME}
               LIBRARY DESTINATION ${APPLICATION_INSTALL_PATH}
               COMPONENT Dependencies)
     endif()
   else()
     install(TARGETS ${APPLICATION_TARGET_NAME}
             LIBRARY DESTINATION lib
             COMPONENT Dependencies)
   endif()

   # What is the path to the applications
   # a MODULE target is always treated as LIBRARY
   get_target_property(APPLICATION_BINARY_PATH ${APPLICATION_TARGET_NAME} LIBRARY_OUTPUT_DIRECTORY)

   if (NOT APPLICATION_BINARY_PATH)
     set(APPLICATION_BINARY_PATH ${CMAKE_CURRENT_BINARY_DIR})
   endif()

   if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
     set(_script_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
   else()
     set(_script_output_dir ${CMAKE_BINARY_DIR}/bin)
   endif()
   if(OTB_INSTALL_RUNTIME_DIR)
     set(_script_install_dir ${OTB_INSTALL_RUNTIME_DIR})
   else()
     set(_script_install_dir bin)
   endif()
   set(INTERMEDIATE_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
   set(SCRIPT_EXT "")
   if(WIN32)
     set(SCRIPT_EXT ".bat")
   endif()

   # ----- Create and install launcher scripts ------
   foreach(type CLI)
     string(TOLOWER "${type}" type_lower)
     set(SCRIPT_NAME otb${type_lower}_${APPLICATION_NAME}${SCRIPT_EXT})
     otb_write_app_launcher(
       NAME ${APPLICATION_NAME}
       OUTPUT ${INTERMEDIATE_DIR}/${SCRIPT_NAME}
       TYPE ${type})
     # Copy it next to the application shared lib, and give executable rights
     file(COPY ${INTERMEDIATE_DIR}/${SCRIPT_NAME}
          DESTINATION ${_script_output_dir}
          FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
     # Install a version of this script if we are inside the OTB build
     install(PROGRAMS ${_script_output_dir}/${SCRIPT_NAME}
             DESTINATION ${_script_install_dir}
             COMPONENT ${${otb-module}_COMPONENT})
   endforeach()

   list(APPEND OTB_APPLICATIONS_NAME_LIST ${APPLICATION_NAME})
   list(REMOVE_DUPLICATES OTB_APPLICATIONS_NAME_LIST)
   set(OTB_APPLICATIONS_NAME_LIST ${OTB_APPLICATIONS_NAME_LIST}
       CACHE STRING "List of all applications" FORCE)
endmacro()

macro(otb_test_application)
  cmake_parse_arguments(TESTAPPLICATION  "" "NAME;APP" "OPTIONS;TESTENVOPTIONS;VALID" ${ARGN} )
  if(otb-module)
    otb_add_test(NAME ${TESTAPPLICATION_NAME}
                  COMMAND otbTestDriver
                  ${TESTAPPLICATION_VALID}
                  Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                  ${TESTAPPLICATION_APP}
                  $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
                  ${TESTAPPLICATION_OPTIONS}
                  -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
    # Be sure that the ${otb-module}-all target triggers the build of commandline launcher and testdriver
    add_dependencies(${otb-module}-all otbApplicationLauncherCommandLine)
    add_dependencies(${otb-module}-all otbTestDriver)
  else()
    add_test(NAME ${TESTAPPLICATION_NAME}
            COMMAND otbTestDriver
            ${TESTAPPLICATION_VALID}
            Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
            ${TESTAPPLICATION_APP}
            $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
            ${TESTAPPLICATION_OPTIONS}
            -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
  endif()
endmacro()

macro(otb_write_app_launcher)
  cmake_parse_arguments(APPLAUNCHER  "" "NAME;OUTPUT;TYPE" "" ${ARGN} )
  if("${APPLAUNCHER_TYPE}" STREQUAL "CLI")
    set(_launcher_type "otbcli")
  else()
    message(FATAL_ERROR "Unknown launcher type : ${APPLAUNCHER_TYPE}, only support CLI")
  endif()

  if(WIN32)
    # Launcher script in Batch format
    file(WRITE "${APPLAUNCHER_OUTPUT}"
"@echo off
::
:: Autogenerated by OTB installation process
:: DO NOT MODIFY
::
set CURRENT_SCRIPT_DIR=%~dp0
if exist %CURRENT_SCRIPT_DIR%${_launcher_type}.bat (
  :: Prefer using the launcher inside the script dir
  set OTB_LAUNCHER=%CURRENT_SCRIPT_DIR%${_launcher_type}.bat
) else (
  :: Use the one from the PATH
  set OTB_LAUNCHER=${_launcher_type}.bat
)

:: start the application
%OTB_LAUNCHER% ${APPLAUNCHER_NAME} %*
")
  else()
    # Launcher script in Shell format
    file(WRITE "${APPLAUNCHER_OUTPUT}"
"#!/bin/sh
#
# Autogenerated by OTB installation process
# DO NOT MODIFY
#
CURRENT_SCRIPT_DIR=$(dirname \"\$0\")
if [ -e \"\$CURRENT_SCRIPT_DIR/${_launcher_type}\" ] ; then
  # Prefer using the launcher inside the script dir
  OTB_LAUNCHER=$CURRENT_SCRIPT_DIR/${_launcher_type}
else
  # Use the one from the PATH
  OTB_LAUNCHER=${_launcher_type}
fi

# start the application
\$OTB_LAUNCHER ${APPLAUNCHER_NAME} \"$@\"
")
  endif()
endmacro()
