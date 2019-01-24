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

set(OTBMajorityVotingTests
otbMajorityVotingTestDriver.cxx
otbNeighborhoodMajorityVotingImageFilterTest.cxx
)

add_executable(otbMajorityVotingTestDriver ${OTBMajorityVotingTests})
target_link_libraries(otbMajorityVotingTestDriver ${OTBMajorityVoting-Test_LIBRARIES})
otb_module_target_label(otbMajorityVotingTestDriver)

# Tests Declaration

otb_add_test(NAME leTvNeighborhoodMajorityVotingImageFilterTest COMMAND otbMajorityVotingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  otbNeighborhoodMajorityVotingImageFilterTest
  ${INPUTDATA}/Classification/QB_1_ortho_4Cls_N_Classified_OTB.tif
  ${TEMP}/QB_1_ortho_4Cls_N_Classified_OTB_NMV_x2_y5_nodv10_undv7.tif
  false #KeepOriginalLabelBool
  2 #xRadius
  5 #yRadius
  10 #LabelForNoDataPixels
  7 #LabelForUndecidedPixels
  0 #OnlyIsolatedPixels
  )

otb_add_test(NAME leTvNeighborhoodMajorityVotingIsolThresPixTest COMMAND otbMajorityVotingTestDriver
  otbNeighborhoodMajorityVotingImageFilterIsolatedTest
  )

otb_add_test(NAME leTvSVMImageClassificationFilterWithNeighborhoodMajorityVoting COMMAND otbMajorityVotingTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSVMImageClassificationWithNMVFilterOutput.tif
  ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
  otbNeighborhoodMajorityVotingImageFilterTest
  ${BASELINE}/leSVMImageClassificationFilterOutput.tif
  ${TEMP}/leSVMImageClassificationWithNMVFilterOutput.tif
  false
  )
