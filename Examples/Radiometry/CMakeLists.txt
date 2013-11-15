project(ImageRadiometryExamples)
include_regular_expression("^.*$")

add_executable(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx )
target_link_libraries(ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )

add_executable(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx )
target_link_libraries(AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )


add_executable(NDVIRAndNIRVegetationIndexImageFilter NDVIRAndNIRVegetationIndexImageFilter.cxx )
target_link_libraries(NDVIRAndNIRVegetationIndexImageFilter OTBRadiometry OTBCommon OTBIO )

add_executable(AtmosphericCorrectionSequencement AtmosphericCorrectionSequencement.cxx )
target_link_libraries(AtmosphericCorrectionSequencement OTBRadiometry OTBCommon OTBIO )

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)

set(EXE_TESTS ${CXX_TEST_PATH}/otbRadiometryExamplesTests)

#Tolerance sur diff pixel image
set(TOL 0.0)
set(EPSILON 0.000000000001)


# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

add_test(raTeARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
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

add_test(raTeAVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
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

add_test(raTeNDVIRAndNIRVegetationIndexImageFilterTest ${EXE_TESTS}
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



add_test(raTeAtmosphericCorrectionSequencementTest ${EXE_TESTS}
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


include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbRadiometryExamplesTests otbRadiometryExamplesTests.cxx)
target_link_libraries(otbRadiometryExamplesTests OTBBasicFilters OTBCommon OTBDisparityMap OTBRadiometry OTBIO OTBSpatialReasoning OTBChangeDetection OTBFeatureExtraction  OTBLearning  OTBMultiScale OTBFusion    ${ITK_LIBRARIES}   OTBTesting)

endif()
