PROJECT(ProjectionsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
        SensorModelExample.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_EXECUTABLE(SensorModelExample SensorModelExample.cxx )
TARGET_LINK_LIBRARIES(SensorModelExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(MapProjectionExample MapProjectionExample.cxx )
TARGET_LINK_LIBRARIES(MapProjectionExample OTBProjections OTBCommon OTBIO)

ADD_EXECUTABLE(OrthoRectificationExample OrthoRectificationExample.cxx )
TARGET_LINK_LIBRARIES(OrthoRectificationExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

IF( OTB_USE_CURL )
ADD_EXECUTABLE(PlaceNameToLonLatExample PlaceNameToLonLatExample.cxx )
TARGET_LINK_LIBRARIES(PlaceNameToLonLatExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO ${CURL_LIBRARY} tinyXML)
ENDIF( OTB_USE_CURL )

ADD_EXECUTABLE(VectorDataProjectionExample VectorDataProjectionExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataProjectionExample OTBProjections OTBCommon OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
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

ADD_TEST(MapProjectionExampleTest ${EXE_TESTS}
	--compare-ascii ${TOL}
	${BASELINE}/mapProjectionExample.txt
	${TEMP}/mapProjectionExample.txt
    MapProjectionExampleTest
    	${TEMP}/mapProjectionExample.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(VectorDataProjectionExampleTest ${EXE_TESTS}
	--compare-binary
	${BASELINE}/vectorDataProjectionExample.shp
	${TEMP}/vectorDataProjectionExample.shp
    VectorDataProjectionExampleTest
        ${INPUTDATA}/Capitole-Shadows.kml
        ${INPUTLARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
    	${TEMP}/vectorDataProjectionExample.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

IF( OTB_USE_CURL )
ADD_TEST(PlaceNameToLonLatExampleTest ${EXE_TESTS}
    PlaceNameToLonLatExampleTest
        Toulouse
)
ENDIF( OTB_USE_CURL )

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
  TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests OTBCommon OTBIO OTBProjections ${CURL_LIBRARY} tinyXML)
ELSE( OTB_USE_CURL )
  ADD_EXECUTABLE(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
  TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests OTBCommon OTBIO OTBProjections)
ENDIF( OTB_USE_CURL )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
