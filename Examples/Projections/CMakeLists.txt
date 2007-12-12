PROJECT(ProjectionsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
        SensorModelExample.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


SET(PROJECTIONS_EXAMPLES ${CXX_TEST_PATH}/otbProjectionsExamplesTests)

ADD_EXECUTABLE(SensorModelExample SensorModelExample.cxx )
TARGET_LINK_LIBRARIES(SensorModelExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(OrthoRectificationExample OrthoRectificationExample.cxx )
TARGET_LINK_LIBRARIES(OrthoRectificationExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)


IF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)

SET(INPUTDATA ${OTB_SOURCE_DIR}/Examples/Data)
SET(INPUTLARGEDATA ${OTB_DATA_ROOT}/LargeInput)
SET(TEMP ${OTB_BINARY_DIR}/Examples/Projections/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbProjectionsExamplesTests)

SET(TOL 0.0)

ADD_TEST(OrthoRectificationExampleTest ${EXE_TESTS}
	--compare-image ${TOL}
	${BASELINE}/panOrthoToul.tif
	${TEMP}/panOrthoToul.tif
    OrthoRectificationExampleTest  
	 ${INPUTLARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 
	${TEMP}/panOrthoToul.tif 
	375000 
	4828100 
	500 
	500 
	0.6 
	-0.6
	
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBProjections)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND NOT OTB_DISABLE_CXX_EXAMPLES_TESTING )
