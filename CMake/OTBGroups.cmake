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

# Set a list of group names
set(group_list
  Core
  FeaturesExtraction
  Hyperspectral
  Learning
  Miscellaneous
  Remote
  SAR
  Segmentation
  StereoProcessing
  ThirdParty
  )
set_property(GLOBAL PROPERTY OTB_GROUPS_LIST ${group_list})

set(Core_documentation "This group contains the core module used in Orfeo ToolBox")
set(FeaturesExtraction_documentation "This group contains algorithms dedicated to hyperspectral remote sensing")
set(Hyperspectral_documentation "This group contains algorithms dedicated to hyperspectral remote sensing")
set(IO_documentation "This group contains everything related to input/output")
set(Learning_documentation "This group contains algorithms and frameworks related to supervised or unsupervised learning")
set(Miscellaneous_documentation "This group contains miscellaneous algorithms")
set(Radiometry_documentation "This group contains algorithms related to the processing of image radiometry")
set(StereoProcessing_documentation "This group contains algorithms related to registration of images")
set(SAR_documentation "This group contains algorithms related to SAR processing")
set(Segmentation_documentation "This group contains algorithms related to image segmentaiton")
set(ThirdParty_documentation "This group contains all Orfeo ToolBox third parties")

set(Remote_documentation "This group of modules is for OTB based code that have
additional third-party dependencies not bundled with the toolkit,
new algorithms or implementations seeking greater exposure and adoption,
algorithms that hope to eventually be integrated into the toolkit,
niche algorithms with limited application, and Modules in progress that do not
yet have the test coverage and cross-platform standards required by the main toolkit.
The modules are OFF by default in ITK's CMake configuration.
Note that these modules do get the same level of support and backwards
compatibility as other modules in the toolkit.")

#------------------------------------------------
# Find the modules in each group and the module name line in otb-module.cmake
# OUTPUT for each group:
# ${group}_module_list that contains names of each "otb_module" defined in
# otb-module.cmake file found recursively
foreach( group ${group_list} )
  if (CMAKE_DEBUG)
    message(STATUS "[CMAKE_DEBUG] ${group}Targets_EXPORTED == ${${group}Targets_EXPORTED} before turning it to 0")
  endif()
  set(${group}Targets_EXPORTED 0 CACHE INTERNAL "Bool to not declare multiple times ${group}Targets.cmake file" FORCE)

  set( _${group}_module_list )
  file( GLOB_RECURSE _${group}_module_files ${OTB_SOURCE_DIR}/Modules/${group}/otb-module.cmake )
  foreach( _module_file ${_${group}_module_files} )
    file( STRINGS ${_module_file} _module_line REGEX "otb_module[ \n]*\\([ \n]*[A-Za-z0-9]*" )
    string( REGEX MATCH "(\\([ \n]*)([A-Za-z0-9]*)" _module_name ${_module_line} )
    set( _module_name ${CMAKE_MATCH_2} )
    set( _${_module_name}_module_line ${_module_line} )
    list( APPEND _${group}_module_list ${_module_name} )
  endforeach()
endforeach()

#------------------------------------------------
# Set up Doxygen Group descriptions

set( group_list_dox )
foreach(group ${group_list} )
  set( group_list_dox
"${group_list_dox}
// -----------------------------------------------
// Group ${group}
/** \\defgroup Group-${group} Group ${group}
${${group}_documentation} */\n"
    )

  foreach(mod ${_${group}_module_list} )
    set( group_list_dox
"${group_list_dox}
/** \\defgroup ${mod} Module ${mod}
\\ingroup Group-${group} */\n"
      )
  endforeach() # mod ${_${group}_module_list}
endforeach() # group ${group_list} 

set( _content ${group_list_dox} )
configure_file(
  "${OTB_SOURCE_DIR}/Utilities/Doxygen/Module.dox.in"
  "${OTB_BINARY_DIR}/Utilities/Doxygen/Modules/OTB-AllGroups.dox"
  )

#------------------------------------------------
# Turn on the OTB_BUILD option for each group

# Set a module name list for each group and exclude
# Modules that should be OFF
# OUTPUT: _${group}_on_module_list that contains list of enabled modules
# if OTB_MODULE_${module}_EXCLUDE_FROM_DEFAULT is undeclared or OFF
foreach( group ${group_list} )
  set( _${group}_on_module_list )
  list( LENGTH _${group}_module_list _num_modules )
  set( _current_module 0 )
  foreach(module ${_${group}_module_list})
    if( NOT OTB_MODULE_${module}_EXCLUDE_FROM_DEFAULT )
      list( APPEND _${group}_on_module_list ${module} )
    endif()
  endforeach()
endforeach()

#by default enable Core and Thirdparty modules
option(OTBGroup_Core  "Request building Core modules" ON)
option(OTBGroup_ThirdParty "Request using thirdparty modules" ON)

# create a list of enable groups to later generate needed Config.cmake files
get_property(GROUPS_ENABLED_LIST GLOBAL PROPERTY OTB_GROUPS_ENABLED)

# Foreach group:
# - Depending of option OTB_BUILD_${group}, defines OTBGroup_${group}
# - If the group is required to be build, appends to
#   OTB_MODULE_${otb-module}_REQUEST_BY (with otb-module as group submodule)
#   the value 
foreach( group ${group_list})
    if(NOT DEFINED OTBGroup_${group})
      if(DEFINED OTB_BUILD_${group})
        option(OTBGroup_${group} "Request building ${group} modules" ${OTB_BUILD_${group}})
      else()
        option(OTBGroup_${group} "Request building ${group} modules" OFF)
      endif()
    endif()
    if (OTBGroup_${group})
      list(APPEND GROUPS_ENABLED_LIST ${group})
      foreach (otb-module ${_${group}_on_module_list} )
         list(APPEND OTB_MODULE_${otb-module}_REQUEST_BY OTBGroup_${group})
      endforeach()
    endif()
    # Hide group options if building all modules anyway.
    if(OTB_BUILD_DEFAULT_MODULES)
      set_property(CACHE OTBGroup_${group} PROPERTY TYPE INTERNAL)
    else()
      set_property(CACHE OTBGroup_${group} PROPERTY TYPE BOOL)
    endif()
endforeach()
list(REMOVE_DUPLICATES GROUPS_ENABLED_LIST)
set_property(GLOBAL PROPERTY OTB_GROUPS_ENABLED ${GROUPS_ENABLED_LIST})

if(OTBGroup_Learning MATCHES ON)
  set(OTB_USE_LIBSVM ON CACHE BOOL "Enable module LibSVM in OTB" FORCE)
  set(OTB_USE_OPENCV ON CACHE BOOL "Enable module OPENCV in OTB" FORCE)
  set(OTB_USE_SHARK ON CACHE BOOL "Enable module Shark in OTB" FORCE)
# else()
#   set(OTB_USE_LIBSVM OFF CACHE BOOL "Enable module LibSVM in OTB" FORCE)
#   set(OTB_USE_OPENCV OFF CACHE BOOL "Enable module OPENCV in OTB" FORCE)
#   set(OTB_USE_SHARK OFF CACHE BOOL "Enable module Shark in OTB" FORCE)
endif()
if(OTBGroup_FeaturesExtraction MATCHES ON)
  set(OTB_USE_MUPARSER ON CACHE BOOL "Enable module MuParser in OTB" FORCE)
  set(OTB_USE_MUPARSERX ON CACHE BOOL "Enable module MuParserX in OTB" FORCE)
# else()
#   set(OTB_USE_MUPARSER OFF CACHE BOOL "Enable module MuParser in OTB" FORCE)
#   set(OTB_USE_MUPARSERX OFF CACHE BOOL "Enable module MuParserX in OTB" FORCE)
endif()
