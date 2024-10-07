#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#Adapted from ITKModuleExternal.cmake
# This file ensures the appropriate variables are set up for a project extending
# OTB before including OTBModuleMacros. This is the preferred way to build an
# OTB module outside of the OTB source tree.

include(GenerateExportHeaderCustom)

macro(otb_module_test)
  include(../otb-module.cmake) # Load module meta-data
  set(${otb-module-test}_LIBRARIES "")
  foreach(dep IN LISTS OTB_MODULE_${otb-module-test}_DEPENDS)
    list(APPEND ${otb-module-test}_LIBRARIES "${${dep}_LIBRARIES}")
  endforeach()
endmacro()


function(otb_add_test)
  set(largeinput_regex "LARGEINPUT{([^;{}\r\n]*)}")

  set(_depends_on_largeinput OFF)
  foreach(arg IN LISTS ARGN)
     if("x${arg}" MATCHES "${largeinput_regex}")
       string(REGEX REPLACE "${largeinput_regex}" "\\1" largeinput_relative_path "${arg}")
       set(_fullpath "${OTB_DATA_LARGEINPUT_ROOT}/${largeinput_relative_path}")
       list(APPEND _out_arg ${_fullpath})
       set(_depends_on_largeinput ON)
     else()
       list(APPEND _out_arg ${arg})
     endif()
  endforeach()

  if (_depends_on_largeinput AND NOT OTB_DATA_USE_LARGEINPUT)
    return()
  endif()

  add_test(${_out_arg})

  if("NAME" STREQUAL "${ARGV0}")
    set(_iat_testname ${ARGV1})
  else()
    set(_iat_testname ${ARGV0})
  endif()

  if(otb-module)
    set(_label ${otb-module})
  else()
    set(_label ${main_project_name})
  endif()

  set_property(TEST ${_iat_testname} PROPERTY LABELS ${_label})

endfunction()


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

#-----------------------------------------------------------------------------
# Function otb_add_test_mpi to run mpi tests
function(otb_add_test_mpi)
   set( _OPTIONS_ARGS )
   set( _ONE_VALUE_ARGS NAME NBPROCS COMMAND)
   set( _MULTI_VALUE_ARGS EXTRA_OPT)
   cmake_parse_arguments( TEST_MPI "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN} )

   # Test nb procs
   if( NOT TEST_MPI_NBPROCS )
     set(TEST_MPI_NBPROCS 2)
   endif()
   # Test command line
   foreach(arg IN LISTS TEST_MPI_UNPARSED_ARGUMENTS)
     list(APPEND ARGS ${arg})
   endforeach()
   set (test_parameters -n ${TEST_MPI_NBPROCS} ${OTB_MPIEXEC_OPT} ${TEST_MPI_EXTRA_OPT} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_MPI_COMMAND} ${ARGS})
   otb_add_test(NAME ${TEST_MPI_NAME} COMMAND ${MPIEXEC} ${test_parameters})
endfunction()


function(otb_module_target_label _target_name)
  if(otb-module)
    set(_label ${otb-module})
    if(TARGET ${otb-module}-all)
      add_dependencies(${otb-module}-all ${_target_name})
    endif()
  else()
    set(_label ${_OTBModuleMacros_DEFAULT_LABEL})
  endif()
  set_property(TARGET ${_target_name} PROPERTY LABELS ${_label})
endfunction()

# Set the VERSION, SOVERSION (both for libs) and OUTPUT_NAME Target properties
# of target "_name"
# VERSION and SOVERSION are set accordingly:
# - If it is an OTB module or OTB P0 module, the otb version is used
# - If set the project version is used
# - Otherwise it is versioned 1.0.0
# OUTPUT_NAME is "_name" prefixed with "otb" if not already in "_name"
function(otb_module_target_name _name)
  set(__major_version 1)
  set(__minor_version 0)
  set(__patch_version 0)

  # if we are in OTB build tree or using a P0 module
  if (OTB_SOURCE_DIR OR IS_P0_MODULE)
    set(__major_version ${OTB_VERSION_MAJOR})
    set(__minor_version ${OTB_VERSION_MINOR})
    set(__patch_version ${OTB_VERSION_PATCH})
  else() # not OTB or P0 module, use project vars if exists
    if (PROJECT_VERSION_MAJOR)
      set(__major_version ${PROJECT_VERSION_MAJOR})
    endif()
    if (PROJECT_VERSION_MINOR)
      set(__minor_version ${PROJECT_VERSION_MINOR})
    endif()
    if (PROJECT_VERSION_PATCH)
      set(__patch_version ${PROJECT_VERSION_PATCH})
    endif()
  endif()

  get_property(_target_type TARGET ${_name} PROPERTY TYPE)
  if (NOT ${_target_type} STREQUAL "EXECUTABLE")
    if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
      set_property(TARGET ${_name} PROPERTY VERSION ${__major_version}.${__minor_version}.${__patch_version})
      set_property(TARGET ${_name} PROPERTY SOVERSION ${__major_version})
    endif()
    if("${_name}" MATCHES "^[Oo][Tt][Bb]")
      set(_otb "")
    else()
      set(_otb "otb")
    endif()
    set_property(TARGET ${_name} PROPERTY OUTPUT_NAME ${_otb}${_name})
  endif()
  unset(__major_version)
  unset(__minor_version)
  unset(__patch_version)
endfunction()

function(otb_module_target_install _name _component)
  #Use specific runtime components for executables and libraries separately when installing a module,
  #considering that the target of a module could be either an executable or a library.
  # get_property(_ttype TARGET ${_name} PROPERTY TYPE)
  # if("${_ttype}" STREQUAL EXECUTABLE)
  #   set(runtime_component Runtime)
  # else()
  #   set(runtime_component Dependencies)
  # endif()
  include(GNUInstallDirs)
  set(${otb-module}_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})
  # by default name the target depending of project
  set(__export_name ${PROJECT_NAME}Targets)
  if (_component)
    set(__export_name ${_component}Targets)
  endif()
  if (CMAKE_DEBUG)
    message(STATUS "[CMAKE_DEBUG] In \"otb_module_target_install\" __export_name == ${__export_name}")
  endif()
  # By default set the install path of target file to lib/cmake/<project_name>
  set(target_file_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

  if (${IS_P0_MODULE})
    set(${otb-module}_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
    set(target_file_dir "${CMAKE_INSTALL_LIBDIR}/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  if (_component)
    install(TARGETS ${_name}
      EXPORT  ${__export_name}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${_component}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${_component}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${_component}
      INCLUDES DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR}
    )
  else()
    install(TARGETS ${_name}
            EXPORT  ${__export_name}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR}
    )
  endif()

  get_property(is_target_exported GLOBAL PROPERTY ${__export_name}_EXPORTED DEFINED)
  if (NOT ${is_target_exported})
    if (CMAKE_DEBUG)
      message(STATUS "[CMAKE_DEBUG] Exporting target ${__export_name} part of component ${_component} in file ${__export_name}.cmake located at ${target_file_dir}")
    endif()
    if (_component)
      install(EXPORT ${__export_name}
              FILE ${__export_name}.cmake
              DESTINATION ${target_file_dir}
              COMPONENT ${_component})
    else()
      install(EXPORT ${__export_name}
              FILE ${__export_name}.cmake
              DESTINATION ${target_file_dir})
    endif()
    set_property(GLOBAL PROPERTY ${__export_name}_EXPORTED TRUE)
  endif()
  unset(__export_name)
endfunction()

function(otb_module_target _name)
  set(_install 1)
  # by default component var is project name. It does not means the project
  # have a component. This name is used to install cmake file a correct path
  set(_component "0")
  foreach(arg ${ARGN})
    if("${arg}" MATCHES "^(NO_INSTALL)$")
      set(_install 0)
    elseif("${arg}" MATCHES "^COMPONENT_[a-zA-Z]+$")
      string(REPLACE "_" ";" _list_components ${arg})
      list(GET _list_components 1 _component)
    else()
      message(FATAL_ERROR "Unknown argument [${arg}]")
    endif()
  endforeach()
  otb_module_target_name(${_name})
  otb_module_target_label(${_name})
  if(_install)
    otb_module_target_install(${_name} ${_component})
  endif()
endfunction()

macro(otb_module _name)
  otb_module_check_name(${_name})
  set(otb-module ${_name})
  set(otb-module-test ${_name}-Test)
  set(_doing "")
  set(OTB_MODULE_${otb-module}_DECLARED 1)
  set(OTB_MODULE_${otb-module-test}_DECLARED 1)
  set(OTB_MODULE_${otb-module}_DEPENDS "")
  set(OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS "")
  set(OTB_MODULE_${otb-module-test}_DEPENDS "${otb-module}")
  set(OTB_MODULE_${otb-module}_DESCRIPTION "description")
  set(OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT 0)
  set(OTB_MODULE_${otb-module}_ENABLE_SHARED 0)
  foreach(arg ${ARGN})
    if("${arg}" MATCHES "^(DEPENDS|OPTIONAL_DEPENDS|TEST_DEPENDS|DESCRIPTION|DEFAULT|COMPONENT)$")
      set(_doing "${arg}")
    elseif("${arg}" MATCHES "^EXCLUDE_FROM_DEFAULT$")
      set(_doing "")
      set(OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT 1)
    elseif("${arg}" MATCHES "^EXCLUDE_FROM_ALL$") # To maintain backward compatibility
      set(_doing "")
      message(AUTHOR_WARNING "EXCLUDE_FROM_ALL is deprecated, please use EXCLUDE_FROM_DEFAULT.")
      set(OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT 1)
    elseif("${arg}" MATCHES "^ENABLE_SHARED$")
      set(_doing "")
      set(OTB_MODULE_${otb-module}_ENABLE_SHARED 1)
    elseif("${arg}" MATCHES "^[A-Z][A-Z][A-Z]$")
      set(_doing "")
      message(AUTHOR_WARNING "Unknown argument [${arg}]")
    elseif("${_doing}" MATCHES "^DEPENDS$")
      list(APPEND OTB_MODULE_${otb-module}_DEPENDS "${arg}")
    elseif("${_doing}" MATCHES "^OPTIONAL_DEPENDS$")
      list(APPEND OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS "${arg}")
    elseif("${_doing}" MATCHES "^TEST_DEPENDS$")
      list(APPEND OTB_MODULE_${otb-module-test}_DEPENDS "${arg}")
    elseif("${_doing}" MATCHES "^DESCRIPTION$")
      set(_doing "")
      set(OTB_MODULE_${otb-module}_DESCRIPTION "${arg}")
    elseif("${_doing}" MATCHES "^DEFAULT")
      message(FATAL_ERROR "Invalid argument [DEFAULT]")
    elseif("${_doing}" MATCHES "^COMPONENT$")
      set(_doing "")
      # Create a var to get the component per module
      # And a list to get all module per component (later used in
      # <Component>Config.cmake file)
      set(OTB_MODULE_${otb-module}_COMPONENT "${arg}")
      set_property(GLOBAL APPEND PROPERTY ${arg}_MODULE_LIST ${otb-module})
      set(OTB_MODULE_${otb-module-test}_TARGET ${arg}Targets)
    else()
      set(_doing "")
      message(AUTHOR_WARNING "Unknown argument [${arg}]")
    endif()
  endforeach()
  list(SORT OTB_MODULE_${otb-module}_DEPENDS) # Deterministic order.
  list(SORT OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS) # Deterministic order.
  list(SORT OTB_MODULE_${otb-module-test}_DEPENDS) # Deterministic order.
endmacro()

set(OTB_MODULE_ACTIVATION_OPTION_LIST "")
macro(otb_module_activation_option _option_desc _default)
  string(REGEX REPLACE "OTB(.*)" "OTB_USE_\\1" _option_name ${otb-module})
  string(TOUPPER ${_option_name} _option_name)
  option(${_option_name} ${_option_desc} ${_default})
  set(OTB_MODULE_${otb-module}_ACTIVATION_OPTION ${_option_name})
  list(APPEND OTB_MODULE_ACTIVATION_OPTION_LIST ${_option_name})
endmacro()

macro(otb_module_check_name _name)
  if( NOT "${_name}" MATCHES "^[a-zA-Z][a-zA-Z0-9]*$")
    message(FATAL_ERROR "Invalid module name: ${_name}")
  endif()
endmacro()

# This macro expect:
# - OTB_DATA_ROOT to be set as env variable or OTB installation to be ${CMAKE_CURRENT_SOURCE_DIR}/../OTB/Data
# - An otb_module.cmake file next to the file using this macro
# OUTPUTS:
# - BASELINE a path where baseline pictures are stored. Depending of OTB_DATA_ROOT
# - BASELINE_FILES a path where baseline files are stored. Depending of OTB_DATA_ROOT
# - INPUTDATA, a path depending of OTB_DATA_ROOT
# - TEMP, a path for temporary files generated during test
# - OTBAPP_BASELINE same as BASELINE but for OTB-Applications
# - OTBAPP_BASELINE_FILES same as BASELINE_FILES but for OTB-Applications
# - ${otb-module}_INSTALL_RUNTIME_DIR path to bin relative to CMAKE_INSTALL_PREFIX
# - ${otb-module}_INSTALL_LIBRARY_DIR path to lib relative to CMAKE_INSTALL_PREFIX
# - ${otb-module}_INSTALL_ARCHIVE_DIR path to lib relative to CMAKE_INSTALL_PREFIX
# - ${otb-module}_INSTALL_INCLUDE_DIR path to include relative to CMAKE_INSTALL_PREFIX
# - ${otb-module}_LIBRARIES contains the list of DEPENDENCIES
# Create a target named ${otb-module}-all
macro(otb_module_impl)
  include(GNUInstallDirs)
  # Force shared lib and testing
  OPTION(BUILD_SHARED_LIBS "Build shared libraries" ON)
  OPTION(BUILD_TESTING "Build testing" OFF)

  find_path(OTB_DATA_ROOT
  NAMES README-OTB-Data
  HINTS $ENV{OTB_DATA_ROOT} ${CMAKE_CURRENT_SOURCE_DIR}/../OTB/Data)
  mark_as_advanced(OTB_DATA_ROOT)

  set(BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB/Images)
  set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
  set(INPUTDATA      ${OTB_DATA_ROOT}/Input)
  set(TEMP           ${CMAKE_BINARY_DIR}/Testing/Temporary)
  set(OTBAPP_BASELINE       ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)
  set(OTBAPP_BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Files)

  if(BUILD_TESTING)
    enable_testing()
  endif()

  include(otb-module.cmake) # Load module meta-data
  # Now we have the otb-module var that comes from the previous cmake file reads
  set(__current_component ${OTB_MODULE_${otb-module}_COMPONENT})
  # NOTE TLA: does these variables are used? Not in other OTB files
  # but can it be used by third parties module (like SiftFast).
  # But it is strange to install other project part depending of one (except if it)
  # is OTB path
  set(${otb-module}_INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_PREFIX}/bin)
  set(${otb-module}_INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  set(${otb-module}_INSTALL_ARCHIVE_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  set(${otb-module}_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)

  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

  # Collect all sources and headers for IDE projects.
  set(_srcs "")
  if("${CMAKE_GENERATOR}" MATCHES "Xcode|Visual Studio|KDevelop"
      OR CMAKE_EXTRA_GENERATOR)
    # Add sources to the module target for easy editing in the IDE.
    set(_include ${${otb-module}_SOURCE_DIR}/include)
    if(EXISTS ${_include})
      set(_src ${${otb-module}_SOURCE_DIR}/src)
      file(GLOB_RECURSE _srcs ${_src}/*.cxx)
      file(GLOB_RECURSE _hdrs ${_include}/*.h ${_include}/*.hxx)
      list(APPEND _srcs ${_hdrs})
    endif()
  endif()

  # Create a ${otb-module}-all target to build the whole module.
  add_custom_target(${otb-module}-all ALL SOURCES ${_srcs})

  otb_module_use(${OTB_MODULE_${otb-module}_DEPENDS})

  foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
    if (${dep}_ENABLED)
      otb_module_use(${dep})
    endif()
  endforeach()

  # Define ${otb-module}_LIBRARIES with each ${${dep}_LIBRARIES} of
  # OTB_MODULE_${otb-module}_DEPENDS
  if(NOT DEFINED ${otb-module}_LIBRARIES)
    set(${otb-module}_LIBRARIES "")

    foreach(dep IN LISTS OTB_MODULE_${otb-module}_DEPENDS)
      list(APPEND ${otb-module}_LIBRARIES "${${dep}_LIBRARIES}")
    endforeach()

    foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
      if (${dep}_ENABLED)
        list(APPEND ${otb-module}_LIBRARIES "${${dep}_LIBRARIES}")
      endif()
    endforeach()

    if(${otb-module}_LIBRARIES)
      list(REMOVE_DUPLICATES ${otb-module}_LIBRARIES)
    endif()
  endif()


  set(${otb-module}_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR})

  if (${IS_P0_MODULE})
    set(${otb-module}_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR})
    message(STATUS "THIS IS AN OTB P0 MODULE! Includes are packaged in \"${${otb-module}_INSTALL_INCLUDE_DIR}\". A cmake file to get module var will be packaged in lib/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  # Add ${${otb-module}_SOURCE_DIR}/include to ${otb-module}_INCLUDE_DIRS
  # install them in the right path
  if(EXISTS ${${otb-module}_SOURCE_DIR}/include)
    list(APPEND ${otb-module}_INCLUDE_DIRS ${${otb-module}_SOURCE_DIR}/include)
    # /!\ Note the last / of directory path to avoid copying include folder
    # but only its files
    if (__current_component)
      install(DIRECTORY ${${otb-module}_SOURCE_DIR}/include/
              DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR}
              COMPONENT ${__current_component})
    else()
        install(DIRECTORY ${${otb-module}_SOURCE_DIR}/include/
                DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR})
    endif()
  endif()

  if(NOT OTB_SOURCE_DIR)
    # When building a module outside the OTB source tree, find the export
    # header.
    list(APPEND ${otb-module}_INCLUDE_DIRS ${${otb-module}_BINARY_DIR}/include)
  else()
    # if OTB_SOURCE_DIR is set all auto-generated export headers for a class
    # goes into OTBCommon_BINARY_DIR/src.
    # Hence it is required to include   ${OTBCommon_BINARY_DIR} to list of
    # ${otb-module}_INCLUDE_DIRS. Not doing this will force developer to
    # to include them explicitly for each module which can result in
    # more problems. ( stephane albert)
    list(APPEND ${otb-module}_INCLUDE_DIRS ${OTBCommon_BINARY_DIR})
  endif()

  if(${otb-module}_INCLUDE_DIRS)
    include_directories(${${otb-module}_INCLUDE_DIRS})
  endif()
  if(${otb-module}_SYSTEM_INCLUDE_DIRS)
    include_directories(${${otb-module}_SYSTEM_INCLUDE_DIRS})
  endif()

  if(${otb-module}_SYSTEM_LIBRARY_DIRS)
    link_directories(${${otb-module}_SYSTEM_LIBRARY_DIRS})
  endif()

  if(${otb-module}_THIRD_PARTY)
    otb_module_warnings_disable(C CXX)
  else()
    if(OTB_CPPCHECK_TEST)
      otb_module_cppcheck_test( ${otb-module} )
    endif()
  endif()

  if(EXISTS ${${otb-module}_SOURCE_DIR}/src/CMakeLists.txt)
    # add the current otb-module to global list property ${${otb-module}-targets}_MODULES
    set_property(GLOBAL APPEND PROPERTY ${${otb-module}-targets}_MODULES ${otb-module})
    add_subdirectory(src)
  endif()

  if(EXISTS ${${otb-module}_SOURCE_DIR}/app/CMakeLists.txt AND NOT ${otb-module}_NO_SRC)
    add_subdirectory(app)
  endif()

  if(BUILD_TESTING AND EXISTS ${${otb-module}_SOURCE_DIR}/test/CMakeLists.txt)
    add_subdirectory(test)
    endif()

  if( OTB_MODULE_${otb-module}_ENABLE_SHARED )
    if(OTB_SOURCE_DIR)
      set(_export_header_file "${OTBCommon_BINARY_DIR}/${otb-module}Export.h")
    else()
      set(_export_header_file "${${otb-module}_BINARY_DIR}/include/${otb-module}Export.h")
    endif()

    # Generate the export macro header for symbol visibility/Windows DLL declspec
    # This header is called *Modulename*Export.h in the build directory,
    # and contains defines for _EXPORT macros such as OTBApplicationEngine_EXPORT
    otb_generate_export_header(${otb-module}
      EXPORT_FILE_NAME ${_export_header_file}
      EXPORT_MACRO_NAME ${otb-module}_EXPORT
      DEPRECATED_MACRO_NAME ${otb-module}_DEPRECATED
      NO_EXPORT_MACRO_NAME ${otb-module}_HIDDEN
      STATIC_DEFINE OTB_STATIC )
    if (__current_component)
      install(FILES ${_export_header_file}
              DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR}
              COMPONENT ${__current_component})
    else()
      install(FILES ${_export_header_file}
              DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR})
    endif()
    if (BUILD_SHARED_LIBS)
      # export flags are only added when building shared libs, they cause
      # mismatched visibility warnings when building statically.
      add_compiler_export_flags(my_abi_flags)
      set_property(TARGET ${otb-module} APPEND
        PROPERTY COMPILE_FLAGS "${my_abi_flags}")
    endif()
  endif()

  set(otb-module-EXPORT_CODE-build "${${otb-module}_EXPORT_CODE_BUILD}")
  set(otb-module-EXPORT_CODE-install "${${otb-module}_EXPORT_CODE_INSTALL}")

  # TODO TLA: create a macro for this part as it is the same in otb modules
  if (${IS_P0_MODULE})
    # Define variables used by OTBModuleInfo that will be configured in ${otb-module}Info

    # Dependencies
    set(otb-module-DEPENDS "${OTB_MODULE_${otb-module}_DEPENDS}")
    foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
      if (${dep}_ENABLED)
        list(APPEND otb-module-DEPENDS ${dep})
      endif()
    endforeach()

    # Include dirs
    # As Group can be separated and installed to different places, ensure
    # that includes and library path are not relative to OTB but to Group
    # location
    # here use ${GROUP_${${otb-module}_COMPONENT}_LOCATION} that will be init
    # when reading the <GROUP>Config.cmake file to the root of the Group
    set(otb-module-INCLUDE_DIRS "\${GROUP_${__current_component}_LOCATION}/${${otb-module}_INSTALL_INCLUDE_DIR}")
    if(${otb-module}_SYSTEM_INCLUDE_DIRS)
      list(APPEND otb-module-INCLUDE_DIRS "${${otb-module}_SYSTEM_INCLUDE_DIRS}")
    endif()

    # LIBS
    set(otb-module-LIBRARIES "${${otb-module}_LIBRARIES}")
    set(otb-module-LIBRARY_DIRS "\${GROUP_${__current_component}_LOCATION}/lib")

    # add system lib dir if it exists
    if (${${otb-module}_SYSTEM_LIBRARY_DIRS})
      list(APPEND otb-module-LIBRARY_DIRS "${${otb-module}_SYSTEM_LIBRARY_DIRS}")
    endif()
    set(otb-module-EXPORT_CODE "${otb-module-EXPORT_CODE-build}")

    # create a file with variables previously fields
    # file will be installed in lib/cmake/OTB-X.X/Module
    # For P0 module the path is the same as OTB install path
    configure_file(${OTB_DIR}/OTBModuleInfo.cmake.in "${CMAKE_BINARY_DIR}/CMakeFiles/${otb-module}.cmake" @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/CMakeFiles/${otb-module}.cmake
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}/Modules"
            COMPONENT ${__current_component}}
    )
  endif()
  unset(__current_component)
endmacro() # otb_module_impl
