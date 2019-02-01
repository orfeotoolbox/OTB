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

project(PatentedExamples)

add_executable(EstimateAffineTransformationExample EstimateAffineTransformationExample.cxx)
target_link_libraries(EstimateAffineTransformationExample ${OTB_LIBRARIES})

if (OTBSiftFast_LOADED)
add_executable(SIFTDisparityMapEstimation SIFTDisparityMapEstimation.cxx)
target_link_libraries(SIFTDisparityMapEstimation ${OTB_LIBRARIES})

add_executable(SIFTExample SIFTExample.cxx)
target_link_libraries(SIFTExample ${OTB_LIBRARIES})

add_executable(SIFTDensityExample SIFTDensityExample.cxx)
target_link_libraries(SIFTDensityExample ${OTB_LIBRARIES})

add_executable(SIFTFastExample SIFTFastExample.cxx)
target_link_libraries(SIFTFastExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
