
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON_10 0.0000000001)

SET(CHANGEDETECTION_TESTS1 ${CXX_TEST_PATH}/otbChangeDetectionTests1)
SET(CHANGEDETECTION_TESTS2 ${CXX_TEST_PATH}/otbChangeDetectionTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANGEDETECTION_TESTS1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::CBAMI   ------------------------------

ADD_TEST(cdTvCBAMI ${CHANGEDETECTION_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/cdCBAMIImage.png
                    ${TEMP}/cdCBAMIImage.png
        otbCBAMIChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png
	 ${INPUTDATA}/GomaApres.png
	 2
	 ${TEMP}/cdCBAMIImage.png
	 )

ADD_TEST(cdTvCorrel ${CHANGEDETECTION_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/cdCorrelImage.png
                    ${TEMP}/cdCorrelImage.png
	otbCorrelChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png
	 ${INPUTDATA}/GomaApres.png
	 2
	 ${TEMP}/cdCorrelImage.png
	 )

ADD_TEST(cdTvMeanDiff ${CHANGEDETECTION_TESTS1}
  --compare-image ${EPSILON_10}
		  ${BASELINE}/cdMeanDiffImage.hdr
		  ${TEMP}/cdMeanDiffImage.hdr
	otbMeanDiffChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png
	 ${INPUTDATA}/GomaApres.png
	 2
	 ${TEMP}/cdMeanDiffImage.hdr
	 )

ADD_TEST(cdTvMeanRatio ${CHANGEDETECTION_TESTS1}
  --compare-image ${TOL}   ${BASELINE}/cdMeanRatioImage.png
                    ${TEMP}/cdMeanRatioImage.png
	otbMeanRatioChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png
	 ${INPUTDATA}/GomaApres.png
	 2
	 ${TEMP}/cdMeanRatioImage.png
	 )

ADD_TEST(cdTvLHMI ${CHANGEDETECTION_TESTS1}
#  --compare-image ${TOL}   ${BASELINE}/cdLHMIImage.png
#                    ${TEMP}/cdLHMIImage.png
	otbLHMIChangeDetectionTest
	 ${INPUTDATA}/GomaAvantSousEch.png
	 ${INPUTDATA}/GomaApresSousEch.png
	 2
	 ${TEMP}/cdLHMIImage.png
	 )

ADD_TEST(cdTvJHMI ${CHANGEDETECTION_TESTS1}
  --compare-image ${TOL}
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

ADD_TEST(cdTuKullbackLeiblerDistanceImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerDistanceImageFilterNew)

ADD_TEST(cdTvKullbackLeiblerDistanceImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${EPSILON_10}
			 ${BASELINE}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
			 ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
	 otbKullbackLeiblerDistanceImageFilter
			${INPUTDATA}/GomaAvant.png
			${INPUTDATA}/GomaApres.png
			${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
			35)

ADD_TEST(cdTuKullbackLeiblerProfileImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerProfileImageFilterNew)

ADD_TEST(cdTlKullbackLeiblerProfileImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${EPSILON_10}
			 ${BASELINE}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
			 ${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
	 otbKullbackLeiblerProfileImageFilter
			${INPUTDATA}/GomaAvant.png
			${INPUTDATA}/GomaApres.png
			${TEMP}/cdTVKullbackLeiblerProfileImageFilterOutput.hdr
			5 51)


ADD_TEST(cdTuKullbackLeiblerSupervizedDistanceImageFilterNew ${CHANGEDETECTION_TESTS2}
         otbKullbackLeiblerSupervizedDistanceImageFilterNew)

ADD_TEST(cdTlKullbackLeiblerSupervizedDistanceImageFilter ${CHANGEDETECTION_TESTS2}
         --compare-image ${TOL}
             ${BASELINE}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
			 ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png
         otbKullbackLeiblerSupervizedDistanceImageFilter
             ${INPUTDATA}/ERSBefore.png
             ${INPUTDATA}/ERSAfter.png
             ${INPUTDATA}/ROI_ERS.png
             35
             ${TEMP}/cdTVKullbackLeiblerSupervizedDistanceImageFilterOutput.png)


# -------       Fichiers sources CXX -----------------------------------
SET(BasicChangeDetection_SRCS1 
otbChangeDetectionTests1.cxx 
otbCBAMIChangeDetectionTest.cxx
otbCorrelChangeDetectionTest.cxx
otbMeanDiffChangeDetectionTest.cxx
otbMeanRatioChangeDetectionTest.cxx
otbLHMIChangeDetectionTest.cxx
otbJHMIChangeDetectionTest.cxx
)

# -------       Fichiers sources CXX -----------------------------------
SET(BasicChangeDetection_SRCS2
otbChangeDetectionTests2.cxx 
otbKullbackLeiblerDistanceImageFilterNew.cxx
otbKullbackLeiblerDistanceImageFilter.cxx
otbKullbackLeiblerProfileImageFilterNew.cxx
otbKullbackLeiblerProfileImageFilter.cxx
otbKullbackLeiblerSupervizedDistanceImageFilterNew.cxx
otbKullbackLeiblerSupervizedDistanceImageFilter.cxx
)

OTB_ADD_EXECUTABLE(otbChangeDetectionTests1 "${BasicChangeDetection_SRCS1}" "OTBChangeDetection;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbChangeDetectionTests2 "${BasicChangeDetection_SRCS2}" "OTBChangeDetection;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
