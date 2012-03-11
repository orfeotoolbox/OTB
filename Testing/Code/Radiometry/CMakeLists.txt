
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(RADIOMETRY_TESTS1 ${CXX_TEST_PATH}/otbRadiometryTests1)
SET(RADIOMETRY_TESTS2 ${CXX_TEST_PATH}/otbRadiometryTests2)
SET(RADIOMETRY_TESTS3 ${CXX_TEST_PATH}/otbRadiometryTests3)
SET(RADIOMETRY_TESTS4 ${CXX_TEST_PATH}/otbRadiometryTests4)
SET(RADIOMETRY_TESTS5 ${CXX_TEST_PATH}/otbRadiometryTests5)
SET(RADIOMETRY_TESTS6 ${CXX_TEST_PATH}/otbRadiometryTests6)
SET(RADIOMETRY_TESTS7 ${CXX_TEST_PATH}/otbRadiometryTests7)
SET(RADIOMETRY_TESTS8 ${CXX_TEST_PATH}/otbRadiometryTests8)
SET(RADIOMETRY_TESTS9 ${CXX_TEST_PATH}/otbRadiometryTests9)
SET(RADIOMETRY_TESTS10 ${CXX_TEST_PATH}/otbRadiometryTests10)
SET(SPECTRUM_DB ${OTB_DATA_ROOT}/Input/BD-CNES)


ADD_TEST(raTvLAIFromNDVIFormosat2_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS1}
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
        otbMultiChannelRAndNIRVegetationIndexImageFilter
        LAIFormosat2
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raMultiChannelRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
        1
        4
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::RAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS2}
        otbRAndBAndNIRVegetationIndexImageFilterNew )


ADD_TEST(raTvARVI_RAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
                    ${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
        otbRAndBAndNIRVegetationIndexImageFilter
        ARVI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_b.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
        0.6 # Gamma parameter
        )

# -------            otb::MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTuMultiChannelRAndBAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS2}
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
                    ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
        otbMultiChannelRAndBAndNIRVegetationIndexImageFilter
        ARVI
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
        3 1 4
    0.6 # Gamma parameter
        )

# -------            otb::ImageToLuminanceImageFilter   ------------------------------
ADD_TEST(raTuImageToLuminanceImageFilterNew ${RADIOMETRY_TESTS2}
        otbImageToLuminanceImageFilterNew
)

ADD_TEST(raTvImageToLuminanceImageFilter ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilter.tif
                          ${TEMP}/raTvImageToLuminanceImageFilter.tif
        otbImageToLuminanceImageFilter
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvImageToLuminanceImageFilter.tif
        10   #channel 1 alpha
        20   #channel 2 alpha
        30   #channel 3 alpha
        40   #channel 4 alpha
        1    #channel 1 beta
        2    #channel 2 beta
        3    #channel 3 beta
        4    #channel 4 beta
         )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(raTvImageToLuminanceImageFilterAutoSpot5 ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoSpot5.tif
                          ${TEMP}/raTvImageToLuminanceImageFilterAutoSpot5.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoSpot5.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoIkonos ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoIkonos.tif
                          ${TEMP}/raTvImageToLuminanceImageFilterAutoIkonos.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
        ${TEMP}/raTvImageToLuminanceImageFilterAutoIkonos.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoWV2PAN ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoWV2MULTI ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoQuickbirdPAN ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoQuickbirdXS ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoQuickbirdXS ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
         )

ADD_TEST(raTvImageToLuminanceImageFilterAutoFORMOSAT ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoFormosat.tif
                       ${TEMP}/raTvImageToLuminanceImageFilterAutoFormosat.tif
        otbImageToLuminanceImageFilterAuto
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/raTvImageToLuminanceImageFilterAutoFormosat.tif
         )

ENDIF(OTB_DATA_USE_LARGEINPUT)




# -------            otb::LuminanceToReflectanceImageFilter   ------------------------------
ADD_TEST(raTuLuminanceToReflectanceImageFilterNew ${RADIOMETRY_TESTS2}
        otbLuminanceToReflectanceImageFilterNew
)

ADD_TEST(raTvLuminanceToReflectanceImageFilter ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterDsol.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterDsol.tif
        otbLuminanceToReflectanceImageFilter
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvLuminanceToReflectanceImageFilterDsol.tif
        0.2 #radius
        10  #channel 1 illumination
        20  #channel 2 illumination
        30  #channel 3 illumination
        40  #channel 4 illumination
        0.9923885328 #d/d0 corresponding to the date 03/05
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterDayMonth ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilter.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterDayMounth.tif
        otbLuminanceToReflectanceImageFilter
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvLuminanceToReflectanceImageFilterDayMounth.tif
        0.2 #radius
        10  #channel 1 illumination
        20  #channel 2 illumination
        30  #channel 3 illumination
        40  #channel 4 illumination
        3   #day
        5   #mounth
        )

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoSpot5 ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoIkonos ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
                          ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoWV2PAN ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
                       ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoWV2MULTI ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
                       ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
                       ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
                       ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
         )

ADD_TEST(raTvLuminanceToReflectanceImageFilterAutoFORMOSAT ${RADIOMETRY_TESTS2}
   --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
                       ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
        otbLuminanceToReflectanceImageFilterAuto
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
         )

ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::ImageToReflectanceImageFilter   ------------------------------
ADD_TEST(raTuImageToReflectanceImageFilterNew ${RADIOMETRY_TESTS2}
        otbImageToReflectanceImageFilterNew
)

ADD_TEST(raTvImageToReflectanceImageFilter ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToReflectanceImageFilterDsol.tif
                          ${TEMP}/raTvImageToReflectanceImageFilterDsol.tif
        otbImageToReflectanceImageFilter
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvImageToReflectanceImageFilterDsol.tif
        0.2 #radius
        1   #channel 1 alpha
        2   #channel 2 alpha
        3   #channel 3 alpha
        4   #channel 4 alpha
        10  #channel 1 beta
        11  #channel 2 beta
        12  #channel 3 beta
        13  #channel 4 beta
        10  #channel 1 illumination
        20  #channel 2 illumination
        30  #channel 3 illumination
        40  #channel 4 illumination
        0.9923885328 #d/d0 corresponding to the date 03/05
         )

ADD_TEST(raTvRomaniaImageToReflectance ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}
                  ${BASELINE}/raTvRomania_Reflectance.tif
                  ${TEMP}/raTvRomaniaReflectanceImage.tif
        otbImageToReflectanceImageFilter
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raTvRomaniaReflectanceImage.tif
        27.3                 # = 90-62.70 : elevation et azimuth solaire
        1.981247824          #channel 1 alpha = 0.881338*2.24800
        4.332207085          #channel 2 alpha = 0.858713*5.04500
        2.32064768           #channel 3 alpha = 0.685568*3.38500
        9.3177861            #channel 4 alpha = 6.19122*1.50500
        0                    #channel 1 beta
        0                    #channel 2 beta
        0                    #channel 3 beta
        0                    #channel 4 beta
        1057.56              #channel 1 illumination
        1570.23              #channel 2 illumination
        1842.94              #channel 3 illumination
        232.820              #channel 4 illumination
        0.9889145564708814   #= sqrt(0.977952) d/d0 corresponding to the date 03/05
        )

ADD_TEST(raTvImageToReflectanceImageFilterDayMounth ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToReflectanceImageFilter.tif
                          ${TEMP}/raTvImageToReflectanceImageFilterDayMounth.tif
        otbImageToReflectanceImageFilter
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvImageToReflectanceImageFilterDayMounth.tif
        0.2 #radius
        1   #channel 1 alpha
        2   #channel 2 alpha
        3   #channel 3 alpha
        4   #channel 4 alpha
        10  #channel 1 beta
        11  #channel 2 beta
        12  #channel 3 beta
        13  #channel 4 beta
        10  #channel 1 illumination
        20  #channel 2 illumination
        30  #channel 3 illumination
        40  #channel 4 illumination
        3   #day
        5   #mounth
       )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(raTvImageToReflectanceImageFilterAuto ${RADIOMETRY_TESTS2}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToReflectanceImageFilterAuto.tif
                          ${TEMP}/raTvImageToReflectanceImageFilterAuto.tif
        otbImageToReflectanceImageFilterAuto
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/raTvImageToReflectanceImageFilterAuto.tif
         )

ADD_TEST(raTuImageToReflectanceImageFilterAutoFORMOSAT2 ${RADIOMETRY_TESTS2}
        otbImageToReflectanceImageFilterAuto
                ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
                ${TEMP}/raTvImageToReflectanceImageFilterAutoFORMOSAT2.tif
         )

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::DEMCaracteristicsExtractor   ------------------------------

ADD_TEST(raTuDEMCaracteristicsExtractorNew ${RADIOMETRY_TESTS3}
        otbDEMCaracteristicsExtractorNew
)


ADD_TEST(raTvDEMCaracteristicsExtractor ${RADIOMETRY_TESTS3}
  --compare-n-images ${EPSILON_12} 4
                     ${BASELINE}/raTvDEMCaracteristicsExtractorSlop.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
                     ${BASELINE}/raTvDEMCaracteristicsExtractorAspect.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif
                     ${BASELINE}/raTvDEMCaracteristicsExtractorIncidence.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif
                     ${BASELINE}/raTvDEMCaracteristicsExtractorExitance.tif
                     ${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif
        otbDEMCaracteristicsExtractor
        ${INPUTDATA}/pyramide.tif
        20 # view azimut
        40 # view angle
        10 # solar azimut
        30 # solar angle
        ${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
        ${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif
        ${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif
        ${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif
        )


# -------            otb::AtmosphericCorrectionParameters   ------------------------------
# launch the FilterFunctionValues unitary test too.
ADD_TEST(raTuAtmosphericCorrectionParametersNew ${RADIOMETRY_TESTS3}
        otbAtmosphericCorrectionParametersNew
)

##########           otb::FilterFunctionValues -------------------
# in otbAtmosphericCorrectionParameters.h/.cxx
ADD_TEST(raTuFilterFunctionValuesTest ${RADIOMETRY_TESTS3}
        otbFilterFunctionValuesTest
        ${TEMP}/raTuFilterFunctionValuesTest.txt
        0.5             # lambda min
        0.7             # lambda max
        0.05            # user step
        0.2 0.5 0.3 0.9 0.8 # vector
)

##########           otb::FilterFunctionValuesSpectralResponse -------------------
# test the FilterFunctionValuesFilter created by Spectral Responde class
ADD_TEST(raTvFilterFunctionValuesSpectralResponseTest ${RADIOMETRY_TESTS3}
    --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvFilterFunctionValuesSpectralResponseTest.txt
        ${TEMP}/raTvFilterFunctionValuesSpectralResponseTest.txt
        otbFilterFunctionValuesSpectralResponseTest
        ${SPECTRUM_DB}/JHU/becknic/manmade/txt/0092uuu.txt
        ${TEMP}/raTvFilterFunctionValuesSpectralResponseTest.txt
        0.0025
)


##########           otb::FilterFunctionValues DigitalGlobe sensitivity files -------------------
#ADD_TEST(raTvFilterFunctionValuesDigitalGlobeQBTest ${RADIOMETRY_TESTS3}
#    --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvFilterFunctionValuesDigitalGlobeQBTest.txt
#                           ${TEMP}/raTvFilterFunctionValuesDigitalGlobeQBTest.txt
#        otbFilterFunctionValuesDigitalGlobeTest
#    ${INPUTDATA}/Radiometry/DigitalGlobe/radiance_response_QB.txt
#    ${TEMP}/raTvFilterFunctionValuesDigitalGlobeQBTest.txt
#    0.005            # user step
#    1                # user wavelength unit factor (default in micro meter)
#)
#
#ADD_TEST(raTvFilterFunctionValuesDigitalGlobeWV1Test ${RADIOMETRY_TESTS3}
#    --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvFilterFunctionValuesDigitalGlobeWV1Test.txt
#                           ${TEMP}/raTvFilterFunctionValuesDigitalGlobeWV1Test.txt
#        otbFilterFunctionValuesDigitalGlobeTest
#    ${INPUTDATA}/Radiometry/DigitalGlobe/radiance_response_WV1.txt
#    ${TEMP}/raTvFilterFunctionValuesDigitalGlobeWV1Test.txt
#    0.005            # user step
#    1e-3             # user wavelength unit factor (default in micro meter)
#)
#
#ADD_TEST(raTvFilterFunctionValuesDigitalGlobeWV2Test ${RADIOMETRY_TESTS3}
#    --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvFilterFunctionValuesDigitalGlobeWV2Test.txt
#                           ${TEMP}/raTvFilterFunctionValuesDigitalGlobeWV2Test.txt
#        otbFilterFunctionValuesDigitalGlobeTest
#    ${INPUTDATA}/Radiometry/DigitalGlobe/radiance_response_WV2.txt
#    ${TEMP}/raTvFilterFunctionValuesDigitalGlobeWV2Test.txt
#    0.005            # user step
#    1e-3             # user wavelength unit factor (default in micro meter)
#)

##########           otb::SIXSTraits -------------------
ADD_TEST(raTuSIXSTraitsTest ${RADIOMETRY_TESTS3}
    otbSIXSTraitsTest)

ADD_TEST(raTvSIXSTraitsComputeAtmosphericParametersTest ${RADIOMETRY_TESTS3}
    --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
                           ${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
        otbSIXSTraitsComputeAtmosphericParametersTest
        ${INPUTDATA}/in6S_otb
        ${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
)



# -------            otb::AtmosphericRadiativeTerms   ------------------------------
ADD_TEST(raTuAtmosphericRadiativeTermsNew ${RADIOMETRY_TESTS3}
        otbAtmosphericRadiativeTermsNew
)

ADD_TEST(raTuAtmosphericRadiativeTermsSingleChannelNew ${RADIOMETRY_TESTS3}
        otbAtmosphericRadiativeTermsSingleChannelNew
)

ADD_TEST(raTvAtmosphericRadiativeTermsTest ${RADIOMETRY_TESTS3}
         --compare-ascii ${NOTOL}  ${BASELINE_FILES}/raTvAtmosphericRadiativeTermsTest.txt
                                   ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
        otbAtmosphericRadiativeTermsTest
                                   ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
)

# -------            otb::ReflectanceToSurfaceReflectanceImageFilter   ------------------------------
ADD_TEST(raTuReflectanceToSurfaceReflectanceImageFilterNew ${RADIOMETRY_TESTS3}
        otbReflectanceToSurfaceReflectanceImageFilterNew
)
ADD_TEST(raTvReflectanceToSurfaceReflectanceImageFilter ${RADIOMETRY_TESTS3}
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
                              ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
        otbReflectanceToSurfaceReflectanceImageFilterTest
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
        # SAME VALUE FOR EACH CHANNEL
        1 1 1 1 # intrinsic atmospheric reflectance
        2 2 2 2 # spherical albedo of the atmosphere
        3 3 3 3 # total transmission
        2 2 2 2 # downward transmittance
        3 3 3 3 # upward transmittance
       )

IF(OTB_DATA_USE_LARGEINPUT)
#this test was comment after the refactoring of astmospheric correction classes
#//TODO Need to rewrite tests with baselines generated directly with 6S.
#ADD_TEST(raTvReflectanceToSurfaceReflectanceImageFilter2 ${RADIOMETRY_TESTS3}
#  --compare-ascii ${EPSILON_12}  ${BASELINE_FILES}/raTvReflectanceToSurfaceReflectanceImageFilter2.txt
#                              ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter2.txt
#        otbReflectanceToSurfaceReflectanceImageFilterTest2
#    ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
#    ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter2.txt
#       )
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter ${RADIOMETRY_TESTS3}
  --compare-image ${EPSILON_12}
                  ${BASELINE}/raTvRomania_Correction.tif
                  ${TEMP}/raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.tif
        otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter
                ${BASELINE}/raTvRomania_Reflectance.tif
                ${TEMP}/raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.tif
                ${INPUTDATA}/romania_parameter.txt                           # atmo param;
                ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B3.txt   # wavelengths, channel 3
                ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B2.txt   # wavelengths, channel 2
                ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B1.txt   # wavelengths, channel 1
                ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_MIR.txt  # wavelengths, channel 4
      )

# -------            otb::SurfaceAdjacencyEffect6SCorrectionSchemeFilter   ------------------------------
ADD_TEST(raTuSurfaceAdjacencyEffect6SCorrectionSchemeFilterNew ${RADIOMETRY_TESTS3}
        otbSurfaceAdjacencyEffect6SCorrectionSchemeFilterNew
)

ADD_TEST(raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter ${RADIOMETRY_TESTS3}
 --compare-image ${EPSILON_12}  ${BASELINE}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
                             ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
# --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
#                           ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
        otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter
        ${BASELINE}/raTvRomania_Correction.tif
        ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
        2                                                            # Radius;
        0.020                                                        # pixel spacing in kilometers
        ${INPUTDATA}/romania_parameter.txt                           # atmo param;
        ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B3.txt   # wavelengths, channel 3
        ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B2.txt   # wavelengths, channel 2
        ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B1.txt   # wavelengths, channel 1
        ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_MIR.txt  # wavelengths, channel 4
        # FOR plateform verification only : to remove when OK
        ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# -------            otb::AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms   ------------------------------
ADD_TEST(raTuAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew ${RADIOMETRY_TESTS4}
        otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew)

ADD_TEST(raTvAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms ${RADIOMETRY_TESTS4}
--compare-ascii ${EPSILON_4}
                ${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
                ${TEMP}/raTvCorrectionTo6SRadiative.txt
        otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
                ${INPUTDATA}/in6S_otb
                ${TEMP}/raTvCorrectionTo6SRadiative.txt
)

ADD_TEST(raTvAtmosphericCorrectionSequencementTest ${RADIOMETRY_TESTS4}
        --compare-image ${EPSILON_10}
                ${BASELINE}/raTvRomaniaAtmosphericCorrectionSequencement.tif
                ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
        otbAtmosphericCorrectionSequencementTest
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
# ImageToLuminance parameters
        ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
# LuminanceToReflectance parameters
        27.3    #elevation et azimuth solaire
        4       #day
        12      #month
        ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
# AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters
        152.7
        2.5
        -77.0
        1013.
        2.48134
        0.34400
        1
        0.199854
        ${INPUTDATA}/atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt
# ReflectanceToSurfaceReflectance parameters
# SurfaceAdjacencyEffect6SCorrectionSchemeFilter parameters
        2       # Radius;
        0.020   # pixel spacing in kilometers
        )


# -------            EVI RAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvEVI_RAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
        otbEVIRAndBAndNIRVegetationIndexImageFilter
        EVI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_b.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
        2.5
        6.0
        7.5
        1.0
)

# -------            EVI MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvEVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
        otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
        EVI
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
        3   # red
        1   # blue
        4   # nir
        10.0    # gain factor
        30.0    # coefficient of the aerosol resistance term
        7.5     # coefficient of the aerosol resistance term
        400.0   # canopy  background adjustment
)

# -------            TSARVI RAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvTSARVI_RAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
        otbTSARVIRAndBAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_b.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
        0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
        0.9   # b ( ordonnee a l'origine de la droite des sols nus dans l'espace RB/PIR )
        0.08  # x coeff a priori constant
        0.5   # gamma
)

# -------            TSARVI MultiChannelRAndBAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvTSARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS4}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
                   ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
        otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
        3   # red
        1   # blue
        4   # nir
        0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
        0.9   # b ( ordonnee a l'origine de la droite des sols nus dans l'espace RB/PIR )
        0.08  # x coeff a priori constant
        0.5   # gamma
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTuRAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS5}
        otbRAndNIRVegetationIndexImageFilterNew )

ADD_TEST(raTvNDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        NDVI
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
    )
ADD_TEST(raTvRVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        RVI
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
    )

ADD_TEST(raTvSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        SAVI
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
    )

ADD_TEST(raTvMSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        MSAVI
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
    )

ADD_TEST(raTvMSAVI2_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        MSAVI2
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
    )

# -------            IPVI RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvIPVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
                   ${TEMP}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        IPVI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
)


# -------            TNDVI RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvTNDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
                   ${TEMP}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        TNDVI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
)


# -------            GEMI RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvGEMI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
                   ${TEMP}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        GEMI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
)

ADD_TEST(raTvTSAVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        TSAVI
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
)

# -------            LAIFromNDVIFormosat2 RAndNIRVegetationIndexImageFilter   ------------------------------

ADD_TEST(raTvLAIFromNDVIFormosat2_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS5}
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
        otbRAndNIRVegetationIndexImageFilter
        LAIFormosat2
    ${INPUTDATA}/verySmallFSATSW_r.tif
    ${INPUTDATA}/verySmallFSATSW_nir.tif
    ${TEMP}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
    )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTuRAndGAndNIRVegetationIndexImageFilterNew ${RADIOMETRY_TESTS6}
        otbRAndGAndNIRVegetationIndexImageFilterNew )

# -------            otb::MultiChannelRAndGAndNIRIndexImageFilter   ------------------------------
ADD_TEST(raTuMultiChannelRAndGAndNIRIndexImageFilterNew ${RADIOMETRY_TESTS6}
        otbMultiChannelRAndGAndNIRIndexImageFilterNew )

ADD_TEST(raTvPVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
        otbSetASetBRAndNIRVegetationIndexImageFilter
        PVI
        0.7
        0.9
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
)

ADD_TEST(raTvPVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
                    ${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
        otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter
        PVI
        0.7
        0.9
        ${INPUTDATA}/verySmallFSATSW.tif
        ${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
        3 4
)



# -------            AVI RAndGAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvAVI_RAndGAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
                   ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
        otbAVIRAndGAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_g.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
        660.  # lambda r
        560.  # lambda g
        830.  # lambda nir
)

# -------            AVI MultiChannelRAndGAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvAVI_MultiChannelRAndGAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
 --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
                   ${TEMP}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
        otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
        3   # red
        2   # green
        4   # nir
        660.  # lambda r
        560.  # lambda g
        830.  # lambda nir
)

# -------            WDVI RAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvWDVI_RAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
                   ${TEMP}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
        otbWDVIRAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
        2.0   # g : slope of soil line
)

# -------            WDVI MultiChannelRAndNIRVegetationIndexImageFilter   ------------------------------
ADD_TEST(raTvWDVI_MultiChannelRAndNIRVegetationIndexImageFilter ${RADIOMETRY_TESTS6}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
                   ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
        otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
        3   # red
        4   # nir
        2.0   # g : slope of soil line
)

# -------            LAI NDVI Logarithmic   ------------------------------
ADD_TEST(raTvLAIFromNDVILogarithmicFunctorTest ${RADIOMETRY_TESTS6}
        otbLAIFromNDVILogarithmic
        3   # red
        4   # nir
        0.12   # ndvi soil
        0.91   # ndvi infinity
        0.70   # extinction coefficient
)

# -------            LAI Reflectances Linear   ------------------------------
ADD_TEST(raTvLAIFromReflectancesLinearFunctorTest ${RADIOMETRY_TESTS6}
        otbLAIFromReflectancesLinear
        3   # red
        4   # nir
        -18   # red coef
        13   # nir coef
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::GAndRIndexImageFilter   ------------------------------
ADD_TEST(raTuGAndRIndexImageFilterNew ${RADIOMETRY_TESTS7}
        otbGAndRIndexImageFilterNew )

# -------            otb::MultiChannelGAndRIndexImageFilter   ------------------------------
ADD_TEST(raTuMultiChannelGAndRIndexImageFilterNew ${RADIOMETRY_TESTS7}
        otbMultiChannelGAndRIndexImageFilterNew )

# -------            IR GAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIR_GAndRIndexImageFilter ${RADIOMETRY_TESTS7}
 --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
                   ${TEMP}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
        otbGAndRIndexImageFilter
        IR
        ${INPUTDATA}/verySmallFSATSW_g.tif
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${TEMP}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
)

# -------            IR MultiChannelGAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIR_MultiChannelGAndRIndexImageFilter ${RADIOMETRY_TESTS7}
   --compare-image ${EPSILON_10}   ${BASELINE}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
                     ${TEMP}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
        otbMultiChannelGAndRIndexImageFilter
        IR
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
        1 2
)

# -------            IC GAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIC_GAndRIndexImageFilter ${RADIOMETRY_TESTS7}
 --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
                   ${TEMP}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
        otbGAndRIndexImageFilter
        IC
        ${INPUTDATA}/verySmallFSATSW_g.tif
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${TEMP}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
)

# -------            IC MultiChannelGAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIC_MultiChannelGAndRIndexImageFilter ${RADIOMETRY_TESTS7}
   --compare-image ${NOTOL}   ${BASELINE}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
                     ${TEMP}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
        otbMultiChannelGAndRIndexImageFilter
        IC
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
        1 2
)

# -------            IB GAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIB_GAndRIndexImageFilter ${RADIOMETRY_TESTS7}
 --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
                   ${TEMP}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
        otbGAndRIndexImageFilter
        IB
        ${INPUTDATA}/verySmallFSATSW_g.tif
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${TEMP}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
)

# -------            IB MultiChannelGAndRIndexImageFilter   ------------------------------
ADD_TEST(raTvIB_MultiChannelGAndRIndexImageFilter ${RADIOMETRY_TESTS7}
   --compare-image ${EPSILON_10}   ${BASELINE}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
                     ${TEMP}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
        otbMultiChannelGAndRIndexImageFilter
        IB
        ${INPUTDATA}/Romania_Extract.tif
        ${TEMP}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
        1 2
)

# -------            IB2 GAndRAndNIRIndexImageFilter   ------------------------------
ADD_TEST(raTvIB2_GAndRAndNIRIndexImageFilter ${RADIOMETRY_TESTS7}
 --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
                   ${TEMP}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
        otbIBGAndRAndNIRIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_g.tif
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
)

# -------            IB2 MultiChannelGAndRAndNIRIndexImageFilter   ------------------------------
ADD_TEST(raTvIB2_MultiChannelGAndRIndexImageFilter ${RADIOMETRY_TESTS7}
   --compare-image ${EPSILON_6}   ${BASELINE}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
                     ${TEMP}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
         otbIBMultiChannelGAndRAndNIRIndexImageFilter
         ${INPUTDATA}/Romania_Extract.tif
         ${TEMP}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
         2 1 3
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# # -------            NDBI BuiltUp TM4AndTM5IndexImageFilter   ------------------------------
ADD_TEST(raTvNDBI_TM4AndTM5BuiltUpIndexImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
                   ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
        otbNDBITM4AndTM5IndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
)

# -------            NDBI BuiltUp MultiChannelTM4AndTM5IndexImageFilter   ------------------------------
ADD_TEST(raTvNDBI_MultiChannelTM4AndTM5BuiltUpIndexImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
                   ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
        otbNDBIMultiChannelTM4AndTM5IndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
        3   # red
        4   # nir
)

# -------            ISU BuiltUp RAndNIRIndexImageFilterr   ------------------------------
ADD_TEST(raTvISU_RAndNIRBuiltUpIndexImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
                   ${TEMP}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
        otbISURAndNIRIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
        100.
        25.
)

# -------            ISU BuiltUp MultiChannelRAndNIRIndexImageFilter   ------------------------------
ADD_TEST(raTvISU_MultiChannelRAndNIRBuiltUpIndexImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
                   ${TEMP}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
        otbISUMultiChannelRAndNIRIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
        3 4 # red nir channels
        100.
        25.
)


# -------            NDWI MutliChannel WaterIndexFilter ------------------------------
ADD_TEST(raTvNDWI_MultiChannelWaterIndexImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
                   ${TEMP}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
        otbNDWIMultiChannelWaterIndexImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
        3 4 # mir nir channels
)


# -------            NDWI SingleChannel WaterIndexFilter ------------------------------
ADD_TEST(raTvNDWI_WaterIndexImageFilter ${RADIOMETRY_TESTS8}
--compare-image ${EPSILON_12}   ${BASELINE}/raWaterIndex_NDWI_verySmallFSATSW.tif
                  ${TEMP}/raWaterIndex_NDWI_verySmallFSATSW.tif
        otbNDWIWaterIndexImageFilter
        ${INPUTDATA}/verySmallFSATSW_r.tif
        ${INPUTDATA}/verySmallFSATSW_nir.tif
        ${TEMP}/raWaterIndex_NDWI_verySmallFSATSW.tif
)




# -------            WaterSqrtSpectralAngleImageFilter (Ossman)------------------------------
ADD_TEST(raTvWaterSqrtSpectralAngleImageFilter ${RADIOMETRY_TESTS8}
 --compare-image ${EPSILON_12}   ${BASELINE}/raWater_SqrtSpectralAngle.tif
                   ${TEMP}/raWater_SqrtSpectralAngle.tif
        otbWaterSqrtSpectralAngleImageFilter
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        ${TEMP}/raWater_SqrtSpectralAngle.tif
        0 1 2 3
)

# -------            Aeronet New------------------------------
ADD_TEST(raTuAeronetNew ${RADIOMETRY_TESTS8}
        otbAeronetNew
)

# -------            Aeronet Seysses------------------------------
ADD_TEST(raTvAeronetExtractData_BadDate ${RADIOMETRY_TESTS8}
        otbAeronetExtractDataBadData
        ${INPUTDATA}/AERONET/090101_091231_Seysses.lev15
        23    # Day
        2     # Month
        2009  # Year
        11    # Hour
        0     # Minute
        0.4   # Epsilon
)

# -------            Aeronet Seysses------------------------------
ADD_TEST(raTvAeronetExtractData_BadHour ${RADIOMETRY_TESTS8}
        otbAeronetExtractDataBadData
        ${INPUTDATA}/AERONET/090101_091231_Seysses.lev15
        16    # Day
        2     # Month
        2009  # Year
        10    # Hour
        0     # Minute
        0.4   # Epsilon
)

# -------            Aeronet Seysses Level 15------------------------------
ADD_TEST(raTvAeronet_SeyssesLevel15_ExtractData ${RADIOMETRY_TESTS8}
   --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raAeronetExtractData_SeyssesLevel15_23_03_2009_11_00_00_epsilon_0dot4.txt
                          ${TEMP}/raAeronetExtractData_SeyssesLevel15_23_03_2009_11_00_00_epsilon_0dot4.txt
        otbAeronetExtractData
        ${INPUTDATA}/AERONET/090101_091231_Seysses.lev15
        23    # Day
        3     # Month
        2009  # Year
        10    # Hour
        0     # Minute
        0.4   # Epsilon
        ${TEMP}/raAeronetExtractData_SeyssesLevel15_23_03_2009_11_00_00_epsilon_0dot4.txt
)

ADD_TEST(raTvAeronet_ToulouseLevel20_ExtractData ${RADIOMETRY_TESTS8}
   --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raAeronetExtractData_ToulouseLevel20_31_08_1999_16_30_epsilon_2.txt
                          ${TEMP}/raAeronetExtractData_ToulouseLevel20_31_08_1999_16_30_epsilon_2.txt
        otbAeronetExtractData
        ${INPUTDATA}/AERONET/990101_011231_Toulouse.lev20
        31    # Day
        8     # Month
        1999  # Year
        16    # Hour
        30     # Minute
        2   # Epsilon
        ${TEMP}/raAeronetExtractData_ToulouseLevel20_31_08_1999_16_30_epsilon_2.txt
)
ADD_TEST(raTvAeronet_ArcachonLevel10_ExtractData ${RADIOMETRY_TESTS8}
   --compare-ascii ${EPSILON_6} ${BASELINE_FILES}/raAeronetExtractData_ArcachonLevel10_16_08_2009_13_00_epsilon_1.txt
                          ${TEMP}/raAeronetExtractData_ArcachonLevel10_16_08_2009_13_00_epsilon_1.txt
        otbAeronetExtractData
        ${INPUTDATA}/AERONET/080101_091231_Arcachon.lev10
        16    # Day
        8     # Month
        2009  # Year
        13    # Hour
        0     # Minute
        1     # Epsilon
        ${TEMP}/raAeronetExtractData_ArcachonLevel10_16_08_2009_13_00_epsilon_1.txt
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            TerraSarBrightnessFunctor   ------------------------------
ADD_TEST(raTvTerraSarBrightnessFunctor ${RADIOMETRY_TESTS9}
 --compare-ascii ${EPSILON_12}
                               ${BASELINE_FILES}/raTvTerraSarBrightnessImageFunctor.txt
                               ${TEMP}/raTvTerraSarBrightnessImageFunctor.txt
        otbTerraSarBrightnessFunctor
                12 # real pixel
                12 # complex real part pixel
                0  # complex imaginary part pixel
                1  # calFactor
                ${TEMP}/raTvTerraSarBrightnessImageFunctor.txt
)

# -------            TerraSarBrightnessImageFilter   ------------------------------
ADD_TEST(raTuTerraSarBrightnessImageFilterNew ${RADIOMETRY_TESTS9}
       otbTerraSarBrightnessImageFilterNew
)

ADD_TEST(raTvTerraSarBrightnessImageFilterTest ${RADIOMETRY_TESTS9}
    --compare-image ${EPSILON_12}
                ${BASELINE}/raTvTerraSarBrightnessImageFilterTest.tif
                ${TEMP}/raTvTerraSarBrightnessImageFilterTest.tif
        otbTerraSarBrightnessImageFilterTest
        ${INPUTDATA}/ExtractIMAGE_HH_SRA_strip_012.tif
        ${TEMP}/raTvTerraSarBrightnessImageFilterTest.tif
        0 # don't use metadata
        0 # results in linear scale
)

ADD_TEST(raTvTerraSarBrightnessImageComplexFilterTest ${RADIOMETRY_TESTS9}
    --compare-image ${EPSILON_12}
                ${BASELINE}/raTvTerraSarBrightnessImageComplexFilterTest.tif
                ${TEMP}/raTvTerraSarBrightnessImageComplexFilterTest.tif
        otbTerraSarBrightnessImageComplexFilterTest
        ${INPUTDATA}/ExtractIMAGE_HH_SRA_spot_074.tif
        ${TEMP}/raTvTerraSarBrightnessImageComplexFilterTest.tif
        0 # don't use metadata
        0 # results in linear scale
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(raTvTerraSarBrightnessImageFilterUPSALA ${RADIOMETRY_TESTS9}
    --compare-image ${EPSILON_12}
                ${BASELINE}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
                ${TEMP}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
        otbTerraSarBrightnessImageFilterTest
        ${LARGEINPUT}/TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/IMAGEDATA/IMAGE_HH_SRA_strip_012.tif
        ${TEMP}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
        1 # use metadata
        0 # results in linear scale
        2000 2000 250 250 # Extract
)

ADD_TEST(raTvTerraSarBrightnessImageComplexFilterTORONTO ${RADIOMETRY_TESTS9}
    --compare-image ${EPSILON_12}
                ${BASELINE}/raTvTerraSarBrightnessComplexImageFilter_TORONTO.tif
                ${TEMP}/raTvTerraSarBrightnessImageComplexFilter_TORONTO.tif
        otbTerraSarBrightnessImageComplexFilterTest
        ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/raTvTerraSarBrightnessImageComplexFilter_TORONTO.tif
        1 # use metadata
        0 # results in linear scale
        2000 2000 250 250 # Extract
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            SarParametricMapFunction   ------------------------------
ADD_TEST(raTuSarParametricMapFunction ${RADIOMETRY_TESTS9}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/raTvSarParametricMapFunctionOutputAscii.txt
                    ${TEMP}/raTvSarParametricMapFunctionOutputAscii.txt
        otbSarParametricMapFunctionTest
                 ${INPUTDATA}/scene.png
                 ${TEMP}/raTvSarParametricMapFunctionOutputAscii.txt
)
ADD_TEST(raTvSarParametricMapFunctionToImageFilter  ${RADIOMETRY_TESTS9}
        --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarParametricMapFunctionToImageFilter.tif
                    ${TEMP}/raTvSarParametricMapFunctionToImageFilter.tif
        otbSarParametricMapFunctionToImageFilter
            ${TEMP}/raTvSarParametricMapFunctionToImageFilter.tif
            100 200 # image size
)


# -------            SarRadiometricCalibrationFunctor   ------------------------------
ADD_TEST(raTuSarRadiometricCalibrationFunctor ${RADIOMETRY_TESTS9}
        otbSarRadiometricCalibrationFunctor
)
ADD_TEST(raTuSarRadiometricCalibrationFunctorWithoutNoise ${RADIOMETRY_TESTS9}
        otbSarRadiometricCalibrationFunctorWithoutNoise
)
# -------            SarRadiometricCalibrationFunction   ------------------------------
ADD_TEST(raTvSarRadiometricCalibrationFunction ${RADIOMETRY_TESTS9}
        --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/raTvSarRadiometricCalibrationFunctionOutputAscii.txt
              ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithNoise.txt
        otbSarRadiometricCalibrationFunction
                   ${INPUTDATA}/scene.png
                   ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithNoise.txt
)
ADD_TEST(raTvSarRadiometricCalibrationFunctionWithoutNoise ${RADIOMETRY_TESTS9}
        --compare-ascii ${NOTOL}
                  ${BASELINE_FILES}/raTvSarRadiometricCalibrationFunctionOutputAscii.txt
              ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithoutNoise.txt
        otbSarRadiometricCalibrationFunctionWithoutNoise
                   ${INPUTDATA}/scene.png
                   ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithoutNoise.txt
)

# -------  SarRadiometricCalibrationToImageFilter   ------------------------------

IF(OTB_DATA_USE_LARGEINPUT)

# Read data with a complex IFreader (Input data type = CInt16) and done ExtractROIFilter
# after RadiometricCalibFilter.
  ADD_TEST(raTvSarRadiometricCalibrationToImageWithComplexPixelFilter_TSX_PANGKALANBUUN  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
                    ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
        otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
                1000 1000 250 250 # Extract
          )

# Read data with a complex IFreader (Input data type = CInt16) and done ExtractROIFilter
# after RadiometricCalibFilter without take into account Noise.
  ADD_TEST(raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_TSX_PANGKALANBUUN  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
                    ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
        otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
                1000 1000 250 250 # Extract
          )

# Read data with a scalar IFreader (Input data type = CInt16) and done ExtractROIFilter
# after RadiometricCalibFilter.
  ADD_TEST(raTvSarRadiometricCalibrationToImageWithRealPixelFilter_TSX_PANGKALANBUUN  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
                    ${TEMP}/raTvSarRadiometricCalibrationToImageFilterRealPixel_TSX_PANGKALANBUUN_HH.tif
        otbSarRadiometricCalibrationToImageFilterWithRealPixelTest
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarRadiometricCalibrationToImageFilterRealPixel_TSX_PANGKALANBUUN_HH.tif
                 1000 1000 250 250 # Extract
          )

# Read data with a complex IFreader (Input data type = CInt16) and done ExtractROIFilter
# before RadiometricCalibFilter.
  ADD_TEST(raTvSarRadiometricCalibrationToImagePixelFilterWithExtractROIBefore_TSX_PANGKALANBUUN  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
                    ${TEMP}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
        otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest
            ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
                1000 1000 250 250 # Extract
          )

# Read data with a complex IFreader (Input data type = CInt16) and compare the result of
# CReader->SarRadiometricCalibFilter with CReader->ModulusFilter->SarRadiometricCalibFilter
  ADD_TEST(raTvSarRadiometricCalibrationToImageCompareFilter_TSX_PANGKALANBUUN  ${RADIOMETRY_TESTS9}
        otbSarRadiometricCalibrationToImageFilterCompareTest
               ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
               1000 1000 250 250 # Extract
          )


# Read data with a complex IFreader (Input data type = CInt16)
  ADD_TEST(raTvSarRadiometricCalibrationToImageWithComplexPixelFilter_TSX_TORONTO ${RADIOMETRY_TESTS9}
        --compare-image ${EPSILON_12}
           ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
           ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
        otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest
           ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
           ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
                   2000 2000 250 250 # Extract
          )

# Read data with a scalar IFreader (Input data type = UInt16)
  ADD_TEST(raTvSarRadiometricCalibrationToImageWithRealPixelFilter_TSX_UPSALA ${RADIOMETRY_TESTS9}
        --compare-image ${EPSILON_12}
           ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
           ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
        otbSarRadiometricCalibrationToImageFilterWithRealPixelTest
           ${LARGEINPUT}/TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/IMAGEDATA/IMAGE_HH_SRA_strip_012.tif
           ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
               2000 2000 250 250 # Extract
          )

ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            SarBrightnessFunctor   ------------------------------
ADD_TEST(raTuSarBrightnessFunctor ${RADIOMETRY_TESTS9}
        otbSarBrightnessFunctor
)
ADD_TEST(raTuSarBrightnessFunctorWithoutNoise ${RADIOMETRY_TESTS9}
        otbSarBrightnessFunctorWithoutNoise
)

# -------            SarBrightnessFunction   ------------------------------
ADD_TEST(raTvSarBrightnessFunction ${RADIOMETRY_TESTS9}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/raTvSarBrightnessFunctionOutputAscii.txt
                    ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithNoise.txt
        otbSarBrightnessFunction
                 ${INPUTDATA}/scene.png
                 ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithNoise.txt
)

ADD_TEST(raTvSarBrightnessFunctionWithoutNoise ${RADIOMETRY_TESTS9}
        --compare-ascii ${NOTOL}
                    ${BASELINE_FILES}/raTvSarBrightnessFunctionOutputAscii.txt
                    ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithoutNoise.txt
        otbSarBrightnessFunctionWithoutNoise
                 ${INPUTDATA}/scene.png
                 ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithoutNoise.txt
)

# ------- SarBrightnessToImageFilter   ------------------------------
IF(OTB_DATA_USE_LARGEINPUT)

# Read data with a scalar IFreader (Input data type = CInt16)
  ADD_TEST(raTvSarBrightnessToImageFilter  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
                    ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
        otbSarBrightnessToImageFilterTest
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
                1000 1000 250 250 # Extract
          )

# Read data with a real IFreader (Input data type = CInt16) and done ExtractROIFilter
# after BrightnessCalibFilter without take into account Noise.
  ADD_TEST(raTvSarBrightnessToImageFilterWithoutNoise  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
                    ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
        otbSarBrightnessToImageFilterTestWithoutNoise
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
                1000 1000 250 250 # Extract
          )

# Read data with a scalar IFreader (Input data type = CInt16)
ADD_TEST(raTvSarBrightnessToImageWithComplexPixelFilter  ${RADIOMETRY_TESTS9}
            --compare-image ${EPSILON_12}
                    ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
                    ${TEMP}/raTvSarBrightnessToImageFilterComplexPixel_TSX_PANGKALANBUUN_HH.tif
        otbSarBrightnessToImageWithComplexPixelFilterTest
                ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
                ${TEMP}/raTvSarBrightnessToImageFilterComplexPixel_TSX_PANGKALANBUUN_HH.tif
                1000 1000 250 250 # Extract
        )

ENDIF(OTB_DATA_USE_LARGEINPUT)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbRADIOMETRY_TESTS10 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



# -------            Landsat TM Bright   ------------------------------
ADD_TEST(raTvLandsatTMIndexBrightTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexBright
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM Vis   ------------------------------
ADD_TEST(raTvLandsatTMIndexVisTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexVis
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM NIR   ------------------------------
ADD_TEST(raTvLandsatTMIndexNIRTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNIR
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM MIR1   ------------------------------
ADD_TEST(raTvLandsatTMIndexMIR1Test ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexMIR1
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM MIR2   ------------------------------
ADD_TEST(raTvLandsatTMIndexMIR2Test ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexMIR2
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM TIR   ------------------------------
ADD_TEST(raTvLandsatTMIndexTIRTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexTIR
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM MIRTIR   ------------------------------
ADD_TEST(raTvLandsatTMIndexMIRTIRTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexMIRTIR
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM NDVI   ------------------------------
ADD_TEST(raTvLandsatTMIndexNDVITest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNDVI
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM NDBSI   ------------------------------
ADD_TEST(raTvLandsatTMIndexNDBSITest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNDBSI
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)


# -------            Landsat TM BIO   ------------------------------
ADD_TEST(raTvLandsatTMIndexBIOTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexBIO
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM NDSI   ------------------------------
ADD_TEST(raTvLandsatTMIndexNDSITest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNDSI
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM NDSIVis   ------------------------------
ADD_TEST(raTvLandsatTMIndexNDSIVisTest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNDSIVis
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)


# -------            Landsat TM NDBBBI   ------------------------------
ADD_TEST(raTvLandsatTMIndexNDBBBITest ${RADIOMETRY_TESTS10}
        otbLandsatTMIndexNDBBBI
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

# -------            Landsat TM Linguistic Labels   ------------------------------
ADD_TEST(raTvLandsatTMLinguisticLabelsTest ${RADIOMETRY_TESTS10}
        otbLandsatTMLinguisticLabels
        3   #TM1
        4   #TM2
        8   #TM3
        21  #TM4
        33  #TM5
        41  #TM61
        8   #TM62
        21  #TM7

)

ADD_TEST(raTvLandsatTMLinguisticLabelsTest2 ${RADIOMETRY_TESTS10}
        otbLandsatTMLinguisticLabels
        3   #TM1
        4   #TM2
        80   #TM3
        100  #TM4
        33  #TM5
        1  #TM61
        8   #TM62
        51  #TM7

)

ADD_TEST(raTvLandsatTMKernelSpectralRulesTest ${RADIOMETRY_TESTS10}
        otbLandsatTMKernelSpectralRules
        63   #TM1
        64   #TM2
        57   #TM3
        150  #TM4
        33  #TM5
        1  #TM61
        8   #TM62
        51  #TM7

)

ADD_TEST(raTvLandsatTMKernelSpectralRulesTest2 ${RADIOMETRY_TESTS10}
        otbLandsatTMKernelSpectralRules
        49   #TM1
        50   #TM2
        70   #TM3
        100  #TM4
        69  #TM5
        0  #TM61
        0   #TM62
        0  #TM7

)

ADD_TEST(raTvLandsatTMKernelSpectralRulesWithImageTest ${RADIOMETRY_TESTS10}
        otbLandsatTMKernelSpectralRulesWithImage
        ${INPUTDATA}/L5Strip.tif
)

ADD_TEST(raTvLandsatTMBrightTest ${RADIOMETRY_TESTS10}
        otbLandsatTMBrightTest
        ${INPUTDATA}/L5Strip.tif
        ${TEMP}/raTvLandsatTMBrightTest_cloudImage.tif
)


ADD_TEST(raTvLandsatTMThickCloudTest ${RADIOMETRY_TESTS10}
        otbLandsatTMThickCloudTest
        ${INPUTDATA}/L5Strip.tif
        ${TEMP}/raTvLandsatTMThickCloudTest_cloudImage.tif
)

ADD_TEST(raTvLandsatTMThinCloudTest ${RADIOMETRY_TESTS10}
        otbLandsatTMThinCloudTest
        ${INPUTDATA}/L5Strip.tif
        ${TEMP}/raTvLandsatTMThinCloudTest_cloudImage.tif
)

ADD_TEST(raTvLandsatTMVegetationTest ${RADIOMETRY_TESTS10}
        otbLandsatTMVegetationTest
        ${INPUTDATA}/L5Strip.tif
        ${TEMP}/raTvLandsatTMVegetationTest_Image.tif
)

ADD_TEST(raTvLandsatTMSpectralRuleBasedClassifierTest ${RADIOMETRY_TESTS10}
        otbLandsatTMSpectralRuleBasedClassifierTest
        ${INPUTDATA}/L5Strip.tif
        ${TEMP}/raTvLandsatTMSpectralRuleBasedClassifierTest_Image.tif
)




# A enrichir
SET(Radiometry_SRCS1
otbRadiometryTests1.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
)
SET(Radiometry_SRCS2
otbRadiometryTests2.cxx
otbRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbRAndBAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbImageToLuminanceImageFilterNew.cxx
otbImageToLuminanceImageFilter.cxx
otbImageToLuminanceImageFilterAuto.cxx
otbLuminanceToReflectanceImageFilterNew.cxx
otbLuminanceToReflectanceImageFilter.cxx
otbLuminanceToReflectanceImageFilterAuto.cxx
otbImageToReflectanceImageFilterNew.cxx
otbImageToReflectanceImageFilter.cxx
otbImageToReflectanceImageFilterAuto.cxx
)
SET(Radiometry_SRCS3
otbRadiometryTests3.cxx
otbDEMCaracteristicsExtractorNew.cxx
otbDEMCaracteristicsExtractor.cxx
otbAtmosphericCorrectionParametersNew.cxx
otbFilterFunctionValues.cxx
otbSIXSTraitsTest.cxx
otbSIXSTraitsComputeAtmosphericParameters.cxx
otbAtmosphericRadiativeTermsTest.cxx
otbReflectanceToSurfaceReflectanceImageFilterTest.cxx
otbSurfaceAdjacencyEffect6SCorrectionSchemeFilterNew.cxx
otbSurfaceAdjacencyEffect6SCorrectionSchemeFilter.cxx
otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.cxx
)
SET(Radiometry_SRCS4
otbRadiometryTests4.cxx
otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTermsNew.cxx
otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms.cxx
otbAtmosphericCorrectionSequencement.cxx
otbEVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbTSARVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
)

SET(Radiometry_SRCS5
otbRadiometryTests5.cxx
otbRAndNIRVegetationIndexImageFilterNew.cxx
otbRAndNIRVegetationIndexImageFilter.cxx
)

SET(Radiometry_SRCS6
otbRadiometryTests6.cxx
otbSetASetBRAndNIRVegetationIndexImageFilter.cxx
otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbRAndGAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndGAndNIRIndexImageFilterNew.cxx
otbAVIRAndGAndNIRVegetationIndexImageFilter.cxx
otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx
otbWDVIRAndNIRVegetationIndexImageFilter.cxx
otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbLAIFromNDVILogarithmicFunctorTest.cxx
otbLAIFromReflectancesLinearFunctorTest.cxx
)

SET(Radiometry_SRCS7
otbRadiometryTests7.cxx
otbGAndRIndexImageFilterNew.cxx
otbGAndRIndexImageFilter.cxx
otbMultiChannelGAndRIndexImageFilterNew.cxx
otbMultiChannelGAndRIndexImageFilter.cxx
otbIBGAndRAndNIRIndexImageFilter.cxx
otbIBMultiChannelGAndRAndNIRIndexImageFilter.cxx
)

SET(Radiometry_SRCS8
otbRadiometryTests8.cxx
otbNDBITM4AndTM5IndexImageFilter.cxx
otbNDBIMultiChannelTM4AndTM5IndexImageFilter.cxx
otbISURAndNIRIndexImageFilter.cxx
otbISUMultiChannelRAndNIRIndexImageFilter.cxx
otbNDWIMultiChannelWaterIndexImageFilter.cxx
otbNDWIWaterIndexImageFilter.cxx
otbWaterSqrtSpectralAngleImageFilter.cxx
otbAeronetNew.cxx
otbAeronetExtractData.cxx
otbAeronetExtractDataBadData.cxx
)

SET(Radiometry_SRCS9
otbRadiometryTests9.cxx
otbTerraSarBrightnessFunctor.cxx
otbTerraSarBrightnessImageFilterNew.cxx
otbTerraSarBrightnessImageFilterTest.cxx
otbTerraSarBrightnessImageComplexFilterTest.cxx
otbSarParametricMapFunctionTest.cxx
otbSarParametricMapFunctionToImageFilter.cxx
############ Radiometric Calib tests
otbSarRadiometricCalibrationFunctor.cxx
otbSarRadiometricCalibrationFunctorWithoutNoise.cxx
otbSarRadiometricCalibrationFunction.cxx
otbSarRadiometricCalibrationFunctionWithoutNoise.cxx
otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest.cxx
otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise.cxx
otbSarRadiometricCalibrationToImageFilterWithRealPixelTest.cxx
otbSarRadiometricCalibrationToImageFilterCompareTest.cxx
otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest.cxx
############ Brightness Calib tests
otbSarBrightnessFunctor.cxx
otbSarBrightnessFunctorWithoutNoise.cxx
otbSarBrightnessFunction.cxx
otbSarBrightnessFunctionWithoutNoise.cxx
otbSarBrightnessToImageFilterTest.cxx
otbSarBrightnessToImageFilterTestWithoutNoise.cxx
otbSarBrightnessToImageWithComplexPixelFilterTest.cxx
)

SET(Radiometry_SRCS10
otbRadiometryTests10.cxx
otbLandsatTMIndexBrightTest.cxx
otbLandsatTMIndexVisTest.cxx
otbLandsatTMIndexNIRTest.cxx
otbLandsatTMIndexMIR1Test.cxx
otbLandsatTMIndexMIR2Test.cxx
otbLandsatTMIndexTIRTest.cxx
otbLandsatTMIndexMIRTIRTest.cxx
otbLandsatTMIndexNDVITest.cxx
otbLandsatTMIndexNDBSITest.cxx
otbLandsatTMIndexBIOTest.cxx
otbLandsatTMIndexNDSITest.cxx
otbLandsatTMIndexNDSIVisTest.cxx
otbLandsatTMIndexNDBBBITest.cxx
otbLandsatTMLinguisticLabelsTest.cxx
otbLandsatTMKernelSpectralRules.cxx
otbLandsatTMBrightTest.cxx
otbLandsatTMThickCloudTest.cxx
otbLandsatTMThinCloudTest.cxx
otbLandsatTMVegetationTest.cxx
otbLandsatTMSpectralRuleBasedClassifierTest.cxx
)


OTB_ADD_EXECUTABLE(otbRadiometryTests1 "${Radiometry_SRCS1}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests2 "${Radiometry_SRCS2}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests3 "${Radiometry_SRCS3}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests4 "${Radiometry_SRCS4}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests5 "${Radiometry_SRCS5}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests6 "${Radiometry_SRCS6}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests7 "${Radiometry_SRCS7}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests8 "${Radiometry_SRCS8}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests9 "${Radiometry_SRCS9}" "OTBRadiometry;OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbRadiometryTests10 "${Radiometry_SRCS10}" "OTBRadiometry;OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
