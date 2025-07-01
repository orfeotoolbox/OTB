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

#[[
Check that the otb-module named "otb-module" is correctly init
(otb-module.cmake file read) as this module is a dependency of "_needed_by"
module
]]
macro(otb_module_check otb-module _needed_by stack)
  # module is declared if its otb-module file is read (see otb_module macro in OTBStandaloneModuleMacros)
  if(NOT OTB_MODULE_${otb-module}_DECLARED)
    get_property(_enabled_groups GLOBAL PROPERTY OTB_GROUPS_ENABLED)
    message(FATAL_ERROR "No such module \"${otb-module}\" needed by \"${_needed_by}\". Maybe there is a missing -DOTBGroup_<xx> option.\nCurrently enabled groups: ${_enabled_groups}")
  endif()
  if(check_started_${otb-module} AND NOT check_finished_${otb-module})
    # We reached a module while traversing its own dependencies recursively.
    set(msg "")
    foreach(entry ${stack})
      set(msg " ${entry} =>${msg}")
      if("${entry}" STREQUAL "${otb-module}")
        break()
      endif()
    endforeach()
    message(FATAL_ERROR "Module dependency cycle detected:\n ${msg} ${otb-module}")
  elseif(NOT check_started_${otb-module})
    # Traverse dependencies of this module.  Mark the start and finish.
    set(check_started_${otb-module} 1)
    foreach(dep IN LISTS OTB_MODULE_${otb-module}_DEPENDS)
      otb_module_check(${dep} ${otb-module} "${otb-module};${stack}")
    endforeach()
    foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
      otb_module_check(${dep} ${otb-module} "${otb-module};${stack}")
    endforeach()
    set(check_finished_${otb-module} 1)
  endif()
endmacro()


# Follow dependencies.
# Marks "otb-module" and its dependencies "_needed_by" ENABLED with the variable
# ${otb-module}_ENABLED. All of its dependencies in "_needed_by" list will
# also be marked enabled with the same var.
macro(otb_module_enable otb-module _needed_by)
  if(NOT Module_${otb-module})
    if(NOT ${otb-module}_TESTED_BY OR
      NOT "x${_needed_by}" STREQUAL "x${${otb-module}_TESTED_BY}")
      list(APPEND OTB_MODULE_${otb-module}_NEEDED_BY ${_needed_by})
    endif()
  endif()
  if(NOT ${otb-module}_ENABLED)
    set(${otb-module}_ENABLED 1)
    # if this module has an ACTIVATION_OPTION=OFF, don't bother enable its dependencies
    if(NOT (OTB_MODULE_${otb-module}_ACTIVATION_OPTION
     AND NOT ${OTB_MODULE_${otb-module}_ACTIVATION_OPTION}))
      foreach(dep IN LISTS OTB_MODULE_${otb-module}_DEPENDS)
        otb_module_enable(${dep} ${otb-module})
      endforeach()
      foreach(dep IN LISTS OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS)
        otb_module_enable(${dep} ${otb-module})
      endforeach()
      if(${otb-module}_TESTED_BY AND (BUILD_TESTING OR Module_${otb-module}))
        otb_module_enable(${${otb-module}_TESTED_BY} "")
      endif()
    endif()
  endif()
endmacro()

# Filter out the ENABLED modules using the OTB_USE_XXX options
macro(otb_module_disable otb-module _disabled_by)
  if(NOT ${otb-module}_IS_TEST AND ${otb-module}_ENABLED)
    message(STATUS "Disabled ${otb-module} because ${_disabled_by} is OFF")
  endif()
  set(${otb-module}_ENABLED 0)
  # disable project depending of otb-module
  foreach(otb-module-dependee ${OTB_MODULE_${otb-module}_DEPENDEES})
    otb_module_disable(${otb-module-dependee} ${_disabled_by})
  endforeach()
endmacro()

macro(init_module_vars)
  if( "${otb-module}" STREQUAL "" )
    message(FATAL_ERROR "CMake variable otb-module is not set")
  endif()
  # define the target name relative to the component if it exists
  set(${otb-module}-targets ${OTB_MODULE_${otb-module}_COMPONENT}Targets)
  set(${otb-module}-targets-install "${OTB_INSTALL_PACKAGE_DIR}/${${otb-module}-targets}.cmake")
  set(${otb-module}-targets-build "${OTB_BINARY_DIR}/${${otb-module}-targets}.cmake")
  if (CMAKE_DEBUG)
    message(STATUS "[CMAKE_DEBUG] OTB_MODULE_${otb-module}_COMPONENT of ${otb-module} ==  ${OTB_MODULE_${otb-module}_COMPONENT}")
    message(STATUS "[CMAKE_DEBUG] \${otb-module}-targets of ${otb-module} == ${${otb-module}-targets}")
    message(STATUS "[CMAKE_DEBUG] \${otb-module}-targets-install of ${otb-module} == ${${otb-module}-targets-install}")
    message(STATUS "[CMAKE_DEBUG] \${otb-module}-targets-build of ${otb-module} == ${${otb-module}-targets-build}")
  endif()
endmacro()

# This script sorts out the module dependencies, provides user options for customizing
# the list of modules to be built, and enables modules accordingly.

include(${OTB_SOURCE_DIR}/CMake/OTBGroups.cmake)

# Create the group list and set default options
init_OTBGroups_config()
get_enabled_groups_list()
auto_enable_thirdparties()

get_property(enabled_groups_list GLOBAL PROPERTY OTB_GROUPS_ENABLED)

# This var will contains modules of all enabled groups and test modules
# associated if test are enabled
set(OTB_MODULES_ALL)

# Read only modules of enabled groups. Each module conf is in otb-module.cmake
# file
# After this loop the OTB_MODULES_ALL contains a module list of wanted groups
foreach( _group ${enabled_groups_list} )
  # match all otb-module.cmake files in ${OTB_SOURCE_DIR}/Modules/${group}/*
  # and its subdirs.
  # Note the '*' to skip the otb-module.cmake at the root of each group.
  # If we read the otb-module.cmake (for instance when there are remote module)
  # the group itself will be in OTB_MODULE_ENABLED list later but with
  # wrong dependency order.
  file( GLOB_RECURSE _${_group}_module_files RELATIVE "${OTB_SOURCE_DIR}" ${OTB_SOURCE_DIR}/Modules/${_group}/*/otb-module.cmake )
  foreach( _module_file ${_${_group}_module_files} )
    # read otb-module.cmake file, doing this init module variables
    include(${OTB_SOURCE_DIR}/${_module_file})
    list(APPEND OTB_MODULES_ALL ${otb-module})
    get_filename_component(${otb-module}_BASE ${_module_file} DIRECTORY)
    set(${otb-module}_SOURCE_DIR ${OTB_SOURCE_DIR}/${${otb-module}_BASE})
    set(${otb-module}_BINARY_DIR ${OTB_BINARY_DIR}/${${otb-module}_BASE})

    if( BUILD_TESTING AND EXISTS ${${otb-module}_SOURCE_DIR}/test )
      list(APPEND OTB_MODULES_ALL ${otb-module-test})
      set(${otb-module-test}_SOURCE_DIR ${${otb-module}_SOURCE_DIR}/test)
      set(${otb-module-test}_BINARY_DIR ${${otb-module}_BINARY_DIR}/test)
      set(${otb-module-test}_IS_TEST 1)
      set(${otb-module}_TESTED_BY ${otb-module-test})
      set(${otb-module-test}_TESTS_FOR ${otb-module})
    endif()

    # Exclude remote modules from default modules so that they are
    # enabled/disabled using their variable Module_XXXX
    if("${_group}" STREQUAL "Remote")
      set(OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT 1)
    endif()

    if( NOT OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT )
      list(APPEND OTB_MODULE_${otb-module}_REQUEST_BY OTBGroup_${_group})
    endif()

    # NOTE TLA: is these check always needed ? Seems to be the only 
    # OTBDeprecated|OTBReview|OTBIntegratedTest across all the project

    # Reject bad dependencies of module or of its tests
    string(REGEX MATCHALL ";(OTBDeprecated|OTBReview|OTBIntegratedTest);"
      _bad_deps ";${OTB_MODULE_${otb-module}_DEPENDS};${OTB_MODULE_${otb-module-test}_DEPENDS};")
    # check if the bad dependencies are just the module itself or
    # OTBIntegratedTest
    foreach(dep ${_bad_deps})
      if(NOT "${otb-module}" MATCHES "^(${dep}|OTBIntegratedTest)$")
        message(FATAL_ERROR
          "Module \"${otb-module}\" loaded from\n"
          "  ${${otb-module}_BASE}/otb-module.cmake\n"
          "may not depend on module \"${dep}\".")
      endif()
    endforeach()
  endforeach()
endforeach()

# Check that all dependencies of each module exists
foreach(otb-module ${OTB_MODULES_ALL})
  otb_module_check("${otb-module}" "" "")
endforeach()

#----------------------------------------------------------------------
# Construct direct dependees (first-level) of each module
foreach(potential_dependency ${OTB_MODULES_ALL})
  foreach(otb-module ${OTB_MODULES_ALL})
    list(FIND OTB_MODULE_${otb-module}_DEPENDS ${potential_dependency} _find_output)
    if( NOT ${_find_output} EQUAL -1 )
      list(APPEND OTB_MODULE_${potential_dependency}_DEPENDEES ${otb-module})
    endif()
  endforeach()
endforeach()

#----------------------------------------------------------------------
# Provide an option to build the default set of OTB modules. Only a small
# set of modules are excluded and they have the "EXCLUDE_FROM_DEFAULT" tags in
# their module definition file (otb-module.cmake).
#
# However, if you choose to customize which modules will be built, OTB also
# allows you to manually enable modules by using either individual Module_*
# options or OTBGroup_* options.
option(OTB_BUILD_DEFAULT_MODULES "Build the default OTB modules." OFF)

#----------------------------------------------------------------------
# Provide an cmake option for each module.
# This option is hide later if module is compiled anyway (in group, in default
# or is a dependency of another module)
foreach(otb-module ${OTB_MODULES_ALL})
  if(NOT ${otb-module}_IS_TEST)
    option(Module_${otb-module} "Request building ${otb-module}" OFF)
    mark_as_advanced(Module_${otb-module})
    if(OTB_MODULE_${otb-module}_EXCLUDE_FROM_DEFAULT)
      set(OTB_MODULE_${otb-module}_IN_DEFAULT 0)
    else()
      set(OTB_MODULE_${otb-module}_IN_DEFAULT ${OTB_BUILD_DEFAULT_MODULES})
    endif()
  endif()
endforeach()

# Mark modules enabled if:
# - Module_<module-name> option is ON (like in remotes modules)
# - OTB_BUILD_DEFAULT_MODULES is ON and module not excluded from default
# - the module is requested by a specific group
foreach(otb-module ${OTB_MODULES_ALL})
  if(Module_${otb-module})
    set_property(CACHE Module_${otb-module} PROPERTY TYPE BOOL)
    otb_module_enable("${otb-module}" "")
  elseif(OTB_MODULE_${otb-module}_IN_DEFAULT)
    # Hide options for modules that will build anyway.
    set_property(CACHE Module_${otb-module} PROPERTY TYPE INTERNAL)
    otb_module_enable("${otb-module}" "")
  elseif(OTB_MODULE_${otb-module}_REQUEST_BY)
    # Hide options for modules that will build anyway.
    set_property(CACHE Module_${otb-module} PROPERTY TYPE INTERNAL)
    otb_module_enable("${otb-module}" "${OTB_MODULE_${otb-module}_REQUEST_BY}")
  endif()
endforeach()

# Disable module having activation option to OFF
# the ACTIVATION_OPTION var is managed by otb_module_activation_option
# in OTBModuleMacros.cmake
foreach(otb-module ${OTB_MODULES_ALL})
  if(OTB_MODULE_${otb-module}_ACTIVATION_OPTION
     AND NOT ${OTB_MODULE_${otb-module}_ACTIVATION_OPTION})

     otb_module_disable("${otb-module}" "${OTB_MODULE_${otb-module}_ACTIVATION_OPTION}")
  endif()
endforeach()

# Build final list of enabled modules.
set(OTB_MODULES_ENABLED "")
set(OTB_MODULES_DISABLED "")
# for all enabled modules, add them in
# component_module list
foreach(otb-module ${OTB_MODULES_ALL})
  if(${otb-module}_ENABLED)
    list(APPEND OTB_MODULES_ENABLED ${otb-module})
    # add the module in the list of enabled module per component. This will be used later in <Component>Config.cmake files
    # Do not add test modules because they are not packaged
    if(NOT ${otb-module}_IS_TEST)
      list(APPEND OTB_${OTB_MODULE_${otb-module}_COMPONENT}_MODULES_ENABLED ${otb-module})
    endif()
    # We will sort modules according to their dependency graph,
    # to enable them in order. To solve the modules dependency graph,
    # we join both DEPENDS and OPTIONAL_DEPENDS (if enabled)
    set(OTB_MODULE_${otb-module}_DEPENDS_FOR_SORT "")
    list(APPEND OTB_MODULE_${otb-module}_DEPENDS_FOR_SORT ${OTB_MODULE_${otb-module}_DEPENDS})
    foreach(opt-dep ${OTB_MODULE_${otb-module}_OPTIONAL_DEPENDS})
      if(${opt-dep}_ENABLED)
        list(APPEND OTB_MODULE_${otb-module}_DEPENDS_FOR_SORT ${opt-dep})
      endif()
    endforeach()
  else()
    list(APPEND OTB_MODULES_DISABLED ${otb-module})
  endif()
endforeach()
list(SORT OTB_MODULES_ENABLED) # Deterministic order.
list(SORT OTB_MODULES_DISABLED) # Deterministic order.

# Order list to satisfy dependencies.
include(CMake/TopologicalSort.cmake)
topological_sort(OTB_MODULES_ENABLED OTB_MODULE_ _DEPENDS_FOR_SORT)

# Report what will be built.
set(_enabled_modules "${OTB_MODULES_ENABLED}")
list(SORT _enabled_modules)
foreach(otb-module ${_enabled_modules})
  if(NOT ${otb-module}_IS_TEST)
    if(Module_${otb-module})
      set(_reason ", requested by Module_${otb-module}")
    elseif(OTB_MODULE_${otb-module}_IN_DEFAULT)
      set(_reason ", requested by OTB_BUILD_DEFAULT_MODULES")
    else()
      set(_reason ", needed by [${OTB_MODULE_${otb-module}_NEEDED_BY}]")
    endif()
    message(STATUS "Enabled ${otb-module}${_reason}.")
  endif()
endforeach()

if(NOT OTB_MODULES_ENABLED)
  message(WARNING "No modules enabled!")
  file(REMOVE "${OTB_BINARY_DIR}/CoreTargets.cmake")
  return()
endif()

# Build all modules.
foreach(otb-module ${OTB_MODULES_ENABLED})
  # do not declare specific module targets variables for test
  if(NOT ${otb-module}_IS_TEST)
    init_module_vars()
  endif()
  # if file does not exists, this wont fail
  include("${${otb-module}_SOURCE_DIR}/otb-module-init.cmake" OPTIONAL)
  # include and execute the CMakeLists.txt present in module directory and put outputs in
  # "${${otb-module}_BINARY_DIR}"
  add_subdirectory("${${otb-module}_SOURCE_DIR}" "${${otb-module}_BINARY_DIR}")
endforeach()

#----------------------------------------------------------------------------
get_property(CTEST_CUSTOM_MEMCHECK_IGNORE GLOBAL PROPERTY CTEST_CUSTOM_MEMCHECK_IGNORE)
get_property(CTEST_CUSTOM_TESTS_IGNORE GLOBAL PROPERTY CTEST_CUSTOM_TESTS_IGNORE)
configure_file(CMake/CTestCustom.cmake.in CTestCustom.cmake @ONLY)

#-----------------------------------------------------------------------------

# Create list of available modules and libraries.
set(OTB_CONFIG_MODULES_ENABLED "")
foreach(otb-module ${OTB_MODULES_ENABLED})
  if(NOT ${otb-module}_IS_TEST)
    list(APPEND OTB_CONFIG_MODULES_ENABLED ${otb-module})
  endif()
endforeach()

# if all appends correctly, generate doc for all modules
setup_group_desc_doc()
