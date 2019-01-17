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

set(OTBChangeDetectionTests
otbChangeDetectionTestDriver.cxx
otbJHMIChangeDetectionTest.cxx
otbMeanDiffChangeDetectionTest.cxx
otbMultivariateAlterationDetectorImageFilter.cxx
otbCBAMIChangeDetectionTest.cxx
otbKullbackLeiblerSupervizedDistanceImageFilter.cxx
otbCorrelChangeDetectionTest.cxx
otbKullbackLeiblerProfileImageFilter.cxx
otbKullbackLeiblerDistanceImageFilter.cxx
otbMeanRatioChangeDetectionTest.cxx
otbLHMIChangeDetectionTest.cxx
)

add_executable(otbChangeDetectionTestDriver ${OTBChangeDetectionTests})
target_link_libraries(otbChangeDetectionTestDriver ${OTBChangeDetection-Test_LIBRARIES})
otb_module_target_label(otbChangeDetectionTestDriver)

# Tests Declaration

otb_add_test(NAME cdTvJHMI COMMAND otbChangeDetectionTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/cdJHMIImage.png
  ${TEMP}/cdJHMIImage.png
  otbJHMIChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdJHMIImage.png
  )

otb_add_test(NAME cdTvMeanDiff COMMAND otbChangeDetectionTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/cdMeanDiffImage.tif
  ${TEMP}/cdMeanDiffImage.tif
  otbMeanDiffChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdMeanDiffImage.tif
  )

otb_add_test(NAME cdTvMultivariateAlterationDetectorImageFilterSameNbBands COMMAND otbChangeDetectionTestDriver
  --compare-image 0.025
  ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
  ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
  otbMultivariateAlterationDetectorImageFilter
  ${INPUTDATA}/Spot5-Gloucester-before.tif
  ${INPUTDATA}/Spot5-Gloucester-after.tif
  ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif)

otb_add_test(NAME cdTvMultivariateAlterationDetectorImageFilterDiffNbBands COMMAND otbChangeDetectionTestDriver
  --compare-image 0.025
  ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif
  ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif
  otbMultivariateAlterationDetectorImageFilter
  ${INPUTDATA}/Spot5-ERS-Gloucester-before.tif
  ${INPUTDATA}/Spot5-Gloucester-after.tif
  ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif)

otb_add_test(NAME cdTvCBAMI COMMAND otbChangeDetectionTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/cdCBAMIImage.png
  ${TEMP}/cdCBAMIImage.png
  otbCBAMIChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdCBAMIImage.png
  )

otb_add_test(NAME cdTvKullbackLeiblerSupervizedDistanceImageFilter COMMAND otbChangeDetectionTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
  ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
  otbKullbackLeiblerSupervizedDistanceImageFilter
  ${INPUTDATA}/ERSBefore.png
  ${INPUTDATA}/ERSAfter.png
  ${INPUTDATA}/ROI_ERS.png
  35
  ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png)

otb_add_test(NAME cdTvCorrel COMMAND otbChangeDetectionTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/cdCorrelImage.png
  ${TEMP}/cdCorrelImage.png
  otbCorrelChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdCorrelImage.png
  )

otb_add_test(NAME cdTvKullbackLeiblerProfileImageFilter COMMAND otbChangeDetectionTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/cdTVKullbackLeiblerProfileImageFilterOutput.tif
  ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.tif
  otbKullbackLeiblerProfileImageFilter
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.tif
  5 51)

otb_add_test(NAME cdTvKullbackLeiblerDistanceImageFilter COMMAND otbChangeDetectionTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/cdTVKullbackLeiblerDistanceImageFilterOutput.tif
  ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.tif
  otbKullbackLeiblerDistanceImageFilter
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.tif
  35)

otb_add_test(NAME cdTvMeanRatio COMMAND otbChangeDetectionTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/cdMeanRatioImage.png
  ${TEMP}/cdMeanRatioImage.png
  otbMeanRatioChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdMeanRatioImage.png
  )

otb_add_test(NAME cdTvLHMI COMMAND otbChangeDetectionTestDriver
  otbLHMIChangeDetectionTest
  ${INPUTDATA}/GomaAvantSousEch.png
  ${INPUTDATA}/GomaApresSousEch.png
  2
  ${TEMP}/cdLHMIImage.png
  )

