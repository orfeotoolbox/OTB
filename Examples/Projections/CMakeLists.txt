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
TARGET_LINK_LIBRARIES(SensorModelExample OTBProjections OTBCommon OTBIO)

ADD_EXECUTABLE(MapProjectionExample MapProjectionExample.cxx )
TARGET_LINK_LIBRARIES(MapProjectionExample OTBProjections OTBCommon OTBIO)

ADD_EXECUTABLE(OrthoRectificationExample OrthoRectificationExample.cxx )
TARGET_LINK_LIBRARIES(OrthoRectificationExample OTBProjections OTBCommon OTBIO)

IF( OTB_USE_CURL )
ADD_EXECUTABLE(PlaceNameToLonLatExample PlaceNameToLonLatExample.cxx )
TARGET_LINK_LIBRARIES(PlaceNameToLonLatExample OTBProjections OTBCommon OTBIO )

ADD_EXECUTABLE(CoordinateToNameExample CoordinateToNameExample.cxx )
TARGET_LINK_LIBRARIES(CoordinateToNameExample OTBProjections OTBCommon OTBIO )

ENDIF( OTB_USE_CURL )

ADD_EXECUTABLE(VectorDataProjectionExample VectorDataProjectionExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataProjectionExample OTBProjections OTBCommon OTBIO)

ADD_EXECUTABLE(GeometriesProjection GeometriesProjection.cxx )
TARGET_LINK_LIBRARIES(GeometriesProjection OTBProjections OTBOGRAdapters)

ADD_EXECUTABLE(VectorDataExtractROIExample VectorDataExtractROIExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataExtractROIExample OTBProjections OTBCommon OTBIO)

ADD_EXECUTABLE(EstimateRPCSensorModelExample EstimateRPCSensorModelExample.cxx )
TARGET_LINK_LIBRARIES(EstimateRPCSensorModelExample OTBProjections OTBCommon OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbProjectionsExamplesTests)
SET(EXE_TESTS2 ${CXX_TEST_PATH}/otbProjectionsExamplesTests2)

SET(TOL 0.0)
SET(EPSILON_4 0.0001)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTeOrthoRectificationExampleTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/panOrthoToul.tif
        ${TEMP}/panOrthoToul.tif
    OrthoRectificationExampleTest
         ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
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

ADD_TEST(prTeOrthoRectificationExampleXSTest ${EXE_TESTS}
        --compare-image ${TOL}
        ${BASELINE}/xsOrthoToul.tif
        ${TEMP}/xsOrthoToul.tif
    OrthoRectificationExampleTest
         ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
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

ADD_TEST(prTeMapProjectionExampleTest ${EXE_TESTS}
        --compare-ascii ${TOL}
        ${BASELINE}/mapProjectionExample.txt
        ${TEMP}/mapProjectionExample.txt
    MapProjectionExampleTest
            ${TEMP}/mapProjectionExample.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTeVectorDataProjectionExampleTest ${EXE_TESTS}
        --compare-ogr ${TOL}
        ${BASELINE}/vectorDataProjectionExample.shp
        ${TEMP}/vectorDataProjectionExample.shp
    VectorDataProjectionExampleTest
        ${INPUTDATA}/Capitole-Shadows.kml
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
            ${TEMP}/vectorDataProjectionExample.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(prTeGeometriesProjectionTest ${EXE_TESTS}
        --compare-ogr ${TOL}
        ${BASELINE}/vectorDataProjectionExample.shp
        ${TEMP}/geometriesProjectionExample.shp
    GeometriesProjectionTest
        ${INPUTDATA}/Capitole-Shadows.kml
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
            ${TEMP}/geometriesProjectionExample.shp
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

IF(OTB_DATA_USE_LARGEINPUT)
#TODO change the example by using a SPOT4 roamnia
ADD_TEST(prTeEstimateRPCSensorModelExampleTest ${EXE_TESTS}
         --compare-ascii ${EPSILON_4}
         ${BASELINE}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
         ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
         --ignore-lines-with 5 PipelineMTime ImportImageContaine Source: Image Time:
    EstimateRPCSensorModelExample
         ${INPUTLARGEDATA}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT
         ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
         1199 1259 -22.76847 -43.168043 0.
         1497 727  -22.688931 -43.066686 0.
         699 1146 -22.7254 -43.288651 0.
         1379 2457 -22.986073 -43.193502 0.
         2592 1846 -22.932684 -42.854215 0.
         1430 1797 -22.872988 -43.136813 0.
         587 2654 -22.983847  -43.393297 0.
         2398 2422 -23.025664 -42.928778 0.
         1436 2946 -23.074483 -43.192431 0.
         963 2915  -23.047382 -43.310011 0.
         1081 1024 -22.721919 -43.18523  0.
         1395 1213 -22.769191 -43.116752 0.
         1781 2202 -22.959556 -43.069911 0.
         2803 1942 -22.958868 -42.808235 0.
         980 2734 -22.99534 -43.309672 0.
         713 2754 -23.007862 -43.365307 0.
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

IF( OTB_USE_CURL )
ADD_TEST(prTePlaceNameToLonLatExampleTest ${EXE_TESTS2}
    PlaceNameToLonLatExampleTest
        Toulouse
)
ADD_TEST(prTeCoordinateToNameExampleTest ${EXE_TESTS2}
         --compare-ascii ${TOL}
         ${BASELINE}/CoordinateToNameExample.txt
         ${TEMP}/CoordinateToNameExample.txt
    CoordinateToNameExampleTest
         103.78 1.29
         ${TEMP}/CoordinateToNameExample.txt
)
ENDIF( OTB_USE_CURL )

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests OTBCommon OTBIO OTBProjections OTBTesting)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(otbProjectionsExamplesTests2 otbProjectionsExamplesTests2.cxx)
  TARGET_LINK_LIBRARIES(otbProjectionsExamplesTests2 OTBCommon OTBIO OTBProjections OTBTesting)
ENDIF( OTB_USE_CURL )



ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
