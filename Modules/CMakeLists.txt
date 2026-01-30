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

# set(CPACK_COMPONENTS_ALL "Core" "FeaturesExtraction" "Learning" "Miscellaneous" "Remote" "SAR" "Segmentation" "StereoProcessing")
add_subdirectory(ThirdParty)
add_subdirectory(Core)
include("OTBInternalModules.cmake")

# cpack_add_component(Remote
#                     [DISPLAY_NAME Remote]
#                     [DESCRIPTION "Contains all Remote modules libraries and applications for OTB" ]
#                     [OPTIONAL])
