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

otb_module_test()

set(OTBEndmembersExtractionTests
otbEndmembersExtractionTestDriver.cxx
otbEigenvalueLikelihoodMaximization.cxx
otbVirtualDimensionality.cxx
otbVCAImageFilter.cxx
)

add_executable(otbEndmembersExtractionTestDriver ${OTBEndmembersExtractionTests})
target_link_libraries(otbEndmembersExtractionTestDriver ${OTBEndmembersExtraction-Test_LIBRARIES})
otb_module_target_label(otbEndmembersExtractionTestDriver)

# Tests Declaration

otb_add_test(NAME hyTvEigenvalueLikelihoodMaximizationTest COMMAND otbEndmembersExtractionTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/hyTvEigenvalueLikelihoodMaximizationTest.txt
  ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt
  otbEigenvalueLikelihoodMaximizationTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvEigenvalueLikelihoodMaximizationTest.txt)

otb_add_test(NAME hyTvVirtualDimensionalityTest COMMAND otbEndmembersExtractionTestDriver
  --compare-ascii ${EPSILON_9}
  ${BASELINE_FILES}/hyTvVirtualDimensionalityTest.txt
  ${TEMP}/hyTvVirtualDimensionalityTest.txt
  otbVirtualDimensionalityTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvVirtualDimensionalityTest.txt
  1E-3)

otb_add_test(NAME hyTvVCAImageFilterTest1 COMMAND otbEndmembersExtractionTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
  ${TEMP}/hyTvVCAImageFilterTest.tif
  otbVCAImageFilterTestHighSNR
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${TEMP}/hyTvVCAImageFilterTest.tif
  5 )
