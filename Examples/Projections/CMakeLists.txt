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

project(ProjectionsExamples)

add_executable(CoordinateToNameExample CoordinateToNameExample.cxx)
target_link_libraries(CoordinateToNameExample ${OTB_LIBRARIES})

add_executable(EstimateRPCSensorModelExample EstimateRPCSensorModelExample.cxx)
target_link_libraries(EstimateRPCSensorModelExample ${OTB_LIBRARIES})

add_executable(GeometriesProjectionExample GeometriesProjectionExample.cxx)
target_link_libraries(GeometriesProjectionExample ${OTB_LIBRARIES})

add_executable(OrthoRectificationExample OrthoRectificationExample.cxx)
target_link_libraries(OrthoRectificationExample ${OTB_LIBRARIES})

add_executable(PlaceNameToLonLatExample PlaceNameToLonLatExample.cxx)
target_link_libraries(PlaceNameToLonLatExample ${OTB_LIBRARIES})

add_executable(VectorDataExtractROIExample VectorDataExtractROIExample.cxx)
target_link_libraries(VectorDataExtractROIExample ${OTB_LIBRARIES})

add_executable(VectorDataProjectionExample VectorDataProjectionExample.cxx)
target_link_libraries(VectorDataProjectionExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
