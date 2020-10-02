#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

otb_add_test(NAME leTvSharkRFMachineLearningModel COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModel
  ${INPUTDATA}/letter_light.scale
  ${TEMP}/shark_rf_model.txt
  )

otb_add_test(NAME leTvSharkRFMachineLearningModelCanRead COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

otb_add_test(NAME leTvSharkRFMachineLearningModelCanReadFail COMMAND otbSupervisedTestDriver
  otbSharkRFMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )
set_property(TEST leTvSharkRFMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)


otb_add_test(NAME leTvImageClassificationFilterSharkFast COMMAND  otbSupervisedTestDriver
  --compare-n-images ${NOTOL} 2
  ${BASELINE}/leSharkImageClassificationFilterOutput.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  ${BASELINE}/leSharkImageClassificationFilterConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterConfidence.tif
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterOutput.tif
  ${TEMP}/leSharkImageClassificationFilterConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterProba.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

# This test has been added for benchmarking purposes. However, it is
# far too long to be part of regression testing

# otb_add_test(NAME leTvImageClassificationFilterSharkClassic COMMAND  otbSupervisedTestDriver
#   otbSharkImageClassificationFilter
#   ${INPUTDATA}/Classification/QB_1_ortho.tif
#   ${TEMP}/leSharkImageClassificationFilterOutput.tif
#   ${TEMP}/leSharkImageClassificationFilterConfidence.tif
#   0
#   ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
#   )

otb_add_test(NAME leTvImageClassificationFilterSharkFastMask COMMAND  otbSupervisedTestDriver
  --compare-n-images ${NOTOL} 2
  ${BASELINE}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${BASELINE}/leSharkImageClassificationFilterWithMaskConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskConfidence.tif
  otbSharkImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskConfidence.tif
  ${TEMP}/leSharkImageClassificationFilterWithMaskProba.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  ${INPUTDATA}/Classification/QB_1_ortho_mask.tif
  )

