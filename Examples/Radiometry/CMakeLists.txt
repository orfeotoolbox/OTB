PROJECT(ImageRadiometryExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )

ADD_EXECUTABLE(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )


ADD_EXECUTABLE(NDVIRAndNIRVegetationIndexImageFilter NDVIRAndNIRVegetationIndexImageFilter.cxx )
TARGET_LINK_LIBRARIES(NDVIRAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )

ADD_EXECUTABLE(AtmosphericCorrectionSequencement AtmosphericCorrectionSequencement.cxx )
TARGET_LINK_LIBRARIES(AtmosphericCorrectionSequencement OTBRadiometry OTBCommon OTBIO )

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

SET(EXE_TESTS ${CXX_TEST_PATH}/otbRadiometryExamplesTests)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.000000000001)


# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTeARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
  --compare-image ${EPSILON}
                    ${BASELINE}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
                  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
       ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest
                  ${INPUTDATA}/poupees_sub.png
                  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
              ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex3.tif
                  ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex2.tif
                   1
                   2
                   3
                   0.6 # Gamma parameter
         )



# -------            AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest   ------------------------------

ADD_TEST(raTeAVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
  #--compare-image ${TOL}
              #${BASELINE}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
              #${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
    AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest
    ${INPUTDATA}/qb_toulouse_sub.tif
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest2.png
    ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest3.png
    3
    2
    4 # indices of the channels
    660.
    560.
    830. # lambdaR, lambdaG, lambdaNir
    )



# -------            NDVIRAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTeNDVIRAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
  --compare-image ${TOL}
                    ${BASELINE}/NDVIRAndNIRVegetationIndex.tif
                  ${TEMP}/NDVIRAndNIRVegetationIndex.tif
       NDVIRAndNIRVegetationIndexImageFilterTest
         ${INPUTDATA}/poupees_sub_c1.png
        ${INPUTDATA}/poupees_sub_c2.png
         ${TEMP}/NDVIRAndNIRVegetationIndex.tif
         ${TEMP}/NDVIRAndNIRVegetationIndex2.tif
         ${TEMP}/NDVIRAndNIRVegetationIndex3.tif
        ${TEMP}/NDVIRAndNIRVegetationIndex4.tif
         )



ADD_TEST(raTeAtmosphericCorrectionSequencementTest ${EXE_TESTS}
        --compare-image ${EPSILON}
                ${BASELINE}/Example_RomaniaAtmosphericCorrectionSequencement.tif
                ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
        AtmosphericCorrectionSequencementTest
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
        ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
        ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
        ${INPUTDATA}/atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt

# ImageToLuminance parameters
# LuminanceToReflectance parameters
        27.3    #solar azimuthal elevation
        4       #day
        12      #mounth
# AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters
        152.7
        2.5
        -77.0
        1013.
        2.48134
        0.34400
        1
        0.199854
# ReflectanceToSurfaceReflectance parameters
# SurfaceAdjacencyEffect6SCorrectionSchemeFilter parameters
        2       # Radius;
        0.020   # pixel spacing in kilometers
        )


INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbRadiometryExamplesTests otbRadiometryExamplesTests.cxx)
TARGET_LINK_LIBRARIES(otbRadiometryExamplesTests OTBBasicFilters OTBCommon OTBDisparityMap OTBRadiometry OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion    ${ITK_LIBRARIES}   OTBTesting)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
