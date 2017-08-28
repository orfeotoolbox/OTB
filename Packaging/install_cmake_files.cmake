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
include(patch_cmake_files.cmake)

function(install_cmake_files)
 patch_cmake_files(NAME "ITK"
   VERSION "${PKG_ITK_SB_VERSION}"
   MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
   REPLACE_VAR "ITK_INSTALL_PREFIX"
   )
 
 patch_cmake_files(NAME "OTB"
   VERSION "${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}"
   MATCH_STRING  "${CMAKE_INSTALL_PREFIX}"
   REPLACE_VAR "OTB_INSTALL_PREFIX"
   )
endfunction()
