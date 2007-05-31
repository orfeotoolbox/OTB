
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.0000000001)

SET(CHANGEDETECTION_TESTS ${CXX_TEST_PATH}/otbChangeDetectionTests)

# -------            otb::CBAMI   ------------------------------

ADD_TEST(cdTvCBAMI ${CHANGEDETECTION_TESTS}  
  --compare-image ${TOL}   ${BASELINE}/cdCBAMIImage.png
                    ${TEMP}/cdCBAMIImage.png
        otbCBAMIChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdCBAMIImage.png 
	 )
	 
ADD_TEST(cdTvCorrel ${CHANGEDETECTION_TESTS}
  --compare-image ${TOL}   ${BASELINE}/cdCorrelImage.png
                    ${TEMP}/cdCorrelImage.png
	otbCorrelChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png 
	 2 
	 ${TEMP}/cdCorrelImage.png 
	 )

ADD_TEST(cdTvMeanDiff ${CHANGEDETECTION_TESTS}
  --compare-image ${EPSILON} 
		  ${BASELINE}/cdMeanDiffImage.hdr
		  ${TEMP}/cdMeanDiffImage.hdr
	otbMeanDiffChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdMeanDiffImage.hdr 
	 )

ADD_TEST(cdTvMeanRatio ${CHANGEDETECTION_TESTS}
  --compare-image ${TOL}   ${BASELINE}/cdMeanRatioImage.png
                    ${TEMP}/cdMeanRatioImage.png
	otbMeanRatioChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdMeanRatioImage.png 
	 )
	 
ADD_TEST(cdTvLHMI ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/cdLHMIImage.png
#                    ${TEMP}/cdLHMIImage.png
	otbLHMIChangeDetectionTest 
	 ${INPUTDATA}/GomaAvantSousEch.png 
	 ${INPUTDATA}/GomaApresSousEch.png
	 2 
	 ${TEMP}/cdLHMIImage.png 
	 )

ADD_TEST(cdTvJHMI ${CHANGEDETECTION_TESTS}
  --compare-image ${TOL}   
		  ${BASELINE}/cdJHMIImage.png
                  ${TEMP}/cdJHMIImage.png
	otbJHMIChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdJHMIImage.png 
	 )

ADD_TEST(cdTuKullbackLeiblerDistanceImageFilterNew ${CHANGEDETECTION_TESTS}
         otbKullbackLeiblerDistanceImageFilterNew)

ADD_TEST(cdTvKullbackLeiblerDistanceImageFilter ${CHANGEDETECTION_TESTS}
         --compare-image ${EPSILON}
			 ${BASELINE}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
			 ${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
	 otbKullbackLeiblerDistanceImageFilter
			${INPUTDATA}/GomaAvant.png 
			${INPUTDATA}/GomaApres.png
			${TEMP}/cdTVKullbackLeiblerDistanceImageFilterOutput.hdr
			35)


# -------       Fichiers sources CXX -----------------------------------
SET(BasicChangeDetection_SRCS
otbCBAMIChangeDetectionTest.cxx
otbCorrelChangeDetectionTest.cxx
otbMeanDiffChangeDetectionTest.cxx
otbMeanRatioChangeDetectionTest.cxx
otbLHMIChangeDetectionTest.cxx
otbJHMIChangeDetectionTest.cxx
otbKullbackLeiblerDistanceImageFilterNew.cxx
otbKullbackLeiblerDistanceImageFilter.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbChangeDetectionTests otbChangeDetectionTests.cxx ${BasicChangeDetection_SRCS})
TARGET_LINK_LIBRARIES(otbChangeDetectionTests OTBChangeDetection OTBIO OTBCommon gdal ITKIO ITKCommon ITKStatistics)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
