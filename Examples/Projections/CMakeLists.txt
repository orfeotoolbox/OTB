project(ProjectionsExamples)
include_regular_expression("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(
        SensorModelExample.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
endif()


add_executable(SensorModelExample SensorModelExample.cxx )
target_link_libraries(SensorModelExample OTBProjections OTBCommon OTBIO)

add_executable(MapProjectionExample MapProjectionExample.cxx )
target_link_libraries(MapProjectionExample OTBProjections OTBCommon OTBIO)

add_executable(OrthoRectificationExample OrthoRectificationExample.cxx )
target_link_libraries(OrthoRectificationExample OTBProjections OTBCommon OTBIO)

if( OTB_USE_CURL )
add_executable(PlaceNameToLonLatExample PlaceNameToLonLatExample.cxx )
target_link_libraries(PlaceNameToLonLatExample OTBProjections OTBCommon OTBIO )

add_executable(CoordinateToNameExample CoordinateToNameExample.cxx )
target_link_libraries(CoordinateToNameExample OTBProjections OTBCommon OTBIO )

endif()

add_executable(VectorDataProjectionExample VectorDataProjectionExample.cxx )
target_link_libraries(VectorDataProjectionExample OTBProjections OTBCommon OTBIO)

add_executable(GeometriesProjectionExample GeometriesProjectionExample.cxx )
target_link_libraries(GeometriesProjectionExample OTBProjections OTBOGRAdapters)

add_executable(VectorDataExtractROIExample VectorDataExtractROIExample.cxx )
target_link_libraries(VectorDataExtractROIExample OTBProjections OTBCommon OTBIO)

add_executable(EstimateRPCSensorModelExample EstimateRPCSensorModelExample.cxx )
target_link_libraries(EstimateRPCSensorModelExample OTBProjections OTBCommon OTBIO)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(INPUTLARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbProjectionsExamplesTests)
set(EXE_TESTS2 ${CXX_TEST_PATH}/otbProjectionsExamplesTests2)

set(TOL 0.0)
set(EPSILON_4 0.0001)

if(OTB_DATA_USE_LARGEINPUT)
add_test(prTeOrthoRectificationExampleTest ${EXE_TESTS}
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

add_test(prTeOrthoRectificationExampleXSTest ${EXE_TESTS}
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
endif()

add_test(prTeMapProjectionExampleTest ${EXE_TESTS}
        --compare-ascii ${TOL}
        ${BASELINE}/mapProjectionExample.txt
        ${TEMP}/mapProjectionExample.txt
    MapProjectionExampleTest
            ${TEMP}/mapProjectionExample.txt
)

if(OTB_DATA_USE_LARGEINPUT)
add_test(prTeVectorDataProjectionExampleTest ${EXE_TESTS}
        --compare-ogr ${TOL}
        ${BASELINE}/vectorDataProjectionExample.shp
        ${TEMP}/vectorDataProjectionExample.shp
    VectorDataProjectionExampleTest
        ${INPUTDATA}/Capitole-Shadows.kml
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
            ${TEMP}/vectorDataProjectionExample.shp
)
endif()

if(OTB_DATA_USE_LARGEINPUT)
add_test(prTeGeometriesProjectionTest ${EXE_TESTS}
        --compare-ogr ${TOL}
        ${BASELINE}/geometriesProjectionExample.shp
        ${TEMP}/geometriesProjectionExample.shp
    GeometriesProjectionTest
        ${INPUTDATA}/Capitole-Shadows.shp
        ${INPUTLARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
            ${TEMP}/geometriesProjectionExample.shp
)
endif()

if(OTB_DATA_USE_LARGEINPUT)
#TODO change the example by using a SPOT4 roamnia
add_test(prTeEstimateRPCSensorModelExampleTest ${EXE_TESTS}
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

endif()

if( OTB_USE_CURL )
add_test(prTePlaceNameToLonLatExampleTest ${EXE_TESTS2}
    PlaceNameToLonLatExampleTest
        Toulouse
)
add_test(prTeCoordinateToNameExampleTest ${EXE_TESTS2}
         --compare-ascii ${TOL}
         ${BASELINE}/CoordinateToNameExample.txt
         ${TEMP}/CoordinateToNameExample.txt
    CoordinateToNameExampleTest
         103.78 1.29
         ${TEMP}/CoordinateToNameExample.txt
)
endif()

include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbProjectionsExamplesTests otbProjectionsExamplesTests.cxx)
target_link_libraries(otbProjectionsExamplesTests OTBCommon OTBIO OTBProjections OTBTesting)

if( OTB_USE_CURL )
  add_executable(otbProjectionsExamplesTests2 otbProjectionsExamplesTests2.cxx)
  target_link_libraries(otbProjectionsExamplesTests2 OTBCommon OTBIO OTBProjections OTBTesting)
endif()



endif()
