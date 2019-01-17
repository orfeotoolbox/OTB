#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBOpticalCalibrationTests
otbOpticalCalibrationTestDriver.cxx
otbSpectralSensitivityReaderTest.cxx
otbReflectanceToImageImageFilter.cxx
otbRadianceToReflectanceImageFilterAuto.cxx
otbRadianceToImageImageFilterAuto.cxx
otbReflectanceToRadianceImageFilterAuto.cxx
otbAeronetExtractDataBadData.cxx
otbRomaniaReflectanceToRomaniaSurfaceReflectanceImageFilter.cxx
otbRadiometryCorrectionParametersToAtmosphericRadiativeTerms.cxx
otbImageToReflectanceImageFilterAuto.cxx
otbAtmosphericRadiativeTermsTest.cxx
otbImageToReflectanceImageFilter.cxx
otbRadianceToReflectanceImageFilter.cxx
otbReflectanceToImageImageFilterAuto.cxx
otbAeronetExtractData.cxx
otbReflectanceToSurfaceReflectanceImageFilterTest.cxx
otbImageToRadianceImageFilterAuto.cxx
otbAtmosphericCorrectionSequencement.cxx
otbSIXSTraitsTest.cxx
otbSIXSTraitsComputeAtmosphericParameters.cxx
otbSurfaceAdjacencyEffectCorrectionSchemeFilter.cxx
otbRadianceToImageImageFilter.cxx
otbReflectanceToRadianceImageFilter.cxx
otbImageToRadianceImageFilter.cxx
)

add_executable(otbOpticalCalibrationTestDriver ${OTBOpticalCalibrationTests})
target_link_libraries(otbOpticalCalibrationTestDriver ${OTBOpticalCalibration-Test_LIBRARIES})
otb_module_target_label(otbOpticalCalibrationTestDriver)
set_linker_stack_size_flag(otbOpticalCalibrationTestDriver 10000000)

# Tests Declaration

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

otb_add_test(NAME raTvReflectanceToImageImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDayMonth.tif
  otbReflectanceToImageImageFilter
  ${BASELINE}/raTvImageToReflectanceImageFilter.tif
  ${TEMP}/raTvReflectanceToImageImageFilterDayMonth.tif
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
  5   #month
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



otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoSpot5.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoSpot5.img
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoSpot5.img
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoFormosat.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoFormosat.img
  otbRadianceToReflectanceImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvRadianceToReflectanceImageFilterAutoFormosat.img
  )




#FIXME Those tests have performing local copies of header, baselines...They were migrate "as it" for now in version 5.0 but
# something cleaner could be done in the future.
#TODO Rewrite those tests and ban local copy and cmake trickery
if(OTB_DATA_USE_LARGEINPUT AND OTB_DATA_ROOT)

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibLumToImQBPAN)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${TOULOUSEQBDIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdPAN.tif ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TOULOUSEQBDIR}/raTvSMALLTOULOUSERadianceToImageImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvRadianceToImageImageFilterAutoQuickbirdPAN.tif
    otbRadianceToImageImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
    ${TOULOUSEQBDIR}/raTvRadianceToImageImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvSMALLTOULOUSERadianceToImageImageFilterAutoQuickbirdPAN.tif
    )


  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibLumToImQBXS)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${TOULOUSEQBDIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdXS.tif ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TOULOUSEQBDIR}/raTvSMALLTOULOUSERadianceToImageImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvRadianceToImageImageFilterAutoQuickbirdXS.tif
    otbRadianceToImageImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF
    LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
    ${TOULOUSEQBDIR}/raTvRadianceToImageImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvSMALLTOULOUSERadianceToImageImageFilterAutoQuickbirdXS.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibLumToImWv2MULTI)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${ROMEWV2DIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoWV2Multi.tif ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${ROMEWV2DIR}/raTvSMALLROMERadianceToImageImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvRadianceToImageImageFilterAutoWV2Multi.tif
    otbRadianceToImageImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF
    LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
    ${ROMEWV2DIR}/raTvRadianceToImageImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvSMALLROMERadianceToImageImageFilterAutoWV2Multi.tif
    )

  set(BLOSSEVILLEIKONOSDIR ${TEMP}/OpticalCalibLumToImIkonos)
  file(MAKE_DIRECTORY ${BLOSSEVILLEIKONOSDIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*metadata.txt ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*pan*)
  file(COPY ${MTDATA} DESTINATION ${BLOSSEVILLEIKONOSDIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoIkonos.tif ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BLOSSEVILLEIKONOSDIR}/raTvSMALLBLOSSEVILLERadianceToImageImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvRadianceToImageImageFilterAutoIkonos.tif
    otbRadianceToImageImageFilterAuto
    ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif
    LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
    ${BLOSSEVILLEIKONOSDIR}/raTvRadianceToImageImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvSMALLBLOSSEVILLERadianceToImageImageFilterAutoIkonos.tif
    )

  set(TEHERANSPOT5DIR ${TEMP}/OpticalCalibLumToImSPOT5)
  file(MAKE_DIRECTORY ${TEHERANSPOT5DIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/*.DIM)
  file(COPY ${MTDATA} DESTINATION ${TEHERANSPOT5DIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoSpot5.tif ${TEHERANSPOT5DIR}/IMAGERY.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${TEHERANSPOT5DIR}/raTvSMALLTEHERANRadianceToImageImageFilterAutoSpot5.img
    ${TEHERANSPOT5DIR}/raTvRadianceToImageImageFilterAutoSpot5.tif
    otbRadianceToImageImageFilterAuto
    ${TEHERANSPOT5DIR}/IMAGERY.TIF
    LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
    ${TEHERANSPOT5DIR}/raTvRadianceToImageImageFilterAutoSpot5.tif
    ${TEHERANSPOT5DIR}/raTvSMALLTEHERANRadianceToImageImageFilterAutoSpot5.img
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibLumToImWv2PAN)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${ROMEWV2DIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoWV2PAN.tif ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${ROMEWV2DIR}/raTvSMALLROMERadianceToImageImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvRadianceToImageImageFilterAutoWV2PAN.tif
    otbRadianceToImageImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF
    LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
    ${ROMEWV2DIR}/raTvRadianceToImageImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvSMALLROMERadianceToImageImageFilterAutoWV2PAN.tif
    )

  set(SUDOUESTFORMOSATDIR ${TEMP}/OpticalCalibLumToImFormosat)
  file(MAKE_DIRECTORY ${SUDOUESTFORMOSATDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/FORMOSAT/Sudouest_20071013_MS_fmsat)
  file(GLOB MTDATA ${MTDATADIR}/*.DIM)
  file(COPY ${MTDATA} DESTINATION ${SUDOUESTFORMOSATDIR})
  otb_copy_rename(${BASELINE}/raTvImageToRadianceImageFilterAutoFormosat.tif ${SUDOUESTFORMOSATDIR}/IMAGERY.TIF)

  otb_add_test(NAME raTvRadianceToImageImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${SUDOUESTFORMOSATDIR}/raTvSMALLSORadianceToImageImageFilterAutoFormosat.img
    ${SUDOUESTFORMOSATDIR}/raTvRadianceToImageImageFilterAutoFormosat.tif
    otbRadianceToImageImageFilterAuto
    ${SUDOUESTFORMOSATDIR}/IMAGERY.TIF
    LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
    ${SUDOUESTFORMOSATDIR}/raTvRadianceToImageImageFilterAutoFormosat.tif
    ${SUDOUESTFORMOSATDIR}/raTvSMALLSORadianceToImageImageFilterAutoFormosat.img
    )

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibRefToLumQBPAN)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${TOULOUSEQBDIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdPAN.tif
    ${TOULOUSEQBDIR}/raTvReflectanceToRadianceImageFilterAutoQuickbirdPAN.tif
    otbReflectanceToRadianceImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-P1BS-000000128955_01_P001.TIF
    ${TOULOUSEQBDIR}/raTvReflectanceToRadianceImageFilterAutoQuickbirdPAN.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibRefToLumWv2PAN)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${ROMEWV2DIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoWV2PAN.tif
    ${ROMEWV2DIR}/raTvReflectanceToRadianceImageFilterAutoWV2PAN.tif
    otbReflectanceToRadianceImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-P2AS-052298844010_01_P001.TIF
    ${ROMEWV2DIR}/raTvReflectanceToRadianceImageFilterAutoWV2PAN.tif
    )

  set(TEHERANSPOT5DIR ${TEMP}/OpticalCalibRefToLumSPOT5)
  file(MAKE_DIRECTORY ${TEHERANSPOT5DIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/*.DIM)
  file(COPY ${MTDATA} DESTINATION ${TEHERANSPOT5DIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoSpot5.tif ${TEHERANSPOT5DIR}/IMAGERY.TIF)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoSpot5.tif
    ${TEHERANSPOT5DIR}/raTvReflectanceToRadianceImageFilterAutoSpot5.img
    otbReflectanceToRadianceImageFilterAuto
    ${TEHERANSPOT5DIR}/IMAGERY.TIF
    ${TEHERANSPOT5DIR}/raTvReflectanceToRadianceImageFilterAutoSpot5.img
    )

  set(BLOSSEVILLEIKONOSDIR ${TEMP}/OpticalCalibRefToLumIkonos)
  file(MAKE_DIRECTORY ${BLOSSEVILLEIKONOSDIR})
  file(GLOB MTDATA ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*metadata.txt ${OTB_DATA_LARGEINPUT_ROOT}/IKONOS/BLOSSEVILLE/*pan*)
  file(COPY ${MTDATA} DESTINATION ${BLOSSEVILLEIKONOSDIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoIkonos.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvReflectanceToRadianceImageFilterAutoIkonos.tif
    otbReflectanceToRadianceImageFilterAuto
    ${BLOSSEVILLEIKONOSDIR}/po_2619900_pan_0000000.tif
    ${BLOSSEVILLEIKONOSDIR}/raTvReflectanceToRadianceImageFilterAutoIkonos.tif
    )

  set(TOULOUSEQBDIR ${TEMP}/OpticalCalibRefToLumQBXS)
  file(MAKE_DIRECTORY ${TOULOUSEQBDIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${TOULOUSEQBDIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdXS.tif
    ${TOULOUSEQBDIR}/raTvReflectanceToRadianceImageFilterAutoQuickbirdXS.tif
    otbReflectanceToRadianceImageFilterAuto
    ${TOULOUSEQBDIR}/02APR01105228-M1BS-000000128955_01_P001.TIF
    ${TOULOUSEQBDIR}/raTvReflectanceToRadianceImageFilterAutoQuickbirdXS.tif
    )

  set(ROMEWV2DIR ${TEMP}/OpticalCalibRefToLumWv2MULTI)
  file(MAKE_DIRECTORY ${ROMEWV2DIR})
  set(MTDATADIR ${OTB_DATA_LARGEINPUT_ROOT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL)
  file(GLOB MTDATA ${MTDATADIR}/*TIL ${MTDATADIR}/*RPB ${MTDATADIR}/*XML ${MTDATADIR}/*IMD)
  file(COPY ${MTDATA} DESTINATION ${ROMEWV2DIR})
  otb_copy_rename(${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF)

  otb_add_test(NAME raTvReflectanceToRadianceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
    --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoWV2Multi.tif
    ${ROMEWV2DIR}/raTvReflectanceToRadianceImageFilterAutoWV2Multi.tif
    otbReflectanceToRadianceImageFilterAuto
    ${ROMEWV2DIR}/09DEC10103019-M2AS-052298844010_01_P001.TIF
    ${ROMEWV2DIR}/raTvReflectanceToRadianceImageFilterAutoWV2Multi.tif
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





otb_add_test(NAME raTvAtmosphericRadiativeTermsTest COMMAND otbOpticalCalibrationTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/raTvAtmosphericRadiativeTermsTest.txt
  ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
  otbAtmosphericRadiativeTermsTest
  ${TEMP}/raTvAtmosphericRadiativeTermsTest.txt
  )



otb_add_test(NAME raTvImageToReflectanceImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToReflectanceImageFilter.tif
  ${TEMP}/raTvImageToReflectanceImageFilterDayMonth.tif
  otbImageToReflectanceImageFilter
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvImageToReflectanceImageFilterDayMonth.tif
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
  5   #month
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

otb_add_test(NAME raTvRadianceToReflectanceImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilter.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterDayMonth.tif
  otbRadianceToReflectanceImageFilter
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterDayMonth.tif
  0.2 #radius
  10  #channel 1 illumination
  20  #channel 2 illumination
  30  #channel 3 illumination
  40  #channel 4 illumination
  3   #day
  5   #month
  )

otb_add_test(NAME raTvRadianceToReflectanceImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvRadianceToReflectanceImageFilterDsol.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterDsol.tif
  otbRadianceToReflectanceImageFilter
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvRadianceToReflectanceImageFilterDsol.tif
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


otb_add_test(NAME raTvImageToRadianceImageFilterAutoFORMOSAT COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoFormosat.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoFormosat.img
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoFormosat.img
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoIkonos COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoIkonos.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoIkonos.tif
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif}
  ${TEMP}/raTvImageToRadianceImageFilterAutoIkonos.tif
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoQuickbirdXS COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdXS.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoQuickbirdXS.tif
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoQuickbirdXS.tif
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoQuickbirdPAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoQuickbirdPAN.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoQuickbirdPAN.tif
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoQuickbirdPAN.tif
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoWV2MULTI COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoWV2Multi.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoWV2Multi.tif
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoWV2Multi.tif
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoSpot5 COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoSpot5.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoSpot5.img
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoSpot5.img
  )

otb_add_test(NAME raTvImageToRadianceImageFilterAutoWV2PAN COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilterAutoWV2PAN.tif
  ${TEMP}/raTvImageToRadianceImageFilterAutoWV2PAN.tif
  otbImageToRadianceImageFilterAuto
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  ${TEMP}/raTvImageToRadianceImageFilterAutoWV2PAN.tif
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

otb_add_test(NAME raTvRadianceToImageImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilter.tif
  otbRadianceToImageImageFilter
  ${BASELINE}/raTvImageToRadianceImageFilter.tif
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

otb_add_test(NAME raTvReflectanceToRadianceImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDsol.tif
  otbReflectanceToRadianceImageFilter
  ${BASELINE}/raTvRadianceToReflectanceImageFilterDsol.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDsol.tif
  0.2 #radius
  10  #channel 1 illumination
  20  #channel 2 illumination
  30  #channel 3 illumination
  40  #channel 4 illumination
  0.9923885328
  )

otb_add_test(NAME raTvReflectanceToRadianceImageFilterDayMonth COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDayMonth.tif
  otbReflectanceToRadianceImageFilter
  ${BASELINE}/raTvRadianceToReflectanceImageFilter.tif
  ${TEMP}/raTvverySmallFSATSWImageFilterDayMonth.tif
  0.2 #radius
  10  #channel 1 illumination
  20  #channel 2 illumination
  30  #channel 3 illumination
  40  #channel 4 illumination
  3   #day
  5   #month
  )

otb_add_test(NAME raTvImageToRadianceImageFilter COMMAND otbOpticalCalibrationTestDriver
  --compare-image ${EPSILON_12}  ${BASELINE}/raTvImageToRadianceImageFilter.tif
  ${TEMP}/raTvImageToRadianceImageFilter.tif
  otbImageToRadianceImageFilter
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raTvImageToRadianceImageFilter.tif
  10   #channel 1 alpha
  20   #channel 2 alpha
  30   #channel 3 alpha
  40   #channel 4 alpha
  1    #channel 1 beta
  2    #channel 2 beta
  3    #channel 3 beta
  4    #channel 4 beta
  )
