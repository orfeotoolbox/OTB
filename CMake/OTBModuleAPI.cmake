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

#-----------------------------------------------------------------------------
# Private helper macros.

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
    include("${OTB_MODULES_DIR}/${mod}.cmake" OPTIONAL)
    if(NOT ${mod}_LOADED)
      message(FATAL_ERROR "No such module: \"${mod}\"")
    endif()
  endif()
endmacro()

# otb_module_config(<namespace> [modules...])
#
# Configures variables describing the given modules and their dependencies:
#  <namespace>_LIBRARIES    = Libraries to link
#  <namespace>_INCLUDE_DIRS = Header search path
#  <namespace>_LIBRARY_DIRS = Library search path (for outside dependencies)
# Do not name a module as the namespace.
macro(otb_module_config ns)
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
