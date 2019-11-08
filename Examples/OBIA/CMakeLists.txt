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

project(OBIAExamples)

add_executable(HooverMetricsEstimation HooverMetricsEstimation.cxx)
target_link_libraries(HooverMetricsEstimation ${OTB_LIBRARIES})

add_executable(LabelMapToVectorData LabelMapToVectorData.cxx)
target_link_libraries(LabelMapToVectorData ${OTB_LIBRARIES})

if(OTBMathParser_LOADED)
add_executable(RadiometricAttributesLabelMapFilterExample RadiometricAttributesLabelMapFilterExample.cxx)
target_link_libraries(RadiometricAttributesLabelMapFilterExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
