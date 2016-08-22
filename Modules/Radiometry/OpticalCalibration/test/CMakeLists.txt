otb_module_test()

set(OTBOpticalCalibrationTests
otbOpticalCalibrationTestDriver.cxx
otbSpectralSensitivityReaderTest.cxx
otbReflectanceToImageImageFilter.cxx
otbReflectanceToLuminanceImageFilterNew.cxx
otbLuminanceToReflectanceImageFilterNew.cxx
otbLuminanceToReflectanceImageFilterAuto.cxx
otbImageToLuminanceImageFilterNew.cxx
otbRadiometryCorrectionParametersToAtmosphericRadiativeTermsNew.cxx
otbLuminanceToImageImageFilterAuto.cxx
otbLuminanceToImageImageFilterNew.cxx
otbReflectanceToLuminanceImageFilterAuto.cxx
otbAeronetExtractDataBadData.cxx
otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.cxx
otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.cxx
otbSurfaceAdjacencyEffectCorrectionSchemeFilterNew.cxx
otbImageToReflectanceImageFilterAuto.cxx
otbAtmosphericCorrectionParametersNew.cxx
otbImageToReflectanceImageFilterNew.cxx
otbWavelengthSpectralBandsTest.cxx
otbAtmosphericRadiativeTermsTest.cxx
otbImageToReflectanceImageFilter.cxx
otbLuminanceToReflectanceImageFilter.cxx
otbReflectanceToImageImageFilterAuto.cxx
otbAeronetNew.cxx
otbAeronetExtractData.cxx
otbReflectanceToSurfaceReflectanceImageFilterTest.cxx
otbImageMetadataCorrectionParametersNew.cxx
otbImageToLuminanceImageFilterAuto.cxx
otbAtmosphericCorrectionSequencement.cxx
otbSIXSTraitsTest.cxx
otbSIXSTraitsComputeAtmosphericParameters.cxx
otbReflectanceToImageImageFilterNew.cxx
otbSurfaceAdjacencyEffectCorrectionSchemeFilter.cxx
otbLuminanceToImageImageFilter.cxx
otbReflectanceToLuminanceImageFilter.cxx
otbImageToLuminanceImageFilter.cxx
)

add_executable(otbOpticalCalibrationTestDriver ${OTBOpticalCalibrationTests})
target_link_libraries(otbOpticalCalibrationTestDriver ${OTBOpticalCalibration-Test_LIBRARIES})
otb_module_target_label(otbOpticalCalibrationTestDriver)

# Tests Declaration

otb_add_test(NAME raTuSpectralSensitivityReaderNew COMMAND otbOpticalCalibrationTestDriver
  otbSpectralSensitivityReaderNew
  )

otb_add_test(NAME raTvSpectralSensitivityReaderSpot4Test COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${EPSILON_5}
  ${BASELINE_FILES}/raTvSpectralSensitivityReaderSpot4Test.txt
  ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
  --ignore-lines-with 2 DataPath FileName
  otbSpectralSensitivityReaderGenericTest
  ${INPUTDATA}/Radiometry/SPOT4/HRVIR1/rep6S.dat
  LARGEINPUT{SPOT4/ROMANIA/raTvS4_20020518_Reflectance.img}
  ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
  )

otb_add_test(NAME raTvSpectralSensitivityReaderGenericTest COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${EPSILON_5}
  ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
  ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
  --ignore-lines-with 2 DataPath FileName
  otbSpectralSensitivityReaderGenericTest
  ${INPUTDATA}/Radiometry/SPOT5/HRG1/rep6S.dat
  ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
  )

otb_add_test(NAME raTvSpectralSensitivityReaderTest COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${EPSILON_5}
  ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
  ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
  --ignore-lines-with 2 DataPath FileName
  otbSpectralSensitivityReaderTest
  ${INPUTDATA}/Radiometry
  ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
  )

otb_add_test(NAME raTvReflectanceToImageImageFilterDayMounth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDayMounth.tif
  otbReflectanceToImageImageFilter
  ${BASELINE}/raTvImageToReflectanceImageFilter.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDayMounth.tif
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

otb_add_test(NAME raTvReflectanceToImageImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDsol.tif
  otbReflectanceToImageImageFilter
  ${BASELINE}/raTvImageToReflectanceImageFilterDsol.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDsol.tif
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

otb_add_test(NAME raTvRomaniaReflectanceToImage COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raTvRomaniaImage.tif
  otbReflectanceToImageImageFilter
  ${BASELINE}/raTvRomania_Reflectance.tif
  ${TEMP}/raTvRomaniaImage.tif
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

otb_add_test(NAME raTuReflectanceToLuminanceImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbReflectanceToLuminanceImageFilterNew
  )

otb_add_test(NAME raTuLuminanceToReflectanceImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbLuminanceToReflectanceImageFilterNew
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoSpot5.img
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoSpot5.img
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif
  )

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoFormosat.tif
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoFormosat.img
  otbLuminanceToReflectanceImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvLuminanceToReflectanceImageFilterAutoFormosat.img
  )

otb_add_test(NAME raTuImageToLuminanceImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbImageToLuminanceImageFilterNew
  )


otb_add_test(NAME raTuRadiometryCorrectionParametersToAtmosphericRadiativeTermsNew COMMAND otbOpticalCalibrationTestDriver
  otbRadiometryCorrectionParametersToAtmosphericRadiativeTermsNew)

otb_add_test(NAME raTuLuminanceToImageImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbLuminanceToImageImageFilterNew
  )

#FIXME Those tests have performing local copies of header, baselines...They were migrate "as it" for now in version 5.0 but
# something cleaner could be done in the future.
#TODO Rewrite those tests and ban local copy and cmake trickery
if(OTB_DATA_USE_LARGEINPUT)

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibLumToImQBPAN)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${TOULOUSEQBDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TOULOUSEQBDIR}/raTvSMALLTOULOUSELuminanceToImageImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvLuminanceToImageImageFilterAutoQuickbirdPAN.tif
    otbLuminanceToImageImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    ${TOULOUSEQBDIR}/raTvLuminanceToImageImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvSMALLTOULOUSELuminanceToImageImageFilterAutoQuickbirdPAN.tif
    )


  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibLumToImQBXS)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${TOULOUSEQBDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TOULOUSEQBDIR}/raTvSMALLTOULOUSELuminanceToImageImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvLuminanceToImageImageFilterAutoQuickbirdXS.tif
    otbLuminanceToImageImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
    ${TOULOUSEQBDIR}/raTvLuminanceToImageImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvSMALLTOULOUSELuminanceToImageImageFilterAutoQuickbirdXS.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibLumToImWv2MULTI)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${ROMEWV2DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${ROMEWV2DIR}/raTvSMALLROMELuminanceToImageImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvLuminanceToImageImageFilterAutoWV2Multi.tif
    otbLuminanceToImageImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF
    LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
    ${ROMEWV2DIR}/raTvLuminanceToImageImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvSMALLROMELuminanceToImageImageFilterAutoWV2Multi.tif
    )

  set(BLOSSEVILLEIKONOSDIR ${TEMP}/OpticalCalibLumToImIkonos)
  file(MAKE_DIRECTORY ${BLOSSEVILLEIKONOSDIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*metadata.txt ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*pan*)
  foreach(f ${MTDATA})
    configure_file(${f} ${BLOSSEVILLEIKONOSDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoIkonos.tif ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BLOSSEVILLEIKONOSDIR}/raTvSMALLBLOSSEVILLELuminanceToImageImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvLuminanceToImageImageFilterAutoIkonos.tif
    otbLuminanceToImageImageFilterAuto
    ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif
    LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
    ${BLOSSEVILLEIKONOSDIR}/raTvLuminanceToImageImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvSMALLBLOSSEVILLELuminanceToImageImageFilterAutoIkonos.tif
    )

  set(TEHERANSPOT5DIR ${TEMP}/OpticalCalibLumToImSPOT5)
  file(MAKE_DIRECTORY ${TEHERANSPOT5DIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/*.DIM)
  foreach(f ${MTDATA})
    configure_file(${f} ${TEHERANSPOT5DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoSpot5.tif ${TEHERANSPOT5DIR}/IMAGERY.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TEHERANSPOT5DIR}/raTvSMALLTEHERANLuminanceToImageImageFilterAutoSpot5.img
    ${TEHERANSPOT5DIR}/raTvLuminanceToImageImageFilterAutoSpot5.tif
    otbLuminanceToImageImageFilterAuto
    ${TEHERANSPOT5DIR}/IMAGERY.TIF
    LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
    ${TEHERANSPOT5DIR}/raTvLuminanceToImageImageFilterAutoSpot5.tif
    ${TEHERANSPOT5DIR}/raTvSMALLTEHERANLuminanceToImageImageFilterAutoSpot5.img
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibLumToImWv2PAN)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${ROMEWV2DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${ROMEWV2DIR}/raTvSMALLROMELuminanceToImageImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvLuminanceToImageImageFilterAutoWV2PAN.tif
    otbLuminanceToImageImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF
    LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
    ${ROMEWV2DIR}/raTvLuminanceToImageImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvSMALLROMELuminanceToImageImageFilterAutoWV2PAN.tif
    )

  set(SUDOUESTFORMOSATDIR ${TEMP}/OpticalCalibLumToImFormosat)
  file(MAKE_DIRECTORY ${SUDOUESTFORMOSATDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/FORMOSAT/Sudouest_20071013_MS_fmsat)
  file(GLOB MTDATA ${MTDATADIR}/*.DIM)
  foreach(f ${MTDATA})
    configure_file(${f} ${SUDOUESTFORMOSATDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvImageToLuminanceImageFilterAutoFormosat.tif ${SUDOUESTFORMOSATDIR}/IMAGERY.TIF COPYONLY)

  otb_add_test(NAME raTvLuminanceToImageImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${SUDOUESTFORMOSATDIR}/raTvSMALLSOLuminanceToImageImageFilterAutoFormosat.img
    ${SUDOUESTFORMOSATDIR}/raTvLuminanceToImageImageFilterAutoFormosat.tif
    otbLuminanceToImageImageFilterAuto
    ${SUDOUESTFORMOSATDIR}/IMAGERY.TIF
    LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
    ${SUDOUESTFORMOSATDIR}/raTvLuminanceToImageImageFilterAutoFormosat.tif
    ${SUDOUESTFORMOSATDIR}/raTvSMALLSOLuminanceToImageImageFilterAutoFormosat.img
    )

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibRefToLumQBPAN)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${TOULOUSEQBDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdPAN.tif ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvReflectanceToLuminanceImageFilterAutoQuickbirdPAN.tif
    otbReflectanceToLuminanceImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF
    ${TOULOUSEQBDIR}/raTvReflectanceToLuminanceImageFilterAutoQuickbirdPAN.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibRefToLumWv2PAN)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${ROMEWV2DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2PAN.tif ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvReflectanceToLuminanceImageFilterAutoWV2PAN.tif
    otbReflectanceToLuminanceImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF
    ${ROMEWV2DIR}/raTvReflectanceToLuminanceImageFilterAutoWV2PAN.tif
    )

  set(TEHERANSPOT5DIR ${TEMP}/OpticalCalibRefToLumSPOT5)
  file(MAKE_DIRECTORY ${TEHERANSPOT5DIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/*.DIM)
  foreach(f ${MTDATA})
    configure_file(${f} ${TEHERANSPOT5DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoSpot5.tif ${TEHERANSPOT5DIR}/IMAGERY.TIF COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoSpot5.tif
    ${TEHERANSPOT5DIR}/raTvReflectanceToLuminanceImageFilterAutoSpot5.img
    otbReflectanceToLuminanceImageFilterAuto
    ${TEHERANSPOT5DIR}/IMAGERY.TIF
    ${TEHERANSPOT5DIR}/raTvReflectanceToLuminanceImageFilterAutoSpot5.img
    )

  set(BLOSSEVILLEIKONOSDIR ${TEMP}/OpticalCalibRefToLumIkonos)
  file(MAKE_DIRECTORY ${BLOSSEVILLEIKONOSDIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*metadata.txt ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*pan*)
  foreach(f ${MTDATA})
    configure_file(${f} ${BLOSSEVILLEIKONOSDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoIkonos.tif ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvReflectanceToLuminanceImageFilterAutoIkonos.tif
    otbReflectanceToLuminanceImageFilterAuto
    ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvReflectanceToLuminanceImageFilterAutoIkonos.tif
    )

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibRefToLumQBXS)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${TOULOUSEQBDIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoQuickbirdXS.tif ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvReflectanceToLuminanceImageFilterAutoQuickbirdXS.tif
    otbReflectanceToLuminanceImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF
    ${TOULOUSEQBDIR}/raTvReflectanceToLuminanceImageFilterAutoQuickbirdXS.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibRefToLumWv2MULTI)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  foreach(f ${MTDATA})
    configure_file(${f} ${ROMEWV2DIR} COPYONLY)
  endforeach(f)
  configure_file(${BASELINE}/raTvLuminanceToReflectanceImageFilterAutoWV2Multi.tif ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF COPYONLY)

  otb_add_test(NAME raTvReflectanceToLuminanceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvReflectanceToLuminanceImageFilterAutoWV2Multi.tif
    otbReflectanceToLuminanceImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF
    ${ROMEWV2DIR}/raTvReflectanceToLuminanceImageFilterAutoWV2Multi.tif
    )

endif()

otb_add_test(NAME raTvAeronetExtractData_BadDate COMMAND otbOpticalCalibrationTestDriver
  otbAeronetExtractDataBadData
  ${INPUTDATA}/AERONET/090101_091231_Seysses.lev15
  23    # Day
  2     # Month
  2009  # Year
  11    # Hour
  0     # Minute
  0.4   # Epsilon
  )

otb_add_test(NAME raTvAeronetExtractData_BadHour COMMAND otbOpticalCalibrationTestDriver
  otbAeronetExtractDataBadData
  ${INPUTDATA}/AERONET/090101_091231_Seysses.lev15
  16    # Day
  2     # Month
  2009  # Year
  10    # Hour
  0     # Minute
  0.4   # Epsilon
  )

otb_add_test(NAME raTvRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvRadiometryCorrectionParametersToAtmosphericRadiativeTerms COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${EPSILON_4}
  ${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
  ${TEMP}/raTvCorrectionTo6SRadiative.txt
  otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms
  ${INPUTDATA}/in6S_otb
  ${TEMP}/raTvCorrectionTo6SRadiative.txt
  )

otb_add_test(NAME raTuSurfaceAdjacencyEffectCorrectionSchemeFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbSurfaceAdjacencyEffectCorrectionSchemeFilterNew
  )

otb_add_test(NAME raTvImageToReflectanceImageFilterAuto COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToReflectanceImageFilterAuto.tif
  ${TEMP}/raTvImageToReflectanceImageFilterAuto.img
  otbImageToReflectanceImageFilterAuto
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/raTvImageToReflectanceImageFilterAuto.img
  )

otb_add_test(NAME raTuImageToReflectanceImageFilterAutoFORMOSAT2 COMMAND otbOpticalCalibrationTestDriver
  otbImageToReflectanceImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvImageToReflectanceImageFilterAutoFORMOSAT2.tif
  )

otb_add_test(NAME raTuAtmosphericCorrectionParametersNew COMMAND otbOpticalCalibrationTestDriver
  otbAtmosphericCorrectionParametersNew
  )

otb_add_test(NAME raTuImageToReflectanceImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbImageToReflectanceImageFilterNew
  )

otb_add_test(NAME raTuWavelengthSpectralBandsNew COMMAND otbOpticalCalibrationTestDriver
  otbWavelengthSpectralBandsNew
  )

otb_add_test(NAME raTuFilterFunctionValuesNew COMMAND otbOpticalCalibrationTestDriver
  otbFilterFunctionValuesNew
  )

otb_add_test(NAME raTvAtmosphericRadiativeTermsTest COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/raTvAtmosphericRadiativeTermsTest.txt
  ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
  otbAtmosphericRadiativeTermsTest
  ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
  )

otb_add_test(NAME raTuAtmosphericRadiativeTermsNew COMMAND otbOpticalCalibrationTestDriver
  otbAtmosphericRadiativeTermsNew
  )

otb_add_test(NAME raTuAtmosphericRadiativeTermsSingleChannelNew COMMAND otbOpticalCalibrationTestDriver
  otbAtmosphericRadiativeTermsSingleChannelNew
  )

otb_add_test(NAME raTvImageToReflectanceImageFilterDayMounth COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvImageToReflectanceImageFilter COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvRomaniaImageToReflectance COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvLuminanceToReflectanceImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvLuminanceToReflectanceImageFilter COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTuReflectanceToImageImageFilterAutoFORMOSAT2 COMMAND otbOpticalCalibrationTestDriver
  otbReflectanceToImageImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvReflectanceToImageImageFilterAutoFORMOSAT2.tif
  ${TEMP}/raTvSMALLReflectanceToImageImageFilterAutoFORMOSAT2.tif
  )

otb_add_test(NAME raTvReflectanceToImageImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${TEHERANSPOT5DIR}/raTvSMALLTEHERANReflectanceToImageImageFilterAutoSpot5.img
  ${TEHERANSPOT5DIR}/raTvReflectanceToImageImageFilterAutoSpot5.tif
  otbReflectanceToImageImageFilterAuto
  ${TEHERANSPOT5DIR}/IMAGERY.TIF
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEHERANSPOT5DIR}/raTvReflectanceToImageImageFilterAutoSpot5.tif
  ${TEHERANSPOT5DIR}/raTvSMALLTEHERANReflectanceToImageImageFilterAutoSpot5.img
  )

otb_add_test(NAME raTuAeronetNew COMMAND otbOpticalCalibrationTestDriver
  otbAeronetNew
  )

otb_add_test(NAME raTvAeronet_ToulouseLevel20_ExtractData COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvAeronet_ArcachonLevel10_ExtractData COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTvAeronet_SeyssesLevel15_ExtractData COMMAND otbOpticalCalibrationTestDriver
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

otb_add_test(NAME raTuReflectanceToSurfaceReflectanceImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbReflectanceToSurfaceReflectanceImageFilterNew
  )

otb_add_test(NAME raTvReflectanceToSurfaceReflectanceImageFilter2 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvReflectanceToSurfaceReflectanceImageFilter2.tif
  ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter2.tif
  otbReflectanceToSurfaceReflectanceImageFilterTest2
  ${INPUTDATA}/ToulouseExtract_WithGeom.tif
  ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter2.tif
  1013
  1.424
  0.344
  1
  0.1
  )

otb_add_test(NAME raTvReflectanceToSurfaceReflectanceImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
  ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
  otbReflectanceToSurfaceReflectanceImageFilterTest
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvReflectanceToSurfaceReflectanceImageFilter.tif
  1 1 1 1 # intrinsic atmospheric reflectance
  2 2 2 2 # spherical albedo of the atmosphere
  3 3 3 3 # total transmission
  2 2 2 2 # downward transmittance
  3 3 3 3 # upward transmittance
  )

otb_add_test(NAME raTuImageMetadataCorrectionParametersNew COMMAND otbOpticalCalibrationTestDriver
  otbImageMetadataCorrectionParametersNew
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoFormosat.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoFormosat.img
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoFormosat.img
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoIkonos.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoIkonos.tif
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoIkonos.tif
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdXS.tif
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoQuickbirdPAN.tif
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2Multi.tif
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoSpot5.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoSpot5.img
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoSpot5.img
  )

otb_add_test(NAME raTvImageToLuminanceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
  ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
  otbImageToLuminanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvImageToLuminanceImageFilterAutoWV2PAN.tif
  )

otb_add_test(NAME raTvAtmosphericCorrectionSequencementTest COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/raTvRomaniaAtmosphericCorrectionSequencement.tif
  ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
  otbAtmosphericCorrectionSequencementTest
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raTvRomaniaAtmosphericCorrectionSequencement.tif
  ${INPUTDATA}/atmosphericCorrectionSequencement_alpha_beta.txt
  27.3    #elevation et azimuth solaire
  4       #day
  12      #month
  ${INPUTDATA}/atmosphericCorrectionSequencement_solar_illumination.txt
  152.7
  2.5
  -77.0
  1013.
  2.48134
  0.34400
  1
  0.199854
  ${INPUTDATA}/atmosphericCorrectionSequencement_wavelength_spectral_bands_spot4_1.txt
  2       # Radius;
  0.020   # pixel spacing in kilometers
  )

otb_add_test(NAME raTuSIXSTraitsTest COMMAND otbOpticalCalibrationTestDriver
  otbSIXSTraitsTest)

otb_add_test(NAME raTvSIXSTraitsComputeAtmosphericParametersTest COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${EPSILON_4} ${BASELINE_FILES}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
  ${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
  otbSIXSTraitsComputeAtmosphericParametersTest
  ${INPUTDATA}/in6S_otb
  ${TEMP}/raTvSIXSTraitsComputeAtmosphericParametersTest.txt
  )

otb_add_test(NAME raTuReflectanceToImageImageFilterNew COMMAND otbOpticalCalibrationTestDriver
  otbReflectanceToImageImageFilterNew
  )

otb_add_test(NAME raTvSurfaceAdjacencyEffectCorrectionSchemeFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
  ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
  otbSurfaceAdjacencyEffectCorrectionSchemeFilter
  ${BASELINE}/raTvRomania_Correction.tif
  ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilter.tif
  2                                                            # Radius;
  0.020                                                        # pixel spacing in kilometers
  ${INPUTDATA}/romania_parameter.txt                           # atmo param;
  ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B3.txt   # wavelengths, channel 3
  ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B2.txt   # wavelengths, channel 2
  ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_B1.txt   # wavelengths, channel 1
  ${INPUTDATA}/RADIO_WAVELENGHT_SPECTRAL_BAND_SPOT4_1_MIR.txt  # wavelengths, channel 4
  ${TEMP}/raTvSurfaceAdjacencyEffect6SCorrectionSchemeFilterOutput6SVallues.txt
  )

otb_add_test(NAME raTvLuminanceToImageImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilter.tif
  otbLuminanceToImageImageFilter
  ${BASELINE}/raTvImageToLuminanceImageFilter.tif
  ${TEMP}/raTvverySmallFSATSWImageFilter.tif
  10   #channel 1 alpha
  20   #channel 2 alpha
  30   #channel 3 alpha
  40   #channel 4 alpha
  1    #channel 1 beta
  2    #channel 2 beta
  3    #channel 3 beta
  4    #channel 4 beta
  )

otb_add_test(NAME raTvReflectanceToLuminanceImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDsol.tif
  otbReflectanceToLuminanceImageFilter
  ${BASELINE}/raTvLuminanceToReflectanceImageFilterDsol.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDsol.tif
  0.2 #radius
  10  #channel 1 illumination
  20  #channel 2 illumination
  30  #channel 3 illumination
  40  #channel 4 illumination
  0.9923885328
  )

otb_add_test(NAME raTvReflectanceToLuminanceImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDayMounth.tif
  otbReflectanceToLuminanceImageFilter
  ${BASELINE}/raTvLuminanceToReflectanceImageFilter.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDayMounth.tif
  0.2 #radius
  10  #channel 1 illumination
  20  #channel 2 illumination
  30  #channel 3 illumination
  40  #channel 4 illumination
  3   #day
  5   #mounth
  )

otb_add_test(NAME raTvImageToLuminanceImageFilter COMMAND otbOpticalCalibrationTestDriver
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
