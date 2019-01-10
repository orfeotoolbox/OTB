#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

set(OTBUnmixingTests
otbUnmixingTestDriver.cxx
otbMDMDNMFImageFilter.cxx
otbNCLSUnmixingImageFilter.cxx
otbISRAUnmixingImageFilter.cxx
otbUnConstrainedLeastSquareImageFilter.cxx
otbSparseUnmixingImageFilter.cxx
)

add_executable(otbUnmixingTestDriver ${OTBUnmixingTests})
target_link_libraries(otbUnmixingTestDriver ${OTBUnmixing-Test_LIBRARIES})
otb_module_target_label(otbUnmixingTestDriver)

# Tests Declaration

otb_add_test(NAME hyTvMDMDNMFImageFilterTest2 COMMAND otbUnmixingTestDriver
  otbMDMDNMFImageFilterTest2
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest2.tif
  100)

otb_add_test(NAME hyTvMDMDNMFImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/hyTvMDMDNMFImageFilterTest.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
  otbMDMDNMFImageFilterTest
  ${INPUTDATA}/cupriteSubHsi.tif
  ${TEMP}/hyTvMDMDNMFImageFilterTest.tif
  100)

otb_add_test(NAME hyTvNCLSUnmixingImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvNCLSImageFilterTest.tif
  ${TEMP}/hyTvNCLSImageFilterTest.tif
  otbNCLSUnmixingImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvNCLSImageFilterTest.tif
  10)

otb_add_test(NAME hyTvISRAUnmixingImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvISRAUnmixingImageFilterTest.tif
  ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
  otbISRAUnmixingImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvISRAUnmixingImageFilterTest.tif
  10)

otb_add_test(NAME hyTvUnConstrainedLeastSquareImageFilterTest COMMAND otbUnmixingTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE_FILES}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
  ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif
  otbUnConstrainedLeastSquareImageFilterTest
  ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
  ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
  ${TEMP}/hyTvUnConstrainedLeastSquareImageFilterTest.tif)

