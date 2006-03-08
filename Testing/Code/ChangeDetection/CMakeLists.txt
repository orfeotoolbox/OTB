
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(TOL 0.0)

SET(CHANGEDETECTION_TESTS ${CXX_TEST_PATH}/otbChangeDetectionTests)

# -------            otb::CBAMI   ------------------------------

ADD_TEST(coTvCBAMI ${CHANGEDETECTION_TESTS}  
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
        otbCBAMIChangeDetectionTest
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdCBAMIImage.png 
	 )
	 
ADD_TEST(cdTvCorrel ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbCorrelChangeDetectionTest 
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdCorrelImage.png 
	 )

ADD_TEST(cdTvMeanDiff ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbMeanDiffChangeDetectionTest 
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdMeanDiffImage.png 
	 )

ADD_TEST(cdTvMeanRatio ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbMeanRatioChangeDetectionTest 
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdMeanRatioImage.png 
	 )
	 
ADD_TEST(cdTvLHMI ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbLHMIChangeDetectionTest 
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdLHMIImage.png 
	 )

ADD_TEST(cdTvJHMI ${CHANGEDETECTION_TESTS}
#  --compare-image ${TOL}   ${BASELINE}/coExtractROI_cthead1_26_97_209_100.png
#                    ${TEMP}/coExtractROI_cthead1_26_97_209_100.png
	otbJHMIChangeDetectionTest 
	 ${INPUTDATA}/cthead1.png 
	 ${INPUTDATA}/cthead1.png 
	 2 
	 ${TEMP}/CdJHMIImage.png 
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

# ${TIFF_LIBRARY}

ADD_EXECUTABLE(otbChangeDetectionTests otbChangeDetectionTests.cxx ${BasicChangeDetection_SRCS})
TARGET_LINK_LIBRARIES(otbChangeDetectionTests OTBChangeDetection OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon ITKStatistics)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
