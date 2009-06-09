PROJECT(OBIAExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ImageToLabelToImage ImageToLabelToImage.cxx )
TARGET_LINK_LIBRARIES(ImageToLabelToImage OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})

ADD_EXECUTABLE(ShapeAttributeComputation ShapeAttributeComputation.cxx )
TARGET_LINK_LIBRARIES(ShapeAttributeComputation OTBCommon OTBIO ${OTB_IO_UTILITIES_DEPENDENT_LIBRARIES})


IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/OBIA)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbOBIAExamplesTests)

SET(TOL 0.0)

ADD_TEST( trTeOBIAImageToLabelToImageTest ${EXE_TESTS}
        --compare-image ${TOL}          ${BASELINE}/OBIAI2L2IOutput.tif
                                        ${TEMP}/OBIAI2L2IOutput.tif
        OBIAImageToLabelToImageTest
        ${INPUTDATA}/MSLabeledOutput.tif
        ${TEMP}/OBIAI2L2IOutput.tif
	${TEMP}/OBIAI2L2IInPretty.tif
	${TEMP}/OBIAI2L2IOutPretty.tif
	1 1108 0
        )

ADD_TEST(trTeOBIAShapeAttributeComputation ${EXE_TESTS}
	--compare-ascii ${EPS}
	${BASELINE}/OBIAShapeAttribute.txt
	${TEMP}/OBIAShapeAttribute.txt
	ShapeAttributeComputationTest
	${INPUTDATA}/MSLabeledOutput.tif
	${TEMP}/OBIAShapeAttribute.txt
)
      
INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbOBIAExamplesTests otbOBIAExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbOBIAExamplesTests  ITKAlgorithms ITKStatistics ITKNumerics OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
