#
# Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
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

# Configuration options for redhat 8

set(site_option
    "USE_SYSTEM_OPENSSL:BOOL=ON"
    "OTB_USE_OPENMP:BOOL=OFF"
)

if (OTB_FULL_BUILD)
  list(APPEND site_option
    "OTB_BUILD_FeaturesExtraction:BOOL=ON"
    "OTB_BUILD_Hyperspectral:BOOL=ON"
    "OTB_BUILD_Learning:BOOL=ON"
    "OTB_BUILD_Miscellaneous:BOOL=ON"
    "OTB_BUILD_Remote:BOOL=ON"
    "OTB_BUILD_SAR:BOOL=ON"
    "OTB_BUILD_Segmentation:BOOL=ON"
    "OTB_BUILD_StereoProcessing:BOOL=ON"
    "OTB_USE_LIBSVM:BOOL=ON"
    "OTB_USE_MUPARSER:BOOL=ON"
    "OTB_USE_MUPARSERX:BOOL=ON"
    "OTB_USE_OPENCV:BOOL=ON"
    "OTB_USE_SHARK:BOOL=ON")
else()
  list(APPEND site_option
    "OTB_BUILD_FeaturesExtraction:BOOL=OFF"
    "OTB_BUILD_Hyperspectral:BOOL=OFF"
    "OTB_BUILD_Learning:BOOL=OFF"
    "OTB_BUILD_Miscellaneous:BOOL=OFF"
    "OTB_BUILD_Remote:BOOL=OFF"
    "OTB_BUILD_SAR:BOOL=OFF"
    "OTB_BUILD_Segmentation:BOOL=OFF"
    "OTB_BUILD_StereoProcessing:BOOL=OFF"
    "OTB_USE_LIBSVM:BOOL=OFF"
    "OTB_USE_MUPARSER:BOOL=OFF"
    "OTB_USE_MUPARSERX:BOOL=OFF"
    "OTB_USE_OPENCV:BOOL=OFF"
    "OTB_USE_SHARK:BOOL=OFF")
endif()

