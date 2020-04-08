#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

get_filename_component(_OTBModuleMacros_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

set(_OTBModuleMacros_DEFAULT_LABEL "OTBModular")

include(${_OTBModuleMacros_DIR}/OTBModuleAPI.cmake)
include(${_OTBModuleMacros_DIR}/OTBModuleDoxygen.cmake)
include(${_OTBModuleMacros_DIR}/OTBModuleHeaderTest.cmake)
include(${_OTBModuleMacros_DIR}/OTBApplicationMacros.cmake)

# TODO check if this is still the case:
if(APPLE)
  #RK:  compiler visibility nor woking on osx with appleclang xcode.
  #gcc is a symlink to clang
  set( USE_COMPILER_HIDDEN_VISIBILITY OFF CACHE INTERNAL "" )
endif()

include(GenerateExportHeaderCustom)

if(OTB_CPPCHECK_TEST)
  include(${_OTBModuleMacros_DIR}/OTBModuleCPPCheckTest.cmake)
endif()

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
    if("${arg}" MATCHES "^(DEPENDS|OPTIONAL_DEPENDS|TEST_DEPENDS|DESCRIPTION|DEFAULT)$")
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
    elseif("${arg}" MATCHES "^DEPRECATED$")
      set(_doing "")
      set(OTB_MODULE_${otb-module}_IS_DEPRECATED 1)
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

macro(otb_module_impl)
  include(otb-module.cmake) # Load module meta-data
  set(${otb-module}_INSTALL_RUNTIME_DIR ${OTB_INSTALL_RUNTIME_DIR})
  set(${otb-module}_INSTALL_LIBRARY_DIR ${OTB_INSTALL_LIBRARY_DIR})
  set(${otb-module}_INSTALL_ARCHIVE_DIR ${OTB_INSTALL_ARCHIVE_DIR})
  set(${otb-module}_INSTALL_INCLUDE_DIR ${OTB_INSTALL_INCLUDE_DIR})

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

  if(EXISTS ${${otb-module}_SOURCE_DIR}/include)
    list(APPEND ${otb-module}_INCLUDE_DIRS ${${otb-module}_SOURCE_DIR}/include)
    install(DIRECTORY include/ DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR} COMPONENT Development)
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
    if(EXISTS "${${otb-module}_SOURCE_DIR}/include" AND BUILD_TESTING)
      otb_module_headertest(${otb-module})
    endif()
  endif()

  if(EXISTS ${${otb-module}_SOURCE_DIR}/src/CMakeLists.txt)
    set_property(GLOBAL APPEND PROPERTY OTBTargets_MODULES ${otb-module})
    add_subdirectory(src)
  endif()

  if(EXISTS ${${otb-module}_SOURCE_DIR}/app/CMakeLists.txt AND NOT ${otb-module}_NO_SRC)
    add_subdirectory(app)
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
    generate_export_header(${otb-module}
      EXPORT_FILE_NAME ${_export_header_file}
      EXPORT_MACRO_NAME ${otb-module}_EXPORT
      DEPRECATED_MACRO_NAME ${otb-module}_DEPRECATED
      NO_EXPORT_MACRO_NAME ${otb-module}_HIDDEN
      STATIC_DEFINE OTB_STATIC )
    install(FILES
      ${_export_header_file}
      DESTINATION ${${otb-module}_INSTALL_INCLUDE_DIR}
      COMPONENT Development
      )

    if (BUILD_SHARED_LIBS)
      # export flags are only added when building shared libs, they cause
      # mismatched visibility warnings when building statically.
      if (USE_COMPILER_HIDDEN_VISIBILITY)
        # Prefer to use target properties supported by newer cmake
        set_target_properties(${otb-module} PROPERTIES CXX_VISIBILITY_PRESET hidden)
        set_target_properties(${otb-module} PROPERTIES C_VISIBILITY_PRESET hidden)
        set_target_properties(${otb-module} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
      endif()
    endif()
  endif()

  set(otb-module-EXPORT_CODE-build "${${otb-module}_EXPORT_CODE_BUILD}")
  set(otb-module-EXPORT_CODE-install "${${otb-module}_EXPORT_CODE_INSTALL}")

  set(otb-module-DEPENDS "${OTB_MODULE_${otb-module}_DEPENDS}")
  foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
    if (${dep}_ENABLED)
      list(APPEND otb-module-DEPENDS ${dep})
    endif()
  endforeach()
  set(otb-module-LIBRARIES "${${otb-module}_LIBRARIES}")
  set(otb-module-INCLUDE_DIRS-build "${${otb-module}_INCLUDE_DIRS}")
  set(otb-module-INCLUDE_DIRS-install "\${OTB_INSTALL_PREFIX}/${${otb-module}_INSTALL_INCLUDE_DIR}")
  if(${otb-module}_SYSTEM_INCLUDE_DIRS)
    list(APPEND otb-module-INCLUDE_DIRS-build "${${otb-module}_SYSTEM_INCLUDE_DIRS}")
    list(APPEND otb-module-INCLUDE_DIRS-install "${${otb-module}_SYSTEM_INCLUDE_DIRS}")
  endif()
  set(otb-module-LIBRARY_DIRS "${${otb-module}_SYSTEM_LIBRARY_DIRS}")
  set(otb-module-INCLUDE_DIRS "${otb-module-INCLUDE_DIRS-build}")
  set(otb-module-EXPORT_CODE "${otb-module-EXPORT_CODE-build}")
  configure_file(${_OTBModuleMacros_DIR}/OTBModuleInfo.cmake.in ${OTB_MODULES_DIR}/${otb-module}.cmake @ONLY)
  set(otb-module-INCLUDE_DIRS "${otb-module-INCLUDE_DIRS-install}")
  set(otb-module-EXPORT_CODE "${otb-module-EXPORT_CODE-install}")
  configure_file(${_OTBModuleMacros_DIR}/OTBModuleInfo.cmake.in CMakeFiles/${otb-module}.cmake @ONLY)
  install(FILES
    ${${otb-module}_BINARY_DIR}/CMakeFiles/${otb-module}.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR}/Modules
    COMPONENT Development
    )
  otb_module_doxygen(${otb-module})   # module name
endmacro()

macro(otb_module_test)
  include(../otb-module.cmake) # Load module meta-data
  set(${otb-module-test}_LIBRARIES "")
  otb_module_use(${OTB_MODULE_${otb-module-test}_DEPENDS})
  foreach(dep IN LISTS OTB_MODULE_${otb-module-test}_DEPENDS)
    list(APPEND ${otb-module-test}_LIBRARIES "${${dep}_LIBRARIES}")
  endforeach()
  # make sure the test can link with optional libs
  foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
    if (${dep}_ENABLED)
      list(APPEND ${otb-module-test}_LIBRARIES "${${dep}_LIBRARIES}")
    endif()
  endforeach()
endmacro()

macro(otb_module_warnings_disable)
  foreach(lang ${ARGN})
    if(MSVC)
      string(REGEX REPLACE "(^| )[/-]W[0-4]( |$)" " "
        CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w")
    elseif(BORLAND)
      set(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w-")
    else()
      set(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w")
    endif()
  endforeach()
endmacro()

macro(otb_module_target_label _target_name)
  if(otb-module)
    set(_label ${otb-module})
    if(TARGET ${otb-module}-all)
      add_dependencies(${otb-module}-all ${_target_name})
    endif()
  else()
    set(_label ${_OTBModuleMacros_DEFAULT_LABEL})
  endif()
  set_property(TARGET ${_target_name} PROPERTY LABELS ${_label})
endmacro()

macro(otb_module_target_name _name)
  get_property(_target_type TARGET ${_name} PROPERTY TYPE)
  if (NOT ${_target_type} STREQUAL "EXECUTABLE")
    if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
      set_property(TARGET ${_name} PROPERTY VERSION 1)
      set_property(TARGET ${_name} PROPERTY SOVERSION 1)
    endif()
    if("${_name}" MATCHES "^[Oo][Tt][Bb]")
      set(_otb "")
    else()
      set(_otb "otb")
    endif()
    set_property(TARGET ${_name} PROPERTY OUTPUT_NAME ${_otb}${_name}-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR})
  endif()
endmacro()

macro(otb_module_target_export _name)
  export(TARGETS ${_name} APPEND FILE ${${otb-module}-targets-build})
endmacro()

macro(otb_module_target_install _name)
  #Use specific runtime components for executables and libraries separately when installing a module,
  #considering that the target of a module could be either an executable or a library.
  get_property(_ttype TARGET ${_name} PROPERTY TYPE)
  if("${_ttype}" STREQUAL EXECUTABLE)
    set(runtime_component Runtime)
  else()
    set(runtime_component RuntimeLibraries)
  endif()
  install(TARGETS ${_name}
    EXPORT  ${${otb-module}-targets}
    RUNTIME DESTINATION ${${otb-module}_INSTALL_RUNTIME_DIR} COMPONENT ${runtime_component}
    LIBRARY DESTINATION ${${otb-module}_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${${otb-module}_INSTALL_ARCHIVE_DIR} COMPONENT Development
    )
endmacro()

macro(otb_module_target _name)
  set(_install 1)
  foreach(arg ${ARGN})
    if("${arg}" MATCHES "^(NO_INSTALL)$")
      set(_install 0)
    else()
      message(FATAL_ERROR "Unknown argument [${arg}]")
    endif()
  endforeach()
  otb_module_target_name(${_name})
  otb_module_target_label(${_name})
  otb_module_target_export(${_name})
  if(_install)
    otb_module_target_install(${_name})
  endif()
endmacro()
