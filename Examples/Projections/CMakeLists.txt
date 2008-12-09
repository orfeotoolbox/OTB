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

IF( OTB_USE_CURL )
ADD_EXECUTABLE(PlaceNameToLonLat PlaceNameToLonLat.cxx )
TARGET_LINK_LIBRARIES(PlaceNameToLonLat OTBProjections OTBCommon OTBIO ITKCommon ITKIO ${CURL_LIBRARY} tinyXML)
ENDIF( OTB_USE_CURL )

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbProjectionsExamplesTests)

SET(TOL 0.0)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(OrthoRectificationExampleTest ${EXE_TESTS}
	--compare-image ${TOL}
	${BASELINE}/panOrthoToul.tif
	${TEMP}/panOrthoToul.tif
    OrthoRectificationExampleTest  
	 ${INPUTLARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF 
	${TEMP}/panOrthoToul.tif 
	31
	N
	375000 
	4828100 
	500 
	500 
	0.6 
	-0.6
	
)

ADD_TEST(OrthoRectificationExampleXSTest ${EXE_TESTS}
	--compare-image ${TOL}
	${BASELINE}/xsOrthoToul.tif
	${TEMP}/xsOrthoToul.tif
    OrthoRectificationExampleTest  
	 ${INPUTLARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
	${TEMP}/xsOrthoToul.tif 
	31
	N
	375000 
	4828100 
	500 
	500 
	0.6 
	-0.6
	
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_EXECUTABLE(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests gdal ITKIO ITKAlgorithms ITKStatistics ITKNumerics ITKCommon OTBBasicFilters OTBCommon OTBDisparityMap OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion OTBProjections)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
