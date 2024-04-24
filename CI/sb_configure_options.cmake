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
# For know OTB_DEPENDS is build with default value

set ( cmake_configure_option
"CMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}
CMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}")

if(WIN32)
  set (cmake_configure_option
"${cmake_configure_option}
CMAKE_C_COMPILER_LAUNCHER=buildcache
CMAKE_CXX_COMPILER_LAUNCHER=buildcache")
endif()

set ( full_otb_options )
if(OTB_FULL_BUILD)
  set(full_otb_options "OTB_BUILD_FeaturesExtraction:BOOL=ON
OTB_BUILD_Hyperspectral:BOOL=ON
OTB_BUILD_Learning:BOOL=ON
OTB_BUILD_Miscellaneous:BOOL=ON
OTB_BUILD_SAR:BOOL=ON
OTB_BUILD_Segmentation:BOOL=ON 
OTB_BUILD_StereoProcessing:BOOL=ON"
)
endif()

set(concat_options
"${cmake_configure_option}
${full_otb_options}
")

#Transform the previous string in list
string (REPLACE "\n" ";" sb_options ${concat_options})

foreach(item ${sb_options})
  set( SB_CONFIGURE_OPTIONS "${SB_CONFIGURE_OPTIONS}-D${item};")
endforeach(item)
