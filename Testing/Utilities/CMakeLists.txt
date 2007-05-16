 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)


SET(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)



ADD_TEST(utOssimIntegrationTest ${UTILITIES_TESTS}
ossimIntegrationTest
)

ADD_TEST(utIjBSplineScatteredDataPointSetToImageFilterTest ${UTILITIES_TESTS}
--compare-image ${TOL}
		${BASELINE}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		ijBSplineScatteredDataPointSetToImageFilterTest
		${INPUTDATA}/brain.png
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
)


# -------       Fichiers sources CXX -----------------------------------
SET(UtilitiesTests_SRCS
ossimIntegrationTest.cxx
ijBSplineScatteredDataPointSetToImageFilterTest.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
TARGET_LINK_LIBRARIES(otbUtilitiesTests OTBIO OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon ossim)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )
