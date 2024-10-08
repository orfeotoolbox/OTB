#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

set(OTBTexturesTests
otbTexturesTestDriver.cxx
otbScalarImageToHigherOrderTexturesFilter.cxx
otbHaralickTexturesImageFunction.cxx
otbGreyLevelCooccurrenceIndexedList.cxx
otbScalarImageToTexturesFilter.cxx
otbSFSTexturesImageFilterTest.cxx
otbScalarImageToAdvancedTexturesFilter.cxx
otbScalarImageToPanTexTextureFilter.cxx
)

add_executable(otbTexturesTestDriver ${OTBTexturesTests})
target_link_libraries(otbTexturesTestDriver ${OTBTextures-Test_LIBRARIES})
otb_module_target_label(otbTexturesTestDriver)


otb_add_test(NAME feTvScalarImageToHigherOrderTexturesFilter COMMAND otbTexturesTestDriver
  otbScalarImageToHigherOrderTexturesFilter
  ${INPUTDATA}/Mire_Cosinus.png)


otb_add_test(NAME feTvHaralickTexturesImageFunction COMMAND otbTexturesTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvHaralickTexturesImageFunction.txt
  ${TEMP}/feTvHaralickTexturesImageFunction.txt
  otbHaralickTexturesImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvHaralickTexturesImageFunction.txt
  451846.014047961 5412466.57452216
  )

otb_add_test(NAME feTvGreyLevelCooccurrenceIndexedList COMMAND otbTexturesTestDriver
  otbGreyLevelCooccurrenceIndexedList
  )

otb_add_test(NAME feTvScalarImageToTexturesFilter COMMAND otbTexturesTestDriver
  --compare-n-images ${EPSILON_10} 8
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputEnergy.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputEnergy.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputEntropy.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputEntropy.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputCorrelation.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputCorrelation.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputInverseDifferenceMoment.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputInverseDifferenceMoment.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputInertia.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputInertia.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputClusterShade.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputClusterShade.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputClusterProminence.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputClusterProminence.tif
  ${BASELINE}/feTvScalarImageToTexturesFilterOutputHaralickCorrelation.tif
  ${TEMP}/feTvScalarImageToTexturesFilterOutputHaralickCorrelation.tif
  otbScalarImageToTexturesFilter
  ${INPUTDATA}/Mire_Cosinus.png
  ${TEMP}/feTvScalarImageToTexturesFilterOutput
  8 3 2 2)


otb_add_test(NAME feTvSFSTexturesImageFilterTest COMMAND otbTexturesTestDriver
  --compare-n-images ${EPSILON_8}
  5
  ${BASELINE}/feTvSFSTexturesLengthImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesLengthImageFilterTest.tif
  ${BASELINE}/feTvSFSTexturesWidthImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesWidthImageFilterTest.tif
  ${BASELINE}/feTvSFSTexturesWMeanImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesWMeanImageFilterTest.tif
  ${BASELINE}/feTvSFSTexturesRatioImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesRatioImageFilterTest.tif
  ${BASELINE}/feTvSFSTexturesSDImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesSDImageFilterTest.tif
  otbSFSTexturesImageFilterTest
  ${INPUTDATA}/poupees_sub_c1.png
  ${TEMP}/feTvSFSTexturesLengthImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesWidthImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesWMeanImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesRatioImageFilterTest.tif
  ${TEMP}/feTvSFSTexturesSDImageFilterTest.tif
  50 # spectral threshold
  20 # spatial threshold
  15 # direction
  4 # max min/max number takes in care for ratio
  0.6 # alpha value
  )




otb_add_test(NAME feTvScalarImageToAdvancedTexturesFilter COMMAND otbTexturesTestDriver
  --compare-n-images ${NOTOL} 10
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputVariance.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputMean.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDissimilarity.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDissimilarity.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumAverage.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumAverage.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumVariance.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumVariance.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputSumEntropy.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputSumEntropy.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceEntropy.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceEntropy.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceVariance.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputDifferenceVariance.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputIC1.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputIC1.tif
  ${BASELINE}/feTvScalarImageToAdvancedTexturesFilterOutputIC2.tif
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutputIC2.tif
  otbScalarImageToAdvancedTexturesFilter
  ${INPUTDATA}/Mire_Cosinus.png
  ${TEMP}/feTvScalarImageToAdvancedTexturesFilterOutput
  8 5 1 1)

otb_add_test(NAME feTvScalarImageToPanTexTextureFilter COMMAND otbTexturesTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/feTvScalarImageToPanTexTextureFilterOutputPanTex.tif
  ${TEMP}/feTvScalarImageToPanTexTextureFilterOutputPanTex.tif
  otbScalarImageToPanTexTextureFilter
  ${INPUTDATA}/Mire_Cosinus.png
  ${TEMP}/feTvScalarImageToPanTexTextureFilterOutput
  8 5)
