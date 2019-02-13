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

project(FeatureExtractionExamples)

add_executable(AsymmetricFusionOfLineDetectorExample AsymmetricFusionOfLineDetectorExample.cxx)
target_link_libraries(AsymmetricFusionOfLineDetectorExample ${OTB_LIBRARIES})

add_executable(CloudDetectionExample CloudDetectionExample.cxx)
target_link_libraries(CloudDetectionExample ${OTB_LIBRARIES})

add_executable(ComplexMomentPathExample ComplexMomentPathExample.cxx)
target_link_libraries(ComplexMomentPathExample ${OTB_LIBRARIES})

add_executable(ComplexMomentsImageFunctionExample ComplexMomentsImageFunctionExample.cxx)
target_link_libraries(ComplexMomentsImageFunctionExample ${OTB_LIBRARIES})

add_executable(CorrelationLineDetectorExample CorrelationLineDetectorExample.cxx)
target_link_libraries(CorrelationLineDetectorExample ${OTB_LIBRARIES})

add_executable(EdgeDensityExample EdgeDensityExample.cxx)
target_link_libraries(EdgeDensityExample ${OTB_LIBRARIES})

#OTBRoadExtraction depends OTBMathParser. But OTB_USE_MUPARSER is OFF
if(OTBMathParser_LOADED)
add_executable(ExtractRoadByStepsExample ExtractRoadByStepsExample.cxx)
target_link_libraries(ExtractRoadByStepsExample ${OTB_LIBRARIES})

add_executable(ExtractRoadExample ExtractRoadExample.cxx)
target_link_libraries(ExtractRoadExample ${OTB_LIBRARIES})

add_executable(ParallelLineDetectionExample ParallelLineDetectionExample.cxx)
target_link_libraries(ParallelLineDetectionExample ${OTB_LIBRARIES})
endif()

add_executable(FlusserMomentsImageFunctionExample FlusserMomentsImageFunctionExample.cxx)
target_link_libraries(FlusserMomentsImageFunctionExample ${OTB_LIBRARIES})

add_executable(HarrisExample HarrisExample.cxx)
target_link_libraries(HarrisExample ${OTB_LIBRARIES})

add_executable(HuMomentsImageFunctionExample HuMomentsImageFunctionExample.cxx)
target_link_libraries(HuMomentsImageFunctionExample ${OTB_LIBRARIES})

add_executable(LineSegmentDetectorExample LineSegmentDetectorExample.cxx)
target_link_libraries(LineSegmentDetectorExample ${OTB_LIBRARIES})

add_executable(RightAngleDetectionExample RightAngleDetectionExample.cxx)
target_link_libraries(RightAngleDetectionExample ${OTB_LIBRARIES})

add_executable(PanTexExample PanTexExample.cxx)
target_link_libraries(PanTexExample ${OTB_LIBRARIES})

add_executable(RatioLineDetectorExample RatioLineDetectorExample.cxx)
target_link_libraries(RatioLineDetectorExample ${OTB_LIBRARIES})

add_executable(SeamCarvingExample SeamCarvingExample.cxx)
target_link_libraries(SeamCarvingExample ${OTB_LIBRARIES})

add_executable(SeamCarvingOtherExample SeamCarvingOtherExample.cxx)
target_link_libraries(SeamCarvingOtherExample ${OTB_LIBRARIES})

add_executable(SFSExample SFSExample.cxx)
target_link_libraries(SFSExample ${OTB_LIBRARIES})

add_executable(SURFExample SURFExample.cxx)
target_link_libraries(SURFExample ${OTB_LIBRARIES})

add_executable(TextureExample TextureExample.cxx)
target_link_libraries(TextureExample ${OTB_LIBRARIES})

add_executable(ThresholdToPointSetExample ThresholdToPointSetExample.cxx)
target_link_libraries(ThresholdToPointSetExample ${OTB_LIBRARIES})

add_executable(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx)
target_link_libraries(TouziEdgeDetectorExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
