set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Projections)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

otb_add_test(NAME prTeOrthoRectificationExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/panOrthoToul.tif
    ${TEMP}/panOrthoToul.tif
  Execute $<TARGET_FILE:OrthoRectificationExample>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
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

otb_add_test(NAME prTeOrthoRectificationExampleXSTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/xsOrthoToul.tif
    ${TEMP}/xsOrthoToul.tif
  Execute $<TARGET_FILE:OrthoRectificationExample>
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
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

otb_add_test(NAME prTeMapProjectionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_7}
    ${BASELINE}/mapProjectionExample.txt
    ${TEMP}/mapProjectionExample.txt
  Execute $<TARGET_FILE:MapProjectionExample>
    ${TEMP}/mapProjectionExample.txt
)

otb_add_test(NAME prTeVectorDataProjectionExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${NOTOL}
    ${BASELINE}/vectorDataProjectionExample.shp
    ${TEMP}/vectorDataProjectionExample.shp
  Execute $<TARGET_FILE:VectorDataProjectionExample>
    ${OTB_DATA_ROOT}/Input/Capitole-Shadows.kml
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    ${TEMP}/vectorDataProjectionExample.shp
)

otb_add_test(NAME prTeGeometriesProjectionTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ogr ${NOTOL}
    ${BASELINE}/geometriesProjectionExample.shp
    ${TEMP}/geometriesProjectionExample.shp
  Execute $<TARGET_FILE:GeometriesProjectionExample>
    ${OTB_DATA_ROOT}/Input/Capitole-Shadows.shp
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    ${TEMP}/geometriesProjectionExample.shp
)

#TODO change the example by using a SPOT4 roamnia
otb_add_test(NAME prTeEstimateRPCSensorModelExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${EPSILON_4}
    ${BASELINE}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
    ${TEMP}/otbGCPsToRPCSensorModelImageFilterWithoutDEMOutput.txt
  --ignore-lines-with 6 PipelineMTime ImportImageContaine Source: Image Time: Pointer:
  Execute $<TARGET_FILE:EstimateRPCSensorModelExample>
    LARGEINPUT{SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT}
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
    72 2955 -23.012153 -43.541331 0.
    2884 1898 -22.954691 -42.786188 0.
    2539 47 -22.616041 -42.777877 0.
    577 183 -22.551654 -43.271813 0.
)

otb_add_test(NAME prTePlaceNameToLonLatExampleTest COMMAND ${OTB_TEST_DRIVER}
  Execute $<TARGET_FILE:PlaceNameToLonLatExample>
    Toulouse
)
otb_add_test(NAME prTeCoordinateToNameExampleTest COMMAND ${OTB_TEST_DRIVER}
  --compare-ascii ${NOTOL}
    ${BASELINE}/CoordinateToNameExample.txt
    ${TEMP}/CoordinateToNameExample.txt
  Execute $<TARGET_FILE:CoordinateToNameExample>
    103.78 1.29
    ${TEMP}/CoordinateToNameExample.txt
)
