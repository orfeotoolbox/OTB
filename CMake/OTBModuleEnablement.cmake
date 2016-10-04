# This script sorts out the module dependencies, provides user options for customizing
# the list of modules to be built, and enables modules accordingly.

# Load the module DAG.
set(OTB_MODULES_ALL)
file(GLOB meta RELATIVE "${OTB_SOURCE_DIR}"
   "${OTB_SOURCE_DIR}/*/*/*/otb-module.cmake" # grouped modules
  )
foreach(f ${meta})
  include(${OTB_SOURCE_DIR}/${f})
  list(APPEND OTB_MODULES_ALL ${otb-module})
  get_filename_component(${otb-module}_BASE ${f} PATH)
  set(${otb-module}_SOURCE_DIR ${OTB_SOURCE_DIR}/${${otb-module}_BASE})
  set(${otb-module}_BINARY_DIR ${OTB_BINARY_DIR}/${${otb-module}_BASE})
  if(BUILD_TESTING AND EXISTS ${${otb-module}_SOURCE_DIR}/test)
    list(APPEND OTB_MODULES_ALL ${otb-module-test})
    set(${otb-module-test}_SOURCE_DIR ${${otb-module}_SOURCE_DIR}/test)
    set(${otb-module-test}_BINARY_DIR ${${otb-module}_BINARY_DIR}/test)
    set(${otb-module-test}_IS_TEST 1)
    set(${otb-module}_TESTED_BY ${otb-module-test})
    set(${otb-module-test}_TESTS_FOR ${otb-module})
  endif()

  # Reject bad dependencies.
  string(REGEX MATCHALL ";(OTBDeprecated|OTBReview|OTBIntegratedTest);"
    _bad_deps ";${OTB_MODULE_${otb-module}_DEPENDS};${OTB_MODULE_${otb-module-test}_DEPENDS};")
  foreach(dep ${_bad_deps})
    if(NOT "${otb-module}" MATCHES "^(${dep}|OTBIntegratedTest)$")
      message(FATAL_ERROR
        "Module \"${otb-module}\" loaded from\n"
        "  ${${otb-module}_BASE}/otb-module.cmake\n"
        "may not depend on module \"${dep}\".")
    endif()
  endforeach()
endforeach()
# Clear variables set later in each module.
unset(otb-module)
unset(otb-module-test)

# Validate the module DAG.
macro(otb_module_check otb-module _needed_by stack)
  if(NOT OTB_MODULE_${otb-module}_DECLARED)
    message(FATAL_ERROR "No such module \"${otb-module}\" needed by \"${_needed_by}\"")
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

foreach(otb-module ${OTB_MODULES_ALL})
  otb_module_check("${otb-module}" "" "")
endforeach()

#----------------------------------------------------------------------
# Construct direct dependees (first-level) of each module
foreach(otb-module1 ${OTB_MODULES_ALL})
  foreach(otb-module2 ${OTB_MODULES_ALL})
    list(FIND OTB_MODULE_${otb-module2}_DEPENDS ${otb-module1} _find_output)
    if( NOT ${_find_output} EQUAL -1 )
      list(APPEND OTB_MODULE_${otb-module1}_DEPENDEES ${otb-module2})
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
option(OTB_BUILD_DEFAULT_MODULES "Build the default OTB modules." ON)

#----------------------------------------------------------------------
# Provide an option to build the tests of dependencies of a module when
# BUILD_TESTING is ON.
option(OTB_BUILD_ALL_MODULES_FOR_TESTS "Build the tests of module dependencies." OFF)
mark_as_advanced(OTB_BUILD_ALL_MODULES_FOR_TESTS)

# To maintain backward compatibility
if(DEFINED OTB_BUILD_ALL_MODULES)
  message(WARNING "OTB_BUILD_ALL_MODULES is deprecated, please remove this entry from the CMake "
                  "cache (edit the CMakeCache.txt file located in the top level of the OTB build "
                  "tree directly or via the CMake GUI), and use OTB_BUILD_DEFAULT_MODULES instead.")
  set(OTB_BUILD_DEFAULT_MODULES ${OTB_BUILD_ALL_MODULES} CACHE BOOL "Build the default OTB modules." FORCE)
endif()
# Provide module selections by groups
include(${OTB_SOURCE_DIR}/CMake/OTBGroups.cmake)

# Provide an option for each module.
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

# Follow dependencies.
macro(otb_module_enable otb-module _needed_by)
  if(NOT Module_${otb-module})
    if(NOT ${otb-module}_TESTED_BY OR
      NOT "x${_needed_by}" STREQUAL "x${${otb-module}_TESTED_BY}")
      list(APPEND OTB_MODULE_${otb-module}_NEEDED_BY ${_needed_by})
    endif()
  endif()
  if(NOT ${otb-module}_ENABLED)
    set(${otb-module}_ENABLED 1)
    foreach(dep IN LISTS OTB_MODULE_${otb-module}_DEPENDS)
      otb_module_enable(${dep} ${otb-module})
    endforeach()
    if(${otb-module}_TESTED_BY AND (OTB_BUILD_DEFAULT_MODULES OR OTB_BUILD_ALL_MODULES_FOR_TESTS OR Module_${otb-module}))
      otb_module_enable(${${otb-module}_TESTED_BY} "")
    endif()
  endif()
endmacro()

foreach(otb-module ${OTB_MODULES_ALL})
  if(Module_${otb-module} OR OTB_MODULE_${otb-module}_IN_DEFAULT)
    otb_module_enable("${otb-module}" "")
  elseif(OTB_MODULE_${otb-module}_REQUEST_BY)
    otb_module_enable("${otb-module}" "${OTB_MODULE_${otb-module}_REQUEST_BY}")
  endif()
endforeach()

# Filter out the ENABLED modules using the OTB_USE_XXX options
macro(otb_module_disable otb-module _disabled_by)
  if(NOT ${otb-module}_IS_TEST AND ${otb-module}_ENABLED)
    message(STATUS "Disabled ${otb-module} because ${_disabled_by} is OFF")
  endif()
  set(${otb-module}_ENABLED 0)
  foreach(otb-module-dependee ${OTB_MODULE_${otb-module}_DEPENDEES})
    otb_module_disable(${otb-module-dependee} ${_disabled_by})
  endforeach()
endmacro()

foreach(otb-module ${OTB_MODULES_ALL})
  if(OTB_MODULE_${otb-module}_ACTIVATION_OPTION
     AND NOT ${OTB_MODULE_${otb-module}_ACTIVATION_OPTION})
     otb_module_disable("${otb-module}" "${OTB_MODULE_${otb-module}_ACTIVATION_OPTION}")
  endif()
endforeach()

# Build final list of enabled modules.
set(OTB_MODULES_ENABLED "")
set(OTB_MODULES_DISABLED "")
foreach(otb-module ${OTB_MODULES_ALL})
  if(${otb-module}_ENABLED)
    list(APPEND OTB_MODULES_ENABLED ${otb-module})

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

# TODO : shall we set up CPack ?
# #
# # Set up CPack support
# #
# set(OTB_MODULES_DISABLED_CPACK )
# foreach(m ${OTB_MODULES_DISABLED})
#   list(APPEND OTB_MODULES_DISABLED_CPACK "/${m}/")
# endforeach()
# set(CPACK_SOURCE_IGNORE_FILES
#   "${OTB_MODULES_DISABLED_CPACK};/\\\\.git")
# 
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Insight Toolkit version ${OTB_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VENDOR "ISC")
# set(CPACK_PACKAGE_VERSION_MAJOR "${OTB_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${OTB_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${OTB_VERSION_PATCH}")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
# 
# include(CPack)

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

# Hide options for modules that will build anyway.
foreach(otb-module ${OTB_MODULES_ALL})
  if(NOT ${otb-module}_IS_TEST)
    if(OTB_MODULE_${otb-module}_IN_DEFAULT OR OTB_MODULE_${otb-module}_NEEDED_BY)
      set_property(CACHE Module_${otb-module} PROPERTY TYPE INTERNAL)
    else()
      set_property(CACHE Module_${otb-module} PROPERTY TYPE BOOL)
    endif()
  endif()
endforeach()

#-----------------------------------------------------------------------------
# Construct an in-build-order list of "CDash subproject" modules from the
# list of enabled modules:

set(OTB_CDASH_SUBPROJECT_MODULES)

if(OTB_GENERATE_PROJECT_XML OR OTB_GENERATE_SUBPROJECTS_CMAKE)
  # Since a "CDash subproject" for OTB logically contains both a non-test
  # module and its corresponding test module, the subproject order must take
  # both modules into account. The subprojects in this list are named after
  # the non-test modules in the enabled modules list. But the ordering of
  # these subprojects use the dependencies of the modules *and* their test
  # modules
  foreach(module ${OTB_MODULES_ENABLED})
    if(${module}_TESTED_BY)
      # module that HAS a test module: skip for now... will be added as a
      # subproject later, in the slot when its corresponding "-test" module
      # is encountered
    elseif(${module}_TESTS_FOR)
      # this is a test module... *now* list the module which it tests as
      # the subproject name:
      list(APPEND OTB_CDASH_SUBPROJECT_MODULES ${${module}_TESTS_FOR})
    else()
      # a module that is not a test module, and has no tested by...
      # is just a module-only subproject with no test module:
      list(APPEND OTB_CDASH_SUBPROJECT_MODULES ${module})
    endif()
  endforeach()
endif()

#-----------------------------------------------------------------------------
# Write a Project.xml file to send the description of the submodules and
# their dependencies up to CDash:

if(OTB_GENERATE_PROJECT_XML)
  set(filename "${OTB_BINARY_DIR}/${main_project_name}.Project.xml")

  set(xml "<?xml version='1.0' encoding='UTF-8'?>\n")
  set(xml "${xml}<Project name='${main_project_name}'>\n")
  foreach(module ${OTB_CDASH_SUBPROJECT_MODULES})
    if(${module}_IS_TEST)
      message(FATAL_ERROR "unexpected: subproject names should not be test modules module='${module}' tests_for='${${module}_TESTS_FOR}'")
    endif()
    set(xml "${xml}  <SubProject name='${module}'>\n")
    set(deps "")
    set(dep_list ${OTB_MODULE_${module}_DEPENDS})
    if(${module}_TESTED_BY)
      list(APPEND dep_list ${OTB_MODULE_${${module}_TESTED_BY}_DEPENDS})
      if(dep_list)
        list(SORT dep_list)
      endif()
    endif()
    foreach(dep ${dep_list})
      if(NOT ${dep}_IS_TEST AND NOT "${module}" STREQUAL "${dep}")
        set(xml "${xml}    <Dependency name='${dep}'/>\n")
      endif()
    endforeach()
    set(xml "${xml}  </SubProject>\n")
  endforeach()
  set(xml "${xml}</Project>\n")

  # Always write out "${filename}.in":
  file(WRITE ${filename}.in "${xml}")

  # Use configure_file so "${filename}" only changes when its content changes:
  configure_file(${filename}.in ${filename} COPYONLY)
endif()

#-----------------------------------------------------------------------------
# Write the list of enabled modules out for ctest scripts to use as an
# in-order subproject list:

if(OTB_GENERATE_SUBPROJECTS_CMAKE)
  set(filename "${OTB_BINARY_DIR}/${main_project_name}.SubProjects.cmake")

  set(s "# Generated by CMake, do not edit!\n")
  set(s "${s}set(otb_subprojects\n")
  foreach(otb-module ${OTB_CDASH_SUBPROJECT_MODULES})
    if(${otb-module}_IS_TEST)
      message(FATAL_ERROR "unexpected: subproject names should not be test modules otb-module='${otb-module}' tests_for='${${otb-module}_TESTS_FOR}'")
    endif()
    set(s "${s}  \"${otb-module}\"\n")
  endforeach()
  set(s "${s})\n")

  # Always write out "${filename}.in":
  file(WRITE ${filename}.in "${s}")

  # Use configure_file so "${filename}" only changes when its content changes:
 configure_file(${filename}.in ${filename} COPYONLY)
endif()

#-----------------------------------------------------------------------------

if(NOT OTB_MODULES_ENABLED)
  message(WARNING "No modules enabled!")
  file(REMOVE "${OTB_BINARY_DIR}/OTBTargets.cmake")
  return()
endif()

file(WRITE "${OTB_BINARY_DIR}/OTBTargets.cmake"
  "# Generated by CMake, do not edit!")

macro(init_module_vars)
  verify_otb_module_is_set()
  set(${otb-module}-targets OTBTargets)
  set(${otb-module}-targets-install "${OTB_INSTALL_PACKAGE_DIR}/OTBTargets.cmake")
  set(${otb-module}-targets-build "${OTB_BINARY_DIR}/OTBTargets.cmake")
endmacro()

# Build all modules.
foreach(otb-module ${OTB_MODULES_ENABLED})
  if(NOT ${otb-module}_IS_TEST)
    init_module_vars()
  endif()
  include("${${otb-module}_SOURCE_DIR}/otb-module-init.cmake" OPTIONAL)
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
