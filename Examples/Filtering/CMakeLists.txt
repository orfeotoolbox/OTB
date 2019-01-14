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

add_executable(BinaryThresholdImageFilter BinaryThresholdImageFilter.cxx)
target_link_libraries(BinaryThresholdImageFilter ${OTB_LIBRARIES})

add_executable(CannyEdgeDetectionImageFilter CannyEdgeDetectionImageFilter.cxx)
target_link_libraries(CannyEdgeDetectionImageFilter ${OTB_LIBRARIES})

add_executable(CompositeFilterExample CompositeFilterExample.cxx)
target_link_libraries(CompositeFilterExample ${OTB_LIBRARIES})

add_executable(DanielssonDistanceMapImageFilter DanielssonDistanceMapImageFilter.cxx)
target_link_libraries(DanielssonDistanceMapImageFilter ${OTB_LIBRARIES})

add_executable(DerivativeImageFilter DerivativeImageFilter.cxx)
target_link_libraries(DerivativeImageFilter ${OTB_LIBRARIES})

add_executable(DiscreteGaussianImageFilter DiscreteGaussianImageFilter.cxx)
target_link_libraries(DiscreteGaussianImageFilter ${OTB_LIBRARIES})

add_executable(GeometriesChangeSpatialReference GeometriesChangeSpatialReference.cxx)
target_link_libraries(GeometriesChangeSpatialReference ${OTB_LIBRARIES})

add_executable(GeometriesFilter GeometriesFilter.cxx)
target_link_libraries(GeometriesFilter ${OTB_LIBRARIES})

add_executable(GradientAnisotropicDiffusionImageFilter GradientAnisotropicDiffusionImageFilter.cxx)
target_link_libraries(GradientAnisotropicDiffusionImageFilter ${OTB_LIBRARIES})

add_executable(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx)
target_link_libraries(GradientMagnitudeImageFilter ${OTB_LIBRARIES})

add_executable(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx)
target_link_libraries(GradientMagnitudeRecursiveGaussianImageFilter ${OTB_LIBRARIES})

add_executable(LaplacianRecursiveGaussianImageFilter1 LaplacianRecursiveGaussianImageFilter1.cxx)
target_link_libraries(LaplacianRecursiveGaussianImageFilter1 ${OTB_LIBRARIES})

add_executable(LaplacianRecursiveGaussianImageFilter2 LaplacianRecursiveGaussianImageFilter2.cxx)
target_link_libraries(LaplacianRecursiveGaussianImageFilter2 ${OTB_LIBRARIES})

add_executable(MathematicalMorphologyBinaryFilters MathematicalMorphologyBinaryFilters.cxx)
target_link_libraries(MathematicalMorphologyBinaryFilters ${OTB_LIBRARIES})

add_executable(MathematicalMorphologyGrayscaleFilters MathematicalMorphologyGrayscaleFilters.cxx)
target_link_libraries(MathematicalMorphologyGrayscaleFilters ${OTB_LIBRARIES})

add_executable(MeanImageFilter MeanImageFilter.cxx)
target_link_libraries(MeanImageFilter ${OTB_LIBRARIES})

add_executable(MedianImageFilter MedianImageFilter.cxx)
target_link_libraries(MedianImageFilter ${OTB_LIBRARIES})

add_executable(SecondDerivativeRecursiveGaussianImageFilter SecondDerivativeRecursiveGaussianImageFilter.cxx)
target_link_libraries(SecondDerivativeRecursiveGaussianImageFilter ${OTB_LIBRARIES})

add_executable(ThresholdImageFilter ThresholdImageFilter.cxx)
target_link_libraries(ThresholdImageFilter ${OTB_LIBRARIES})

if (Module_OTBVectorDataRendering)
  add_executable(RasterizationExample RasterizationExample.cxx)
  target_link_libraries(RasterizationExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
