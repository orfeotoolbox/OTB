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
#-----------------------------------------------------------------------------
# This file is included by the generated OTBConfig.cmake
# which is read by every cmake file calling
# find_package(OTB...)
#-----------------------------------------------------------------------------
# Private helper macros.

# Append to current CMakelist and under the include and link directories
# properties with the include and lib dirs of $mod and its depends
macro(_otb_module_use_recurse mod)
  if(NOT ${dep}_USED)
    set(${mod}_USED 1)
    otb_module_load("${mod}")
    foreach(dep IN LISTS ${mod}_DEPENDS)
      _otb_module_use_recurse(${dep})
    endforeach()
    if(${mod}_INCLUDE_DIRS)
      include_directories(${${mod}_INCLUDE_DIRS})
    endif()
    if(${mod}_LIBRARY_DIRS)
      link_directories(${${mod}_LIBRARY_DIRS})
    endif()
  endif()
endmacro()

# append to ${ns}_LIBRAIRIES/INCLUDE_DIRS/LIBRARY_DIRS
# the ${mod}_LIBRAIRIES/INCLUDE_DIRS/LIBRARY_DIRS and thoses of its dependencies
macro(_otb_module_config_recurse ns mod)
  if(NOT _${ns}_${mod}_USED)
    set(_${ns}_${mod}_USED 1)
    list(APPEND _${ns}_USED_MODULES ${mod})
    otb_module_load("${mod}")
    list(APPEND ${ns}_LIBRARIES ${${mod}_LIBRARIES})
    list(APPEND ${ns}_INCLUDE_DIRS ${${mod}_INCLUDE_DIRS})
    list(APPEND ${ns}_LIBRARY_DIRS ${${mod}_LIBRARY_DIRS})
    foreach(dep IN LISTS ${mod}_DEPENDS)
      _otb_module_config_recurse("${ns}" "${dep}")
    endforeach()
  endif()
endmacro()

#-----------------------------------------------------------------------------
# Public interface macros.

# otb_module_load(<module>)
#
# Loads variables describing the given module:
#  <module>_LOADED           = True if the module has been loaded
#  <module>_DEPENDS          = List of dependencies on other modules
#  <module>_LIBRARIES        = Libraries to link
#  <module>_INCLUDE_DIRS     = Header search path
#  <module>_LIBRARY_DIRS     = Library search path (for outside dependencies)
macro(otb_module_load mod)
  if(NOT ${mod}_LOADED)
    # since OTB 9.1, module can be separated in different folders, thus a list
    # OTB_MODULES_DIRS is constructed in the different Config.cmake
    # files of modules
    foreach(__mod_dir IN LISTS OTB_MODULES_DIRS)
      if (EXISTS "${__mod_dir}/${mod}.cmake")
        include("${__mod_dir}/${mod}.cmake" OPTIONAL)
      endif()
    endforeach()

    if(NOT ${mod}_LOADED)
      message(FATAL_ERROR "No such module: \"${mod}\" . Check if file ${mod}.cmake exists in \"${OTB_MODULES_DIRS}\" folders")
    endif()
  endif()
endmacro()

# Create two <module>.cmake files for <module_name>. This file contains
# include path, lib path, library list and cmake dependencies properties of
# <module_name>. These properties can be loaded by otb_module_use during current
# build and by another module depending of <module_name>. As some properties
# change if <module_name> is currently build or already installed, TWO file are
# generated.
# - <module_name> is MANDATORY!
# - DEPENDS <list> are the cmake target dependencies
# - OPTIONAL_DEPENDS <list> a list added to cmake dependencies only if their are previously enabled
# - LIBRARIES <list> is the library list
# - LIBRARY_DIRS <list> is a list of path
# - SYSTEM_LIBRARY_DIRS <list> same as library dirs but for system libs
# - INCLUDE_DIRS_BUILD <list> is a list of include paths of <module_name> DURING ITS BUILD
# - INCLUDE_DIRS_INSTALL <list> is a list of include path of <module_name> where there will be installed
# - SYSTEM_INCLUDE_DIRS <list> a list of system include path
# - EXPORT_CODE_BUILD and EXPORT_CODE install 
function(generate_cmake_module_configs
         module_name otb_dir)
  # ---------- Argument definition and reading
  set(oneValueArgs COMPONENT
                   EXPORT_CODE_BUILD
                   EXPORT_CODE_INSTALL)
  set(multiValuesArgs DEPENDS
                      OPTIONAL_DEPENDS
                      LIBRARIES
                      LIBRARY_DIRS
                      SYSTEM_LIBRARY_DIRS
                      INCLUDE_DIRS_BUILD
                      INCLUDE_DIRS_INSTALL
                      SYSTEM_INCLUDE_DIRS)

  # parse arguments and put their values in var arg_<argument_name>
  cmake_parse_arguments(PARSE_ARGV 2 arg "" "${oneValueArgs}" "${multiValuesArgs}")

  if (arg_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "generate_cmake_module_configs: Unknown args: ${arg_UNPARSED_ARGUMENTS}")
  endif()
  

  # ----------- END argument part

  set(otb-module-DEPENDS "")
  set(otb-module-LIBRARIES "")
  set(otb-module-LIBRARY_DIRS "")
  set(otb-module-INCLUDE_DIRS-install "")
  set(otb-module-INCLUDE_DIRS-build "")
  set(otb-module-INCLUDE_DIRS "")
  set(otb-module-EXPORT_CODE "")

  # --------------- COMMON VARS FOR BOTH BUILD AND INSTALL FILES ------------
  # DEPENDENCIES, normal and optionnal
  if (arg_DEPENDS)
    set(otb-module-DEPENDS "${arg_DEPENDS}")
  endif()
  if (arg_OPTIONAL_DEPENDS)
    foreach(dep IN LISTS arg_OPTIONAL_DEPENDS)
      if (${dep}_ENABLED)
        list(APPEND otb-module-DEPENDS ${dep})
      endif()
    endforeach()
  endif()

  # Libraries to link to
  if (arg_LIBRARIES)
    set(otb-module-LIBRARIES "${arg_LIBRARIES}")
  endif()

  # Add group and system library dir to module configuration
  if (arg_LIBRARY_DIRS)
    if (arg_COMPONENT)
      set(otb-module-LIBRARY_DIRS "${arg_LIBRARY_DIRS}")
    else()
      message(STATUS "COMPONENT of ${module_name} does not exists when setting otb-module-LIBRARY_DIRS")
    endif()
  endif()
  if (arg_SYSTEM_LIBRARY_DIRS)
    list(APPEND otb-module-LIBRARY_DIRS "${arg_SYSTEM_LIBRARY_DIRS}")
  endif()

  # INCLUDES DIRS: the OTB sources include path and system include path
  # The include location for build cmake file is not the same as the install one
  # The include path used during build is the same as in the source tree
  # the one for the install is located in include/OTB-<Major>.<Minor>

  # define OTB include first to use them prior to system ones
  if (arg_INCLUDE_DIRS_INSTALL)
    list(APPEND otb-module-INCLUDE_DIRS-install "${arg_INCLUDE_DIRS_INSTALL}")
  endif()
  if (arg_INCLUDE_DIRS_BUILD)
    list(APPEND otb-module-INCLUDE_DIRS-build "${arg_INCLUDE_DIRS_BUILD}")
  endif()

  # system includes does not differs for build and install
  if(arg_SYSTEM_INCLUDE_DIRS)
    list(APPEND otb-module-INCLUDE_DIRS-build "${arg_SYSTEM_INCLUDE_DIRS}")
    list(APPEND otb-module-INCLUDE_DIRS-install "${arg_SYSTEM_INCLUDE_DIRS}")
  endif()

  # CMake file used during CURRENT BUILD by otb_module_use
  if (arg_EXPORT_CODE_BUILD)
    set(otb-module-EXPORT_CODE "${arg_EXPORT_CODE_BUILD}")
  endif()
  set(otb-module-INCLUDE_DIRS "${otb-module-INCLUDE_DIRS-build}")
  configure_file("${otb_dir}/OTBModuleInfo.cmake.in" "${CMAKE_BINARY_DIR}/${OTB_INSTALL_PACKAGE_DIR}/Modules/${module_name}.cmake" @ONLY)

  # CMake file used by dependencies and INSTALLED with OTB or module
  if (arg_EXPORT_CODE_INSTALL)
    set(otb-module-EXPORT_CODE "${arg_EXPORT_CODE_INSTALL}")
  endif()
  set(otb-module-INCLUDE_DIRS "${otb-module-INCLUDE_DIRS-install}")
  configure_file("${otb_dir}/OTBModuleInfo.cmake.in" "${CMAKE_BINARY_DIR}/CMakeFiles/${module_name}.cmake" @ONLY)
  # create a file with variables previously fields
  # file will be installed in lib/cmake/OTB-X.X/Module
  # For P0 module the path is the same as OTB install path
  if (arg_COMPONENT)
    install(FILES ${CMAKE_BINARY_DIR}/CMakeFiles/${module_name}.cmake
            DESTINATION "${OTB_INSTALL_PACKAGE_DIR}/Modules"
            COMPONENT ${arg_COMPONENT}
    )
  else()
    install(FILES ${CMAKE_BINARY_DIR}/CMakeFiles/${module_name}.cmake
      DESTINATION "${OTB_INSTALL_PACKAGE_DIR}/Modules"
    )
  endif()
endfunction()

# otb_module_config(<namespace> [modules...])
#
# Configures variables describing the given modules and their dependencies:
#  <namespace>_LIBRARIES    = Libraries to link
#  <namespace>_INCLUDE_DIRS = Header search path
#  <namespace>_LIBRARY_DIRS = Library search path (for outside dependencies)
# Do not name a module as the namespace.
macro(otb_module_config ns)
  # if there is mod in ${ARGN}, these three vars are fields with all mod lib,
  # include ...
  set(${ns}_LIBRARIES "")
  set(${ns}_INCLUDE_DIRS "")
  set(${ns}_LIBRARY_DIRS "")

  set(_${ns}_USED_MODULES "")
  foreach(mod ${ARGN})
    _otb_module_config_recurse("${ns}" "${mod}")
  endforeach()
  foreach(mod ${_${ns}_USED_MODULES})
    unset(_${ns}_${mod}_USED)
  endforeach()
  unset(_${ns}_USED_MODULES)

  foreach(v ${ns}_LIBRARIES ${ns}_INCLUDE_DIRS ${ns}_LIBRARY_DIRS)
    if(${v})
      list(REMOVE_DUPLICATES ${v})
    endif()
  endforeach()
endmacro()

# otb_module_use([modules...])
#
# Adds include directories and link directories for the given modules and
# their dependencies.
macro(otb_module_use)
  foreach(mod ${ARGN})
    _otb_module_use_recurse("${mod}")
  endforeach()
endmacro()

macro(otb_module_check_name _name)
  if( NOT "${_name}" MATCHES "^[a-zA-Z][a-zA-Z0-9]*$")
    message(FATAL_ERROR "Invalid module name: ${_name}, only alphanumerical characters are allowed (no space or special character)")
  endif()
  # ensure that there is no difference between PROJECT_NAME declare
  # by project() command in all CMakeLists modules
  # and otb-module name as some cmake OTB code use the variables
  # generated by project command with the otb-module name instead
  # of the project_name
  # Also avoid comparing it with OTB as this function can be used by
  # OTBModuleEnablement when the CMakelists are not already read
  if ( NOT ("${PROJECT_NAME}" STREQUAL "OTB") AND
       NOT ("${_name}" STREQUAL "${PROJECT_NAME}"))
    message(FATAL_ERROR "Name of ${_name} module must be identical to \"${PROJECT_NAME}\" declared by \"project()\" command")
  endif()
endmacro()

macro(otb_module_warnings_disable)
  foreach(lang ${ARGN})
    if(MSVC)
      string(REGEX REPLACE "(^| )[/-]W[0-4]( |$)" " "
        CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w")
    elseif(BORLAND)
      set(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w-")
    else()
      # disable all GCC warnings https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Options-to-Request-or-Suppress-Warnings
      set(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} -w")
    endif()
  endforeach()
endmacro()

# Add a cmake option "OTB_USE_<otb-module>" to enable or disable
# specific module compilation. Often used for otb thirdparties that can be
# optional
# Usage: otb_module_activation_option("Option description" ON/OFF)
macro(otb_module_activation_option _option_desc _default)
  # Remove OTB prefix from ${otb-module} and create a string
  # OTB_USE_${otb-module}
  string(REGEX REPLACE "OTB(.*)" "OTB_USE_\\1" _option_name ${otb-module})
  string(TOUPPER ${_option_name} _option_name)
  option(${_option_name} ${_option_desc} ${_default})
  set(OTB_MODULE_${otb-module}_ACTIVATION_OPTION ${_option_name})
  # this list is init in OTBConstants.cmake
  list(APPEND OTB_MODULE_ACTIVATION_OPTION_LIST ${_option_name})
endmacro()
