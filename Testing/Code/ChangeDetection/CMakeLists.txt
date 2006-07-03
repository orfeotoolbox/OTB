
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_DIR}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_DIR}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_DIR}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_DIR}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(CHANGEDETECTION_TESTS ${CXX_TEST_PATH}/otbChangeDetectionTests)

# -------            otb::CBAMI   ------------------------------

ADD_TEST(cdTvCBAMI ${CHANGEDETECTION_TESTS}  
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbCBAMIChangeDetectionTest
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdCBAMIImage.png 
	 )
	 
ADD_TEST(cdTvCorrel ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbCorrelChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png 
	 2 
	 ${TEMP}/cdCorrelImage.png 
	 )

ADD_TEST(cdTvMeanDiff ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbMeanDiffChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdMeanDiffImage.png 
	 )

ADD_TEST(cdTvMeanRatio ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbMeanRatioChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdMeanRatioImage.png 
	 )
	 
ADD_TEST(cdTvLHMI ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbLHMIChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdLHMIImage.png 
	 )

ADD_TEST(cdTvJHMI ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbJHMIChangeDetectionTest 
	 ${INPUTDATA}/GomaAvant.png 
	 ${INPUTDATA}/GomaApres.png
	 2 
	 ${TEMP}/cdJHMIImage.png 
	 )



# -------       Fichiers sources CXX -----------------------------------
SET(BasicChangeDetection_SRCS
otbCBAMIChangeDetectionTest.cxx
otbCorrelChangeDetectionTest.cxx
otbMeanDiffChangeDetectionTest.cxx
otbMeanRatioChangeDetectionTest.cxx
otbLHMIChangeDetectionTest.cxx
otbJHMIChangeDetectionTest.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbChangeDetectionTests otbChangeDetectionTests.cxx ${BasicChangeDetection_SRCS})
TARGET_LINK_LIBRARIES(otbChangeDetectionTests OTBChangeDetection OTBIO OTBCommon gdal ITKIO ITKCommon ITKStatistics)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
