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

# Functions to fetch remote modules.
include(OTBModuleRemote)

file(GLOB remotes "*.remote.cmake")
# download each remote module present next to this file
foreach(remote_module ${remotes})
  include(${remote_module})
endforeach()

set(_remote_module_dep_order otbGRM
                             OTBTemporalGapFilling
                             S1TilingSupportApplications
                             SertitObject
                             DiapOTBModule
                             OTBBioVars
                             OTBFFSforGMM
                             OTBPhenology)

foreach(_remote_module ${_remote_module_dep_order})
  if (Module_${_remote_module} AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_remote_module}")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/${_remote_module}")

    # Same NASTY fix done in Core CMakelists. Here the problem is different,
    # this is not a cyclic dependency :
    # As we install Remote module in Core component, the quickfix made for Core
    # test (not read the test folder in otb_module_impl if we are in Core), work
    # also here... thus at this line, tests are not read
    # Instead of finding another nasty quickfix, do the same here
    if (BUILD_TESTING)
        # part of this code was in OTBModuleEnablement
        # match all otb-module.cmake files in Core and its subdirs.
        file( GLOB_RECURSE _remote_modules_files "${_remote_module}/*otb-module.cmake" )
        foreach(_module_file ${_remote_modules_files})
            # get parent directory
            get_filename_component(_module_dir ${_module_file} DIRECTORY)
            # check if tests exists and build them
            if (EXISTS "${_module_dir}/test/CMakeLists.txt")
                add_subdirectory(${_module_dir}/test)
            endif()
        endforeach()
    endif()

  endif()
endforeach()
