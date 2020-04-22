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

# kMeans Shark related tests

otb_add_test(NAME leTvSharkKMeansMachineLearningModel COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelTrain
  ${INPUTDATA}/letter.scale
  ${TEMP}/shark_km_model.txt
  )

otb_add_test(NAME otbSharkKMeansMachineLearningModelPredict COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelPredict
  ${INPUTDATA}/letter.scale
  ${INPUTDATA}/Classification/shark_km_model.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanRead COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )

otb_add_test(NAME leTvSharkKMeansMachineLearningModelCanReadFail COMMAND otbUnsupervisedTestDriver
  otbSharkKMeansMachineLearningModelCanRead
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_RFmodel.txt
  )

set_property(TEST leTvSharkKMeansMachineLearningModelCanReadFail PROPERTY WILL_FAIL true)



otb_add_test(NAME leTvImageClassificationFilterSharkKMeans COMMAND  otbUnsupervisedTestDriver
  --compare-n-images ${NOTOL} 1
  ${BASELINE}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  otbSharkUnsupervisedImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  )


otb_add_test(NAME leTvImageClassificationFilterSharkKMeansMask COMMAND  otbUnsupervisedTestDriver
  --compare-n-images ${NOTOL} 1
  ${BASELINE}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  otbSharkUnsupervisedImageClassificationFilter
  ${INPUTDATA}/Classification/QB_1_ortho.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskOutput.tif
  ${TEMP}/leSharkUnsupervisedImageClassificationFilterWithMaskConfidence.tif
  1
  ${INPUTDATA}/Classification/otbSharkImageClassificationFilter_KMeansmodel.txt
  ${INPUTDATA}/Classification/QB_1_ortho_mask.tif
  )
