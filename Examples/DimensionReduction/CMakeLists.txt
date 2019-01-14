#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

project(DimensionReductionExamples)

add_executable(ICAExample ICAExample.cxx)
target_link_libraries(ICAExample ${OTB_LIBRARIES})

add_executable(MaximumAutocorrelationFactor MaximumAutocorrelationFactor.cxx)
target_link_libraries(MaximumAutocorrelationFactor ${OTB_LIBRARIES})

add_executable(MNFExample MNFExample.cxx)
target_link_libraries(MNFExample ${OTB_LIBRARIES})

add_executable(NAPCAExample NAPCAExample.cxx)
target_link_libraries(NAPCAExample ${OTB_LIBRARIES})

add_executable(PCAExample PCAExample.cxx)
target_link_libraries(PCAExample ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
