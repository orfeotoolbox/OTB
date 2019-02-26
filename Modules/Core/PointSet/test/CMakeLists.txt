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

set(OTBPointSetTests
otbPointSetTestDriver.cxx
otbPointSetSourceTest.cxx
otbPointSetExtractROITest.cxx
otbSimplePointCountStrategyTest.cxx
otbRandomPointSetSourceTest.cxx
otbImageToPointSetFilterTest.cxx
otbTransformPointSetFilterTest.cxx
otbThresholdImageToPointSetTest.cxx
)

add_executable(otbPointSetTestDriver ${OTBPointSetTests})
target_link_libraries(otbPointSetTestDriver ${OTBPointSet-Test_LIBRARIES})
otb_module_target_label(otbPointSetTestDriver)

# Tests Declaration

otb_add_test(NAME coTuPointSetSource COMMAND otbPointSetTestDriver
  otbPointSetSourceTest)

otb_add_test(NAME bfTvPointSetExtractROI COMMAND otbPointSetTestDriver
  otbPointSetExtractROITest)

otb_add_test(NAME feTvSimplePointCountStrategyTest COMMAND otbPointSetTestDriver
  otbSimplePointCountStrategyTest)


otb_add_test(NAME bfTvRandomPointSetSource COMMAND otbPointSetTestDriver
  otbRandomPointSetSourceTest)

otb_add_test(NAME bfTuImageToPointSetFilterTest COMMAND otbPointSetTestDriver
  otbImageToPointSetFilterTest)

otb_add_test(NAME bfTvTransformPointSetFilter COMMAND otbPointSetTestDriver
  otbTransformPointSetFilterTest)

otb_add_test(NAME feTvThresholdImageToPointSet1 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To255.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
  0 255 )

otb_add_test(NAME feTvThresholdImageToPointSet2 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To0.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
  0 0 )

otb_add_test(NAME feTvThresholdImageToPointSet3 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_255To255.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
  255 255 )

otb_add_test(NAME feTvThresholdImageToPointSet4 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_1To200.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
  1 200 )

