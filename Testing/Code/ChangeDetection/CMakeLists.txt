
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(CHANGEDETECTION_TESTS1 ${CXX_TEST_PATH}/otbChangeDetectionTests1)
set(CHANGEDETECTION_TESTS2 ${CXX_TEST_PATH}/otbChangeDetectionTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANGEDETECTION_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::CBAMI   ------------------------------

add_test(cdTvCBAMI ${CHANGEDETECTION_TESTS1}
  --compare-image ${NOTOL}   ${BASELINE}/cdCBAMIImage.png
                    ${TEMP}/cdCBAMIImage.png
        otbCBAMIChangeDetectionTest
         ${INPUTDATA}/GomaAvant.png
         ${INPUTDATA}/GomaApres.png
         2
         ${TEMP}/cdCBAMIImage.png
         )

add_test(cdTvCorrel ${CHANGEDETECTION_TESTS1}
  --compare-image ${NOTOL}   ${BASELINE}/cdCorrelImage.png
                    ${TEMP}/cdCorrelImage.png
        otbCorrelChangeDetectionTest
         ${INPUTDATA}/GomaAvant.png
         ${INPUTDATA}/GomaApres.png
         2
         ${TEMP}/cdCorrelImage.png
         )

add_test(cdTvMeanDiff ${CHANGEDETECTION_TESTS1}
  --compare-image ${EPSILON_10}
                  ${BASELINE}/cdMeanDiffImage.hdr
                  ${TEMP}/cdMeanDiffImage.hdr
        otbMeanDiffChangeDetectionTest
         ${INPUTDATA}/GomaAvant.png
         ${INPUTDATA}/GomaApres.png
         2
         ${TEMP}/cdMeanDiffImage.hdr
         )

add_test(cdTvMeanRatio ${CHANGEDETECTION_TESTS1}
  --compare-image ${NOTOL}   ${BASELINE}/cdMeanRatioImage.png
                    ${TEMP}/cdMeanRatioImage.png
        otbMeanRatioChangeDetectionTest
         ${INPUTDATA}/GomaAvant.png
         ${INPUTDATA}/GomaApres.png
         2
         ${TEMP}/cdMeanRatioImage.png
         )

add_test(cdTvLHMI ${CHANGEDETECTION_TESTS1}
#  --compare-image ${NOTOL}   ${BASELINE}/cdLHMIImage.png
#                    ${TEMP}/cdLHMIImage.png
        otbLHMIChangeDetectionTest
         ${INPUTDATA}/GomaAvantSousEch.png
         ${INPUTDATA}/GomaApresSousEch.png
         2
         ${TEMP}/cdLHMIImage.png
         )

add_test(cdTvJHMI ${CHANGEDETECTION_TESTS1}
  --compare-image ${NOTOL}
                  ${BASELINE}/cdJHMIImage.png
                  ${TEMP}/cdJHMIImage.png
        otbJHMIChangeDetectionTest
         ${INPUTDATA}/GomaAvant.png
         ${INPUTDATA}/GomaApres.png
         2
         ${TEMP}/cdJHMIImage.png
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANGEDETECTION_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(cdTuKullbackLeiblerDistanceImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerDistanceImageFilterNew)

add_test(cdTvKullbackLeiblerDistanceImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${EPSILON_10}
                         ${BASELINE}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
                         ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
         otbKullbackLeiblerDistanceImageFilter
                        ${INPUTDATA}/GomaAvant.png
                        ${INPUTDATA}/GomaApres.png
                        ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
                        35)

add_test(cdTuKullbackLeiblerProfileImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerProfileImageFilterNew)

add_test(cdTvKullbackLeiblerProfileImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${EPSILON_10}
                         ${BASELINE}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
                         ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
         otbKullbackLeiblerProfileImageFilter
                        ${INPUTDATA}/GomaAvant.png
                        ${INPUTDATA}/GomaApres.png
                        ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
                        5 51)


add_test(cdTuKullbackLeiblerSupervizedDistanceImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerSupervizedDistanceImageFilterNew)

add_test(cdTvKullbackLeiblerSupervizedDistanceImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${NOTOL}
             ${BASELINE}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
                         ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
         otbKullbackLeiblerSupervizedDistanceImageFilter
             ${INPUTDATA}/ERSBefore.png
             ${INPUTDATA}/ERSAfter.png
             ${INPUTDATA}/ROI_ERS.png
             35
             ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png)

add_test(cdTuMultivariateAlterationDetectorImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbMultivariateAlterationDetectorImageFilterNew)

add_test(cdTvMultivariateAlterationDetectorImageFilterSameNbBands ${CHANGEDETECTION_TESTS2}
         --compare-image 0.025
             ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
             ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
         otbMultivariateAlterationDetectorImageFilter
             ${INPUTDATA}/Spot5-Gloucester-before.tif
             ${INPUTDATA}/Spot5-Gloucester-after.tif
             ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif)

add_test(cdTvMultivariateAlterationDetectorImageFilterDiffNbBands ${CHANGEDETECTION_TESTS2}
         --compare-image 0.025
             ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif
             ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif
         otbMultivariateAlterationDetectorImageFilter
             ${INPUTDATA}/Spot5-ERS-Gloucester-before.tif
             ${INPUTDATA}/Spot5-Gloucester-after.tif
             ${TEMP}/cdTvMultivariateAlterationDetectorImageFilterOutputDiffNbBands.tif)


# -------       Fichiers sources CXX -----------------------------------
set(BasicChangeDetection_SRCS1
otbChangeDetectionTests1.cxx
otbCBAMIChangeDetectionTest.cxx
otbCorrelChangeDetectionTest.cxx
otbMeanDiffChangeDetectionTest.cxx
otbMeanRatioChangeDetectionTest.cxx
otbLHMIChangeDetectionTest.cxx
otbJHMIChangeDetectionTest.cxx
)

# -------       Fichiers sources CXX -----------------------------------
set(BasicChangeDetection_SRCS2
otbChangeDetectionTests2.cxx
otbKullbackLeiblerDistanceImageFilterNew.cxx
otbKullbackLeiblerDistanceImageFilter.cxx
otbKullbackLeiblerProfileImageFilterNew.cxx
otbKullbackLeiblerProfileImageFilter.cxx
otbKullbackLeiblerSupervizedDistanceImageFilterNew.cxx
otbKullbackLeiblerSupervizedDistanceImageFilter.cxx
otbMultivariateAlterationDetectorImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbChangeDetectionTests1 "${BasicChangeDetection_SRCS1}" "OTBChangeDetection;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbChangeDetectionTests2 "${BasicChangeDetection_SRCS2}" "OTBChangeDetection;OTBIO;OTBTesting")

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
