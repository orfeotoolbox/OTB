set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry)
set(INPUTDATA ${OTB_DATA_ROOT}/Examples)

# -------            ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

otb_add_test(NAME raTeARVIMultiChannelRAndBAndNIRVegetationIndexImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
    ${TEMP}/ARVIMultiChannelRAndBAndNIRVegetationIndex.tif
  Execute $<TARGET_FILE:ARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter>
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

otb_add_test(NAME raTeAVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  #--compare-image ${NOTOL}
  #  ${BASELINE}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
  #  ${TEMP}/AVIMultiChannelRAndGAndNIRVegetationIndexImageFilterTest.tif
  Execute $<TARGET_FILE:AVIMultiChannelRAndGAndNIRVegetationIndexImageFilter>
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

otb_add_test(NAME raTeNDVIRAndNIRVegetationIndexImageFilterTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${NOTOL}
    ${BASELINE}/NDVIRAndNIRVegetationIndex.tif
    ${TEMP}/NDVIRAndNIRVegetationIndex.tif
  Execute $<TARGET_FILE:NDVIRAndNIRVegetationIndexImageFilter>
    ${INPUTDATA}/poupees_sub_c1.png
    ${INPUTDATA}/poupees_sub_c2.png
    ${TEMP}/NDVIRAndNIRVegetationIndex.tif
    ${TEMP}/NDVIRAndNIRVegetationIndex2.tif
    ${TEMP}/NDVIRAndNIRVegetationIndex3.tif
    ${TEMP}/NDVIRAndNIRVegetationIndex4.tif
)

otb_add_test(NAME raTeAtmosphericCorrectionSequencementTest COMMAND ${OTB_TEST_DRIVER}
  --compare-image ${EPSILON_8}
    ${BASELINE}/Example_RomaniaAtmosphericCorrectionSequencement.tif
    ${TEMP}/Example_RomaniaAtmosphericCorrectionSequencement.tif
  Execute $<TARGET_FILE:AtmosphericCorrectionSequencement>
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
