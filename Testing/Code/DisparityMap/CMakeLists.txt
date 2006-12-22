 
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)


SET(DISPARITYMAP_TESTS ${CXX_TEST_PATH}/otbDisparityMapTests)


# -------            otb::DisparityMapEstimationMethod   ----------

ADD_TEST(dmTuDisparityMapEstimationMethodNew ${DISPARITYMAP_TESTS} 
	 otbDisparityMapEstimationMethodNew)

ADD_TEST(dmTvDisparityMapEstimationMethod ${DISPARITYMAP_TESTS} 
   --compare-ascii ${TOL}
 		   ${BASELINE_FILES}/dmDisparityMapEstimationOutput1.txt
 		   ${TEMP}/dmDisparityMapEstimationOutput1.txt
	 otbDisparityMapEstimationMethod
	           ${INPUTDATA}/fixed.png
	           ${INPUTDATA}/moving.png
	           ${INPUTDATA}/pointSet.png
	           ${TEMP}/dmDisparityMapEstimationOutput1.txt
		   20 20
)


# -------       Fichiers sources CXX -----------------------------------
SET(BasicDisparityMap_SRCS
otbDisparityMapEstimationMethodNew.cxx
otbDisparityMapEstimationMethod.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbDisparityMapTests otbDisparityMapTests.cxx ${BasicDisparityMap_SRCS})
TARGET_LINK_LIBRARIES(otbDisparityMapTests OTBIO OTBDisparityMap gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )
