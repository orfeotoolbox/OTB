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

project(SegmentationExamples)

add_executable(ConfidenceConnected ConfidenceConnected.cxx)
target_link_libraries(ConfidenceConnected ${OTB_LIBRARIES})

add_executable(ConnectedThresholdImageFilter ConnectedThresholdImageFilter.cxx)
target_link_libraries(ConnectedThresholdImageFilter ${OTB_LIBRARIES})

add_executable(FastMarchingImageFilter FastMarchingImageFilter.cxx)
target_link_libraries(FastMarchingImageFilter ${OTB_LIBRARIES})

add_executable(IsolatedConnectedImageFilter IsolatedConnectedImageFilter.cxx)
target_link_libraries(IsolatedConnectedImageFilter ${OTB_LIBRARIES})

add_executable(LabelizeNeighborhoodConnectedImageFilter LabelizeNeighborhoodConnectedImageFilter.cxx)
target_link_libraries(LabelizeNeighborhoodConnectedImageFilter ${OTB_LIBRARIES})

add_executable(NeighborhoodConnectedImageFilter NeighborhoodConnectedImageFilter.cxx)
target_link_libraries(NeighborhoodConnectedImageFilter ${OTB_LIBRARIES})

add_executable(OtsuMultipleThresholdImageFilter OtsuMultipleThresholdImageFilter.cxx)
target_link_libraries(OtsuMultipleThresholdImageFilter ${OTB_LIBRARIES})

add_executable(OtsuThresholdImageFilter OtsuThresholdImageFilter.cxx)
target_link_libraries(OtsuThresholdImageFilter ${OTB_LIBRARIES})

if(OTBMathParser_LOADED)
  add_executable(StreamingMeanShiftSegmentation StreamingMeanShiftSegmentation.cxx)
  target_link_libraries(StreamingMeanShiftSegmentation ${OTB_LIBRARIES})
endif()

add_executable(VectorConfidenceConnected VectorConfidenceConnected.cxx)
target_link_libraries(VectorConfidenceConnected ${OTB_LIBRARIES})

add_executable(WatershedSegmentation WatershedSegmentation.cxx)
target_link_libraries(WatershedSegmentation ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
