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
otbKullbackLeiblerSupervizedDistanceImageFilterNew.cxx
otbKullbackLeiblerProfileImageFilterNew.cxx
otbKullbackLeiblerDistanceImageFilter.cxx
otbMeanRatioChangeDetectionTest.cxx
otbKullbackLeiblerDistanceImageFilterNew.cxx
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
  ${BASELINE}/cdMeanDiffImage.hdr
  ${TEMP}/cdMeanDiffImage.hdr
  otbMeanDiffChangeDetectionTest
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  2
  ${TEMP}/cdMeanDiffImage.hdr
  )

otb_add_test(NAME cdTuMultivariateAlterationDetectorImageFilterNew COMMAND otbChangeDetectionTestDriver
  otbMultivariateAlterationDetectorImageFilterNew)

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
  ${BASELINE}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
  ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
  otbKullbackLeiblerProfileImageFilter
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
  5 51)

otb_add_test(NAME cdTuKullbackLeiblerSupervizedDistanceImageFilterNew COMMAND otbChangeDetectionTestDriver
  otbKullbackLeiblerSupervizedDistanceImageFilterNew)

otb_add_test(NAME cdTuKullbackLeiblerProfileImageFilterNew COMMAND otbChangeDetectionTestDriver
  otbKullbackLeiblerProfileImageFilterNew)

otb_add_test(NAME cdTvKullbackLeiblerDistanceImageFilter COMMAND otbChangeDetectionTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
  ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
  otbKullbackLeiblerDistanceImageFilter
  ${INPUTDATA}/GomaAvant.png
  ${INPUTDATA}/GomaApres.png
  ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
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

otb_add_test(NAME cdTuKullbackLeiblerDistanceImageFilterNew COMMAND otbChangeDetectionTestDriver
  otbKullbackLeiblerDistanceImageFilterNew)

otb_add_test(NAME cdTvLHMI COMMAND otbChangeDetectionTestDriver
  otbLHMIChangeDetectionTest
  ${INPUTDATA}/GomaAvantSousEch.png
  ${INPUTDATA}/GomaApresSousEch.png
  2
  ${TEMP}/cdLHMIImage.png
  )

