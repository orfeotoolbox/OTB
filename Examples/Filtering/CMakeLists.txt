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

project(FilteringExamples)

add_executable(CompositeFilterExample CompositeFilterExample.cxx)
target_link_libraries(CompositeFilterExample ${OTB_LIBRARIES})

add_executable(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx)
target_link_libraries(DanielssonDistanceMapImageFilter ${OTB_LIBRARIES})

add_executable(GeometriesChangeSpatialReference GeometriesChangeSpatialReference.cxx)
target_link_libraries(GeometriesChangeSpatialReference ${OTB_LIBRARIES})

add_executable(GeometriesFilter GeometriesFilter.cxx)
target_link_libraries(GeometriesFilter ${OTB_LIBRARIES})

add_executable(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx)
target_link_libraries(SecondDerivativeRecursiveGaussianImageFilter ${OTB_LIBRARIES})

if (Module_OTBVectorDataRendering)
  add_executable(RasterizationExample RasterizationExample.cxx)
  target_link_libraries(RasterizationExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
