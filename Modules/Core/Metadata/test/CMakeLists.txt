otb_module_test()

set(OTBMetadataTests
otbMetadataTestDriver.cxx
otbOpticalDefaultImageMetadataInterfaceNew.cxx
otbSarImageMetadataInterfaceTest.cxx
otbDefaultImageMetadataInterfaceFactoryNew.cxx
otbOpticalDefaultImageMetadataInterfaceFactoryNew.cxx
otbPleiadesImageMetadataInterfaceNew.cxx
##>otbImageMetadataInterfaceBaseTest.cxx
otbSarDefaultImageMetadataInterface.cxx
otbSarDefaultImageMetadataInterfaceNew.cxx
otbOpticalImageMetadataInterfaceTest.cxx
otbSarDefaultImageMetadataInterfaceFactoryNew.cxx
otbFormosatImageMetadataInterfaceNew.cxx
otbOpticalDefaultImageMetadataInterface.cxx
otbSpot6ImageMetadataInterfaceNew.cxx
otbSpotImageMetadataInterfaceNew.cxx
otbQuickBirdImageMetadataInterfaceNew.cxx
otbIkonosImageMetadataInterfaceNew.cxx
otbTerraSarImageMetadataInterfaceNew.cxx
otbSentinel1ImageMetadataInterfaceNew.cxx
otbWorldView2ImageMetadataInterfaceNew.cxx
otbDefaultImageMetadataInterface.cxx
otbImageMetadataInterfaceTest2.cxx
otbNoDataHelperTest.cxx
otbSarCalibrationLookupDataTest.cxx
otbRadarsat2ImageMetadataInterfaceNew.cxx
)

add_executable(otbMetadataTestDriver ${OTBMetadataTests})
target_link_libraries(otbMetadataTestDriver ${OTBMetadata-Test_LIBRARIES})
otb_module_target_label(otbMetadataTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuOpticalDefaultImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbOpticalDefaultImageMetadataInterfaceNew )

otb_add_test(NAME ioTvSarImageMetadataInterfaceTest_TSX1dims COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1dims.txt
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
  otbSarImageMetadataInterfaceTest
  LARGEINPUT{TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos}
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
  )

otb_add_test(NAME ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingVVCosFile COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
  otbSarImageMetadataInterfaceTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos}
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
  )

otb_add_test(NAME ioTvSarImageMetadataInterfaceTest_TSX1TORONTO COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
  otbSarImageMetadataInterfaceTest
  LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos}
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
  )

otb_add_test(NAME ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingXmlFile COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
  otbSarImageMetadataInterfaceTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml}
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
  )

otb_add_test(NAME ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingHHCosFile COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
  otbSarImageMetadataInterfaceTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_SENTINEL1 COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_SENTINEL1.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_SENTINEL1.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_SENTINEL1.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_RADARSAT2 COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_RADARSAT2.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_RADARSAT2.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HV.tif}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_RADARSAT2.txt
##>  )

otb_add_test(NAME ioTuSentinel1ImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbSentinel1ImageMetadataInterfaceNew )

otb_add_test(NAME ioTuRadarsat2ImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbRadarsat2ImageMetadataInterfaceNew )

otb_add_test(NAME ioTuDefaultImageMetadataInterfaceFactoryNew COMMAND otbMetadataTestDriver
  otbDefaultImageMetadataInterfaceFactoryNew )

otb_add_test(NAME ioTuOpticalDefaultImageMetadataInterfaceFactoryNew COMMAND otbMetadataTestDriver
  otbOpticalDefaultImageMetadataInterfaceFactoryNew )

otb_add_test(NAME ioTuPleiadesImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbPleiadesImageMetadataInterfaceNew
  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_SPOT COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_SPOT.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingVVCosFile COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingHHCosFile COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_FORMOSAT2 COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_TSX1dims COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_QBMUL COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBMUL.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_WV2Mul COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_QB COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QB.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_WV2Pan COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_IKONOS COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_IKONOS.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingXmlFile COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_QBCevennes COMMAND otbMetadataTestDriver
##>  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
##>  )

##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_TSX1TORONTO COMMAND otbMetadataTestDriver
##>  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
##>  otbImageMetadataInterfaceBaseTest
##>  LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos}
##>  ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
##>  )

otb_add_test(NAME ioTuSarDefaultImageMetadataInterface COMMAND otbMetadataTestDriver
  otbSarDefaultImageMetadataInterface )

otb_add_test(NAME ioTuSarDefaultImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbSarDefaultImageMetadataInterfaceNew )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_QBCevennes COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_WV2Pan COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_QBMUL COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBMUL.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_WV2Mul COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_SPOT COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_SPOT.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_QB COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QB.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_IKONOS COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_IKONOS.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif}
  ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
  )

otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_FORMOSAT2 COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
  ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
  otbOpticalImageMetadataInterfaceTest
  LARGEINPUT{FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF}
  ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
  )

otb_add_test(NAME ioTuSarDefaultImageMetadataInterfaceFactoryNew COMMAND otbMetadataTestDriver
  otbSarDefaultImageMetadataInterfaceFactoryNew )

otb_add_test(NAME ioTuFormosatImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbFormosatImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuOpticalDefaultImageMetadataInterface COMMAND otbMetadataTestDriver
  otbOpticalDefaultImageMetadataInterface )

otb_add_test(NAME ioTuSpot6ImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbSpot6ImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuSpotImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbSpotImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuQuickBirdImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbQuickBirdImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuIkonosImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbIkonosImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuTerraSarImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbTerraSarImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuWorldView2ImageMetadataInterfaceNew COMMAND otbMetadataTestDriver
  otbWorldView2ImageMetadataInterfaceNew
  )

otb_add_test(NAME ioTuDefaultImageMetadataInterface COMMAND otbMetadataTestDriver
  otbDefaultImageMetadataInterface )

otb_add_test(NAME ioTvImageMetadataInterfaceTest COMMAND otbMetadataTestDriver
  --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterface.txt
  ${TEMP}/ioTvImageMetadataInterface.txt
  otbImageMetadataInterfaceTest2
  ${EXAMPLEDATA}/qb_RoadExtract.tif
  ${TEMP}/ioTvImageMetadataInterface.txt
  )

set( GenericTestPHR_TESTNB 0)
foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
  list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")

  ##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_${current_type} COMMAND otbMetadataTestDriver
  ##>  --compare-ascii ${NOTOL}
  ##>    ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_${current_type}.txt
  ##>    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
  ##>  otbImageMetadataInterfaceBaseTest
  ##>    ${current_file}
  ##>   ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
  ##>  )

  otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_${current_type} COMMAND otbMetadataTestDriver
    --compare-ascii ${NOTOL}
      ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_${current_type}.txt
      ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
    otbOpticalImageMetadataInterfaceTest
      ${current_file}
      ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
    )
endforeach()

# SPOT 6
set( GenericTestSPOT6_TESTNB 0)

foreach( current_file ${GenericTestSPOT6_DATA_INPUTS} )
  list(GET GenericTestSPOT6_DATA_TYPES   ${GenericTestSPOT6_TESTNB} current_type )
  math(EXPR GenericTestSPOT6_TESTNB "${GenericTestSPOT6_TESTNB} + 1")

  ##>otb_add_test(NAME ioTvImageMetadataInterfaceBaseTest_${current_type} COMMAND otbMetadataTestDriver
  ##>  --compare-ascii ${NOTOL}
  ##>    ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_${current_type}.txt
  ##>    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
  ##>  otbImageMetadataInterfaceBaseTest
  ##>    ${current_file}
  ##>    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
  ##>  )

  otb_add_test(NAME ioTvOpticalImageMetadataInterfaceTest_${current_type} COMMAND otbMetadataTestDriver
    --compare-ascii ${NOTOL}
      ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_${current_type}.txt
      ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
    otbOpticalImageMetadataInterfaceTest
      ${current_file}
      ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
    )
endforeach()

otb_add_test(NAME coreMetaDataNoDataHelperTest COMMAND otbMetadataTestDriver
  otbNoDataHelperTest)

otb_add_test(NAME ioTvSarCalibrationLookupDataTest_SENTINEL1 COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  otbSarCalibrationLookupDataTest
  ${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.tif?&geom=${INPUTDATA}/SENTINEL1_SLC_S6_1S_extract_300_300.geom
  ${TEMP}/ioTvSarCalibrationLookupDataTest_SENTINEL1.txt
  )

otb_add_test(NAME ioTvSarCalibrationLookupDataTest_RADARSAT2 COMMAND otbMetadataTestDriver
  --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  ${TEMP}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  otbSarCalibrationLookupDataTest
  ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif?&geom=${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.geom
  ${TEMP}/ioTvSarCalibrationLookupDataTest_RADARSAT2.txt
  )
