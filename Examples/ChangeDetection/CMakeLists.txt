PROJECT(ChangeDetectionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx )
TARGET_LINK_LIBRARIES(ChangeDetectionFrameworkExample OTBCommon gdal OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(DiffChDet DiffChDet.cxx)
TARGET_LINK_LIBRARIES(DiffChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(RatioChDet RatioChDet.cxx)
TARGET_LINK_LIBRARIES(RatioChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(CorrelChDet CorrelChDet.cxx)
TARGET_LINK_LIBRARIES(CorrelChDet OTBIO OTBCommon  gdal ITKCommon ITKIO)

ADD_EXECUTABLE(LHMIChDet LHMIChDet.cxx)
TARGET_LINK_LIBRARIES(LHMIChDet OTBIO OTBCommon gdal ITKCommon ITKIO ITKStatistics)

ADD_EXECUTABLE(JHMIChDet JHMIChDet.cxx)
TARGET_LINK_LIBRARIES(JHMIChDet OTBIO OTBCommon gdal ITKCommon ITKIO ITKStatistics)


IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING)

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/ChangeDetection)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbChangeDetectionExamplesTests)

SET(TOL 0.0)


# ------- CorrelChDetTest----------

ADD_TEST(CorrelChDetTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/CorrChDet.tif
	${TEMP}/CorrChDet.tif
	CorrelChDetTest
	${INPUTDATA}/ERSBefore.png
	${INPUTDATA}/ERSAfter.png
	${TEMP}/CorrChDet.tif
    15
)

# ------- DiffChDetTest----------

ADD_TEST(DiffChDetTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/DiffChDet.tif
	${TEMP}/DiffChDet.tif
	DiffChDetTest
	${INPUTDATA}/SpotBefore.png
	${INPUTDATA}/SpotAfter.png
	${TEMP}/DiffChDet.tif
    3
)

# ------- RatioChDetTest----------

ADD_TEST(RatioChDetTest ${EXE_TESTS}
	--compare-n-images ${TOL} 1
	${BASELINE}/RatioChDet.tif
	${TEMP}/RatioChDet.tif
	RatioChDetTest
	${INPUTDATA}/GomaAvant.png
	${INPUTDATA}/GomaApres.png
	${TEMP}/RatioChDet.tif
    3
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbChangeDetectionExamplesTests otbChangeDetectionExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbChangeDetectionExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
