
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


if(OTB_DATA_USE_SPOTPHRINPUT)
  set(SPOTPHRINPUT ${OTB_DATA_SPOTPHRINPUT_ROOT} )
endif()

if(OTB_DATA_USE_CNESPHRINPUT)
  set(CNESPHRINPUT ${OTB_DATA_CNESPHRINPUT_ROOT} )
endif()

set(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
set(COMMON_TESTS3 ${CXX_TEST_PATH}/otbCommonTests3)
set(IO_TESTS1     ${CXX_TEST_PATH}/otbIOTests1)
set(IO_TESTS2     ${CXX_TEST_PATH}/otbIOTests2)
set(IO_TESTS3     ${CXX_TEST_PATH}/otbIOTests3)
set(IO_TESTS4     ${CXX_TEST_PATH}/otbIOTests4)
set(IO_TESTS5     ${CXX_TEST_PATH}/otbIOTests5)
set(IO_TESTS6     ${CXX_TEST_PATH}/otbIOTests6)
set(IO_TESTS7     ${CXX_TEST_PATH}/otbIOTests7)
set(IO_TESTS8     ${CXX_TEST_PATH}/otbIOTests8)
set(IO_TESTS9     ${CXX_TEST_PATH}/otbIOTests9)
set(IO_TESTS10    ${CXX_TEST_PATH}/otbIOTests10)
set(IO_TESTS11    ${CXX_TEST_PATH}/otbIOTests11)
set(IO_TESTS12    ${CXX_TEST_PATH}/otbIOTests12)

if(OTB_USE_JPEG2000_TESTING)
   set(IO_TESTS13    ${CXX_TEST_PATH}/otbIOTests13)
endif()

set(IO_TESTS14    ${CXX_TEST_PATH}/otbIOTests14)
set(IO_TESTS15    ${CXX_TEST_PATH}/otbIOTests15)
set(IO_TESTS16    ${CXX_TEST_PATH}/otbIOTests16)

set(IO_TESTS18    ${CXX_TEST_PATH}/otbIOTests18)

if(OTB_USE_CURL)
  set(IO_TESTS19    ${CXX_TEST_PATH}/otbIOTests19)
endif()

set(IO_TESTS20    ${CXX_TEST_PATH}/otbIOTests20)
set(IO_TESTS21    ${CXX_TEST_PATH}/otbIOTests21)

if(CHECK_GDAL_BUILT_WITH_HDF)
  set(IO_TESTS22    ${CXX_TEST_PATH}/otbIOTests22)
endif()

set(IO_TESTS23    ${CXX_TEST_PATH}/otbIOTests23)
set(IO_TESTS24    ${CXX_TEST_PATH}/otbIOTests24)

# --------------------------------------------------------------------------------
# Manage PHR data input
if(OTB_DATA_USE_LARGEINPUT)
   set(GenericTestPHR_DATA_INPUTS
      ${GenericTestPHR_DATA_INPUTS}
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits/IMG_PHR1Z_PMS_N_001/IMG_PHR1A_PMS-N_201006181052297_ORT_IPU_20111011_0619-001_R1C1.JP2
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20111109_7807-004_R1C1.TIF
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
      ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
      ${LARGEINPUT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2
      ${LARGEINPUT}/PLEIADES/PARIS_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits/IMGPHR_PHR1A_ORTHO_20120502_IPU_20120430_6347-003_V1/IMG_R1C1.JP2
   )

   set(GenericTestPHR_DATA_TYPES
      ${GenericTestPHR_DATA_TYPES}
      SIMUL_PHR_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits
      SIMUL_PHR_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits
      SIMUL_PHR_TIFF_PRIMARY_DIMAPv2_MS_lossy_12bits
      PHR_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits
   )

   set(GenericTestSPOT6_DATA_INPUTS
      ${GenericTestSPOT6_DATA_INPUTS}
      ${LARGEINPUT}/SPOT6/SPOT6_Ortho_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201306031242000_ORT_816005101_R1C1.JP2
      ${LARGEINPUT}/SPOT6/SPOT6_Ortho_Pan-sharpened_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_PMS_001_A/IMG_SPOT6_PMS_201305251604372_ORT_816009101_R1C1.JP2
      ${LARGEINPUT}/SPOT6/SPOT6_Primary_Bundle_12_bits/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201211100138085_SEN_816007101_R1C1.JP2
   )

   set(GenericTestSPOT6_DATA_TYPES
      ${GenericTestSPOT6_DATA_TYPES}
      SPOT6_Ortho_Bundle_12_bits
      SPOT6_Ortho_Pan-sharpened_12_bits
      SPOT6_Primary_Bundle_12_bits
   )

endif()

if(OTB_DATA_USE_SPOTPHRINPUT)
  set(GenericTestPHR_SPOT_DATA_INPUTS
      ${SPOTPHRINPUT}/ORTHO_UTM_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_ORT_IPU_20120221_8798-001_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_UTM_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20120221_8798-002_R1C1.JP2
      ${SPOTPHRINPUT}/PRIMARY_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_SEN_IPU_20120221_8686-001_R1C1.JP2
      ${SPOTPHRINPUT}/PRIMARY_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20120221_8686-002_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_GEOGRAPHIC_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_ORT_IPU_20120221_8718-001_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_GEOGRAPHIC_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20120221_8718-002_R1C1.JP2
      )

  set(GenericTestPHR_SPOT_DATA_TYPES
	  SPOT_PHR_ORTHO_UTM_P
	  SPOT_PHR_ORTHO_UTM_MS
	  SPOT_PHR_ORTHO_GEO_P
      	  SPOT_PHR_ORTHO_GEO_MS
	  SPOT_PHR_PRIMARY_MS
	  SPOT_PHR_PRIMARY_P
	 )

   set(GenericTestPHR_DATA_INPUTS ${GenericTestPHR_DATA_INPUTS} ${GenericTestPHR_SPOT_DATA_INPUTS})
   set(GenericTestPHR_DATA_TYPES ${GenericTestPHR_DATA_TYPES} ${GenericTestPHR_SPOT_DATA_TYPES})
endif()

if(OTB_DATA_USE_CNESPHRINPUT)
  set(GenericTestPHR_CNES_DATA_INPUTS
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201212716417737721/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120127_5579-001_R1C1.JP2
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R3C1.JP2
      )

  set(GenericTestPHR_CNES_DATA_TYPES
	  CNES_PHR_PRIMARY_PMS_R1C1_2012-01-27
	  CNES_PHR_PRIMARY_PMS_R1C1_2012-02-22
	  CNES_PHR_PRIMARY_PMS_R3C1_2012-02-22
	 )

  set(GenericTestPHR_DATA_INPUTS ${GenericTestPHR_DATA_INPUTS} ${GenericTestPHR_CNES_DATA_INPUTS})
  set(GenericTestPHR_DATA_TYPES ${GenericTestPHR_DATA_TYPES} ${GenericTestPHR_CNES_DATA_TYPES})
endif()

# Use in debug to show the variables
#set( GenericTestPHR_TESTNB 0)
#foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
#  list(GET GenericTestPHR_DATA_TYPES  ${GenericTestPHR_TESTNB} current_type   )
#  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
#
#  message(STATUS ${current_file})
#  message(STATUS ${current_type})
#
#endforeach()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(OTB_DATA_USE_LARGEINPUT)

# ---  RADARSAT (GDAL)  ---
add_test(ioTvMultiMonoChannelROI_RADARSAT2ENVI ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROI
         -ushort ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  dir name ---
add_test(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI ${COMMON_TESTS3}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
#                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         -startX 0 -startY 0 -sizeX ParseHdfSubsetName337 -sizeY 280 )
set_tests_properties(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI PROPERTIES DEPENDS ioTvMultiMonoChannelROI_RADARSAT2ENVI)
endif()

# ---  ENVI (GDAL) ---
add_test(ioTvMultiMonoChannelROI_ENVI2PNG ${COMMON_TESTS3}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         -startX 302 -startY 2 -sizeX 134 -sizeY 330
         -channel 1)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(OTB_DATA_USE_LARGEINPUT)
# ---  SAR_ERS2_SLCI (GDAL)  ---
add_test(ioTvImageFileReaderWithComplexPixel_PALSAR ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
            ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
            0 0 100 100)

add_test(ioTvImageFileReaderWithComplexPixel_ERS ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/
            ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
            0 0 100 100)

add_test(ioTvImageFileReaderWithComplexPixel_RADARSAT ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
            0 0 100 100)

add_test(ioTvVectorImageFileReaderWithComplexPixel_RADARSAT2 ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
        otbVectorImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
            0 0 100 100)

add_test(ioTvCompareWritingComplexImage_TIF ${IO_TESTS1}
        otbCompareWritingComplexImageTest
            ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
            ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.tif
            0 0 100 100)

add_test(ioTvCompareWritingComplexImage_HDR ${IO_TESTS1}
         otbCompareWritingComplexImageTest
         ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.hdr
         0 0 100 100)

endif()

# ------- TEST IO COMMON   ------------------------------


# -------            otb::LUMImageIO   ------------------------------
# CanRead()
add_test(ioTuLUMImageIOCanRead ${IO_TESTS1} otbLUMImageIOTestCanRead
        ${INPUTDATA}/poupees.lum)

# CanWrite()
add_test(ioTuLUMImageIOCanWrite ${IO_TESTS1} otbLUMImageIOTestCanWrite
        ${TEMP}/poupees.lum)

add_test(ioTvImageFileReaderHDR2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_1canal.c1.hdr
                                ${TEMP}/ioImageFileReaderHDR2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.c1.hdr
        ${TEMP}/ioImageFileReaderHDR2LUM.lum )

add_test(ioTvImageFileReaderPNG2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2LUM.lum )

add_test(ioTvImageFileReaderLUM2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioImageFileReaderLUM2LUM.lum
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderPNG2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2LUM.lum )
set_tests_properties(ioTvImageFileReaderLUM2LUM PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

add_test(ioTvImageFileWriterLUM2HDR ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderLUM2HDR.hdr
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderLUM2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2HDR.hdr )
set_tests_properties(ioTvImageFileWriterLUM2HDR PROPERTIES DEPENDS ioTvImageFileReaderLUM2LUM)

# --- Image I2
add_test(ioTvImageFileReaderShortLUM2HDR ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr )

add_test(ioTvImageFileReaderShortHDR2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum
        otbShortImageIOTest
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum )
set_tests_properties(ioTvImageFileReaderShortHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderShortLUM2HDR)

add_test(ioTvImageFileReaderShortLUM2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum )

# -------            otb::MWImageIO   ------------------------------
# CanRead()
add_test(ioTuMWImageIOCanReadIMG ${IO_TESTS1} otbMWImageIOTestCanRead
        ${BASELINE}/QB_Toulouse_Ortho_PAN.img)

add_test(ioTuMWImageIOCanReadMW ${IO_TESTS1} otbMWImageIOTestCanRead
        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw)

add_test(ioTvImageFileReaderMW2TIFIMG ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN_nogeoinfo.tif
                                ${TEMP}/ioImageFileReaderMW2TIFIMG.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
        ${TEMP}/ioImageFileReaderMW2TIFIMG.tif )

add_test(ioTvImageFileReaderMW2TIFMW ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw
                                ${TEMP}/ioImageFileReaderMW2TIFMW.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
        ${TEMP}/ioImageFileReaderMW2TIFMW.tif )


# CanWrite()
add_test(ioTuMWImageIOCanWriteMW ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.mw)

add_test(ioTuMWImageIOCanWriteIMG ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.img)
set_tests_properties(ioTuMWImageIOCanWriteIMG PROPERTIES WILL_FAIL true)

add_test(ioTvImageFileReaderTIF2MW ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.mw
                                ${TEMP}/ioImageFileReaderTIF2MW.mw
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderTIF2MW.mw )


# We use the original file as a baseline for the comparison
 add_test(ioTvImageFileReaderPCI2TIF ${IO_TESTS1}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                    ${TEMP}/ioImageFileReaderPCI2TIF.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix
        ${TEMP}/ioImageFileReaderPCI2TIF.tif )

 add_test(ioTvImageFileReaderTIF2PCI ${IO_TESTS1}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                    ${TEMP}/ioImageFileReaderTIF2PCI.pix
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderTIF2PCI.pix )

#-------------  otbImageIOFactoryNew
add_test(ioTuImageIOFactoryNew ${IO_TESTS1}
        otbImageIOFactoryNew )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# --- Image I4
add_test(ioTvImageFileReaderIntLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.lum
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr )
add_test(ioTvImageFileReaderIntHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum )
set_tests_properties(ioTvImageFileReaderIntHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderIntLUM2HDR)

# --- Image FLOAT
add_test(ioTvImageFileReaderFloatLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.lum
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr )

add_test(ioTvImageFileReaderFloatHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum )
set_tests_properties(ioTvImageFileReaderFloatHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderFloatLUM2HDR)

# --- Image DOUBLE
add_test(ioTvImageFileReaderDoubleLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.lum
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr )

add_test(ioTvImageFileReaderDoubleHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum )
set_tests_properties(ioTvImageFileReaderDoubleHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderDoubleLUM2HDR)



add_test(ioTvImageFileReaderFloatPNG2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum
        otbFloatImageIOTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum )


# ---  STREAMING : LUM  ---
add_test(ioTvStreamingImageFilterLUM2PNG ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFilterTestLUM.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFilterTestLUM.png
         )
set_tests_properties(ioTvStreamingImageFilterLUM2PNG PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterLUMWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         0 # StreamingImageFileWriter
         0 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingIFWriterLUMWithoutStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  With STREAMING IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterLUMWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         1 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingIFWriterLUMWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  With IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingWithIFWriterLUMWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
         0 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingWithIFWriterLUMWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  STREAMING  Pipeline Example ---
add_test(ioTvStreamingPipeline_TIF2PNG ${IO_TESTS2}
         otbPipeline
         ${INPUTDATA}/cthead1.png #ROI_IKO_PAN_LesHalles.tif
         ${TEMP}/ioPipeline.png
         )

# --- Image FLOAT
add_test(ioTvImageFileReaderFloatBSQ2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.hd
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr )
add_test(ioTvImageFileReaderFloatHDR2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd )
set_tests_properties(ioTvImageFileReaderFloatHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderFloatBSQ2HDR)

# --- Image DOUBLE
add_test(ioTvImageFileReaderDoubleBSQ2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.hd
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr )

add_test(ioTvImageFileReaderDoubleHDR2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd )
set_tests_properties(ioTvImageFileReaderDoubleHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderDoubleBSQ2HDR)

# ---  STREAMING FILTER : BSQ  ---
add_test(ioTvImageFileReaderPNG2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2BSQ.hd
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2BSQ.hd )

add_test(ioTvStreamingImageFilterBSQ2PNG ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFilterTestBSQ.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFilterTestBSQ.png
         )
set_tests_properties(ioTvStreamingImageFilterBSQ2PNG PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         0 # StreamingImageFileWriter
         0 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingIFWriterBSQWithoutStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         1 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingIFWriterBSQWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingWithIFWriterBSQWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
         0 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
set_tests_properties(ioTvStreamingWithIFWriterBSQWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)


add_test(ioTvGDALImageIO_Tiff_NoOption ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         )

add_test(ioTvGDALImageIO_Tiff_12bits ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
         "NBITS=12"
         )

add_test(ioTvGDALImageIO_Tiff_Stripped ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
         "BLOCKYSIZE=10"
         )

add_test(ioTvGDALImageIO_Tiff_Tiled_16x16 ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
         "TILED=YES"
         "BLOCKXSIZE=16"
         "BLOCKYSIZE=16"
         )

add_test(ioTvGDALImageIO_Tiff_JPEG_20 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_Tiff_JPEG_20.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_20.tif
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_20.tif
         "COMPRESS=JPEG"
         "JPEG_QUALITY=20"
         "PHOTOMETRIC=YCBCR"
         "TILED=YES"
         "BLOCKXSIZE=256"
         "BLOCKYSIZE=256"
         )

add_test(ioTvGDALImageIO_Tiff_JPEG_99 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_Tiff_JPEG_99.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_99.tif
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_JPEG_99.tif
         "COMPRESS=JPEG"
         "JPEG_QUALITY=99"
         "PHOTOMETRIC=YCBCR"
         "TILED=YES"
         "BLOCKXSIZE=256"
         "BLOCKYSIZE=256"
         )

add_test(ioTvGDALImageIO_JPEG_20 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg
                            ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
         "QUALITY=20"
         )

add_test(ioTvGDALImageIO_JPEG_99 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_99.jpg
                            ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
         "QUALITY=99"
         )


add_test(ioTuGDALOverviewsBuilder ${IO_TESTS2}
         otbGDALOverviewsBuilderNew)

add_test(ioTvGDALOverviewsBuilder_TIFF ${IO_TESTS2}
         otbGDALOverviewsBuilder
         ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         4
         )
set_tests_properties(ioTvGDALOverviewsBuilder_TIFF PROPERTIES DEPENDS ioTvGDALImageIO_Tiff_NoOption)




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::BSQImageIO   ------------------------------
# CanRead()
add_test(ioTuBSQImageIOCanRead ${IO_TESTS3} otbBSQImageIOTestCanRead
        ${INPUTDATA}/poupees.hd)

# CanWrite()
add_test(ioTuBSQImageIOCanWrite ${IO_TESTS3} otbBSQImageIOTestCanWrite
        ${TEMP}/poupees.hd)

add_test(ioTvImageFileReaderPNG2ENVI ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioImageFileReaderPNG2ENVI.hdr
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2ENVI.hdr )
set_tests_properties(ioTvImageFileReaderPNG2ENVI PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)


add_test(ioTvImageFileReaderBSQ2PNG ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_1canal.hd
                                ${TEMP}/ioImageFileReaderBSQ2PNG.png
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.hd
        ${TEMP}/ioImageFileReaderBSQ2PNG.png )

add_test(ioTvImageFileReaderRGB_PNG2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/couleurs_extrait.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd )

add_test(ioTvImageFileReaderRGB_PNG2ENVI ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/couleurs_extrait.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr )

# FIXME Desactivated until http://bugs.orfeo-toolbox.org/view.php?id=103
# is solved
#add a new test
 add_test(ioTvCheckNbBandsPNGIndexee ${IO_TESTS3}
         otbPNGIndexedNbBandsTest
         ${INPUTDATA}/sbuv_indexee.png
         4 )


# --- Image I2
add_test(ioTvImageFileReaderShortRGB_BSQ2HDR ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        otbShortRGBImageIOTest
        ${INPUTDATA}/poupees_I2.hd
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr )

add_test(ioTvImageFileReaderShortHDR2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd
        otbShortRGBImageIOTest
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd )
set_tests_properties(ioTvImageFileReaderShortHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderShortRGB_BSQ2HDR)

# --- Image I4
add_test(ioTvImageFileReaderIntBSQ2HDR ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.hd
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr )

add_test(ioTvImageFileReaderIntHDR2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd )
set_tests_properties(ioTvImageFileReaderIntHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderIntBSQ2HDR)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(NOT OTB_TESTING_STANDALONE)

if(OTB_DATA_USE_LARGEINPUT)

add_test(ioTvImageKeywordlistSPOT5 ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5_saveState_loadState.txt
         --ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
         ${TEMP}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistFORMOSAT2 ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistFORMOSAT2.txt
         ${TEMP}/ioTvImageKeywordlistFORMOSAT2.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistFORMOSAT2.txt
         ${TEMP}/ioTvImageKeywordlistFORMOSAT2_saveState_loadState.txt
         --ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
         ${TEMP}/ioTvImageKeywordlistFORMOSAT2.txt
         ${TEMP}/ioTvImageKeywordlistFORMOSAT2_saveState_loadState.txt
)


add_test(ioTvImageKeywordlistQuickbird ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird_saveState_loadState.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistIkonos ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos_saveState_loadState.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
         ${TEMP}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistRapidEye ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf
         ${TEMP}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistEnvisatAsar ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistAlosPalsar ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistRadarSat1_GOMA ${IO_TESTS4}
    --compare-n-ascii ${EPSILON_6} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/DAT_01.001
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistRadarSat1_GOMA_vdf ${IO_TESTS4}
    --compare-n-ascii ${EPSILON_6} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_vdf.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_GOMA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_vdf_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/VDF_DAT.001
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_vdf.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_GOMA_vdf_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistRadarSat1_SARDEGNA ${IO_TESTS4}
    --compare-n-ascii ${EPSILON_6} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/RADARSAT1/SARDEGNA/scene01/dat_01.001
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_saveState_loadState.txt
         )

add_test(ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf ${IO_TESTS4}
    --compare-n-ascii ${EPSILON_6} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1_SARDEGNA.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/RADARSAT1/SARDEGNA/scene01/dat_01.001
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf_saveState_loadState.txt
         )

add_test(ioTvImageKeywordlistRadarSat2 ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2_saveState_loadState.txt
          --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
         ${TEMP}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistTerraSarX ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_saveState_loadState.txt
          --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_saveState_loadState.txt
)

add_test(ioTvImageKeywordlistTerraSarX_Upsala ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala_saveState_loadState.txt
          --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/TSX1_SAR__MGD.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala_saveState_loadState.txt
)
add_test(ioTvImageKeywordlistTerraSarX_Toronto ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto_saveState_loadState.txt
          --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto_saveState_loadState.txt
)

add_test(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingXmlFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)
add_test(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingHHFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)
add_test(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingVVFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)



add_test(ioTvImageKeywordlistErs ${IO_TESTS4}
  --ignore-order --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
         ${TEMP}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs_saveState_loadState.txt
)

endif()

if(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)

  set( GenericTestPHR_TESTNB 0)

  foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
    list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type )
    math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")

      # Get and manipulate ImageKeywordlist
      add_test(ioTvImageKeywordlist_${current_type} ${IO_TESTS4}
               --ignore-order --compare-n-ascii ${NOTOL} 2
                 ${BASELINE_FILES}/ioTvImageKeywordlist_${current_type}.txt
                 ${TEMP}/ioTvImageKeywordlist_${current_type}_OUT.txt
                 ${BASELINE_FILES}/ioTvImageKeywordlist_${current_type}.txt
                 ${TEMP}/ioTvImageKeywordlist_${current_type}_saveState_loadState_OUT.txt
               --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
               otbImageKeywordlist
                  ${current_file}
                  ${TEMP}/ioTvImageKeywordlist_${current_type}_OUT.txt
                  ${TEMP}/ioTvImageKeywordlist_${current_type}_saveState_loadState_OUT.txt
              )

  endforeach()


  set( GenericTestSPOT6_TESTNB 0)

  foreach( current_file ${GenericTestSPOT6_DATA_INPUTS} )
    list(GET GenericTestSPOT6_DATA_TYPES   ${GenericTestSPOT6_TESTNB} current_type )
    math(EXPR GenericTestSPOT6_TESTNB "${GenericTestSPOT6_TESTNB} + 1")

      # Get and manipulate ImageKeywordlist
      add_test(ioTvImageKeywordlist_${current_type} ${IO_TESTS4}
               --ignore-order --compare-n-ascii ${NOTOL} 2
                 ${BASELINE_FILES}/ioTvImageKeywordlist_${current_type}.txt
                 ${TEMP}/ioTvImageKeywordlist_${current_type}_OUT.txt
                 ${BASELINE_FILES}/ioTvImageKeywordlist_${current_type}.txt
                 ${TEMP}/ioTvImageKeywordlist_${current_type}_saveState_loadState_OUT.txt
               --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
               otbImageKeywordlist
                  ${current_file}
                  ${TEMP}/ioTvImageKeywordlist_${current_type}_OUT.txt
                  ${TEMP}/ioTvImageKeywordlist_${current_type}_saveState_loadState_OUT.txt
              )

  endforeach()

endif()


add_test(ioTvOssimJpegFileRessourceLeak ${IO_TESTS4}
        otbOssimJpegFileRessourceLeakTest
        ${INPUTDATA}/poupees.jpg
        1200
)
endif()


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::GDALImageIO   ------------------------------

# READER

add_test(ioTuGDALImageIOCanRead_ENVI ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupees_1canal.c1.hdr )
        ${INPUTDATA}/poupees_1canal.c1 )


add_test(ioTuGDALImageIOCanRead_PDS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${INPUTDATA}/pdsImage.img )

if(OTB_DATA_USE_LARGEINPUT)

add_test(ioTuGDALImageIOCanRead_RADARSAT ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/ )
        ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/DAT_01.001 )

add_test(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/ )
        ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

add_test(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF )

add_test(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF )

add_test(ioTuGDALImageIOCanRead_WV2_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF )

add_test(ioTuGDALImageIOCanRead_WV2_MUL ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF )

add_test(ioTuGDALImageIOCanRead_WV2_PSH ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_pansharpened_natural_color_8bit/052298834010_01_P001_PSH/09DEC10103019-S2AS-052298834010_01_P001.TIF )


add_test(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_pan_0000000.tif)

add_test(ioTuGDALImageIOCanRead_IKONOS_GRN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_grn_0000000.tif)

add_test(ioTuGDALImageIOCanRead_IKONOS_BLU ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_blu_0000000.tif)

add_test(ioTuGDALImageIOCanRead_IKONOS_RED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_red_0000000.tif)

add_test(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif)

add_test(ioTuGDALImageIOCanRead_SPOT4 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/ )
        ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT )

add_test(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif)

add_test(ioTuGDALImageIOCanRead_FORMOSAT2 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF)

add_test(ioTuGDALImageIOCanRead_COSMOSKYMED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5)

add_test(ioTuGDALImageIOCanRead_COSMOSKYMED2 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_SCS_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5)

add_test(ioTuGDALImageIOCanRead_COSMOSKYMED3 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS2_GTC_B_S2_08_HH_RD_SF_20110417180326_20110417180333.h5)

endif()

add_test(ioTuGDALImageIOCanRead_SPOT5TIF ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupeesTIF/ )
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF )

add_test(ioTuGDALImageIOCanRead_HFA ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${INPUTDATA}/HFAGeoreferenced.img)

add_test(ioTuGDALImageIOCanRead_PCI ${IO_TESTS5} otbGDALImageIOTestCanRead
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix)

add_test(ioTvThreadsTest ${IO_TESTS5} otbThreadsTest)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# WRITER

add_test(ioTuGDALImageIOCanWrite_TIFF ${IO_TESTS6} otbGDALImageIOTestCanWrite
        ${INPUTDATA}/testGDALWriter.tif)
add_test(ioTuGDALImageIOCanWrite_HFA ${IO_TESTS6} otbGDALImageIOTestCanWrite
        ${INPUTDATA}/HFAGeoreferenced.img)

# Pb with copy of some infos into the dataset
# cases with no ProjRef
add_test(ioTvGDALImageIOWriteMetadata ${IO_TESTS6}
#    --compare-ascii ${NOTOL}
#       ${TEMP}/ioTvGDALImageIOWriteMetadataCreate_TIFF_None.txt
#       ${TEMP}/ioTvGDALImageIOWriteMetadataWR_TIFF_None.txt
    otbGDALImageIOTestWriteMetadata
       ${TEMP}/ioTvGDALImageIOWriteMetadata.tif
       ${TEMP}/ioTvGDALImageIOWriteMetadata.hdr
       ${TEMP}/ioTvGDALImageIOWriteMetadata_Report.txt
    )



# -------            otb::ONERAImageIO   ------------------------------
# CanRead()
if(OTB_DATA_USE_LARGEINPUT)

add_test(ioTuONERAImageIOCanRead ${IO_TESTS6} otbONERAImageIOTestCanRead
        ${LARGEINPUT}/ONERA/spa3_0215_rad.ent)

# Read one ONERAimage type CFloat32 with IFreaderScalar and write to a 2 bands hdr file OK
# crop 10 10 100 100
add_test(ioTvONERAImageFileReader ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderONERA.hdr
            ${TEMP}/ioImageFileReaderONERA.hdr
         otbImageFileReaderONERATest
            ${LARGEINPUT}/ONERA/spa3_0215_rad.ent
            ${TEMP}/ioImageFileReaderONERA.hdr
            10 10 100 100 )

# Read one ONERAimage type CFloat32 with IFreaderComplex and write to 2 hdr file (Re and Im)  => OK
# crop 10 10 100 100
add_test(ioTvONERAImageFileReaderComplex ${IO_TESTS6}
         --compare-n-images ${EPSILON_9} 2
                        ${BASELINE}/ioImageFileReaderONERAComplexReal.hdr
                        ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
                        ${BASELINE}/ioImageFileReaderONERAComplexImag.hdr
                        ${TEMP}/ioImageFileReaderONERAComplexImag.hdr
         otbImageFileReaderONERAComplex
            ${LARGEINPUT}/ONERA/spa3_0215_rad.ent
            ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
                ${TEMP}/ioImageFileReaderONERAComplexImag.hdr )

# Read one ONERAimage type CFloat32 with IFreaderComplex and write to the same type of file  => OK
# crop 20 20 100 100
add_test(ioTvONERAImageFileWriterComplex ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
         otbImageFileWriterONERAComplex
            ${LARGEINPUT}/ONERA/spa3_0215_rad.ent
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

add_test(ioTvONERAImageStreamWriter ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
                          ${TEMP}/ioImageFileReaderONERAStreamWrite.ent
         otbImageFileWriterStreamingONERAComplex
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
            ${TEMP}/ioImageFileReaderONERAStreamWrite.ent )
set_tests_properties(ioTvONERAImageStreamWriter PROPERTIES DEPENDS ioTvONERAImageFileWriterComplex)
endif()

# -------            otb::MSTARImageIO   ------------------------------
# CanRead()
add_test(ioTuMSTARImageIOCanRead1 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB19896.016)

add_test(ioTuMSTARImageIOCanRead2 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB20010.000)

add_test(ioTvImageFileReaderMSTAR ${IO_TESTS6}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderMSTAR_rescal_chanel1.png
                    ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png
        otbImageFileReaderMSTAR
        ${INPUTDATA}/MSTAR/HB19896.016
        ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageFileReader pour CAI  ------------------------------
# Niveau de Gris
add_test(ioTvImageFileReaderPNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png )

# RGB
add_test(ioTvImageFileReaderRGB_PNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png )

add_test(ioTvImageFileReaderRGB_PNG2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_PNG2TIFF.tiff
                    ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff )

add_test(ioTvImageFileReaderRGB_BSQ2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )

add_test(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

add_test(ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/
        ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )

add_test(ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

# Suppression CAI
# add_test(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS7}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#                     ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#         otbImageFileReaderRGBTest
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )



# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

add_test(ioTvImageFileWriterPNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )

add_test(ioTvImageFileWriterPNG2BSQ ${IO_TESTS7}
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr )

add_test(ioTvImageFileReaderENVI2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
        otbImageFileReaderTest
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png )
set_tests_properties(ioTvImageFileReaderENVI2PNG PROPERTIES DEPENDS ioTvImageFileWriterPNG2BSQ)

add_test(ioTvImageFileReaderPDS2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/pdsImage.img
                           ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif
        otbImageFileReaderTest
        ${INPUTDATA}/pdsImage.img
        ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif)
# RGB

add_test(ioTvImageFileWriterRGB_PNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileWriterRGB_PNG2PNG.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png
        otbImageFileWriterRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png )

add_test(ioTvImageFileWriterRGB_PNG2BSQ ${IO_TESTS7}
        otbImageFileWriterRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr )

add_test(ioTvImageFileReaderRGB_BSQ2PNG_2 ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png
        otbImageFileReaderRGBTest
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png )
set_tests_properties(ioTvImageFileReaderRGB_BSQ2PNG_2 PROPERTIES DEPENDS ioTvImageFileWriterRGB_PNG2BSQ)

# WRITER GDAL
add_test(ioTvGDALImageFileWriterTIF2TIF ${IO_TESTS7}
        otbImageFileWriterRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/poupees.tiff )

# -------            otb::ImageFileReader pour GDAL  ------------------------------
# -------            otb::ImageFileWriter pour GDAL  ------------------------------


add_test(ioTvImageReaderWriterRgbPNG2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png )

#reader writer with box parameter (extract roi)
add_test(ioTvImageReaderWriterRgbPNG2PNGBox ${IO_TESTS7}
  --compare-image ${NOTOL}
  ${BASELINE}/coTvExtractROI_QB.png
  ${TEMP}/coTvExtractROI_QB_Box.png
    otbImageFileReaderRGBTest
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/coTvExtractROI_QB_Box.png?&box=0:0:70:70
  )

add_test(ioTvImageReaderWriterRgbPNG2TIF ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif )
add_test(ioTvImageReaderWriterRgbPNG2ENVI ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees.hdr )
add_test(ioTvImageReaderWriterRgbPNG2JPEG ${IO_TESTS7}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9} ${INPUTDATA}/poupees.png
#                 ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg )

add_test(ioTvImageReaderWriterRgbTIF2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.tif
                     ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.tif
        ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png )
add_test(ioTvImageReaderWriterRgbENVI2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees
                     ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees
        ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png )
add_test(ioTvImageReaderWriterRgbJPEG2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png )
add_test(ioTvImageReaderWriterRgbJPEG2TIF ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if( OTB_USE_CURL )

add_test(coTvCoordinateToNameTest ${IO_TESTS8}
    --compare-ascii ${NOTOL}
         ${BASELINE}/CoordinateToNameTest.txt
         ${TEMP}/CoordinateToNameTest.txt
    otbCoordinateToNameTest
         103.78 1.29
         ${TEMP}/CoordinateToNameTest.txt
)

#this test intentionaly uses the same baseline as the previous one
add_test(coTvCoordinateToNameMultithreadTest ${IO_TESTS8}
    --compare-ascii ${NOTOL}
         ${BASELINE}/CoordinateToNameTest.txt
         ${TEMP}/CoordinateToNameMultithreadTest.txt
    otbCoordinateToNameTest
         103.78 1.29
         ${TEMP}/CoordinateToNameMultithreadTest.txt
         1
)

endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MultiChannelExtractROI   ------------------------------


if(OTB_DATA_USE_LARGEINPUT)
# ---  SPOT4 (CAI)  ---
add_test(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
add_test(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )
endif()

# ---  ENVI  ---
add_test(ioTvMultiChannelROI_ENVI2PNG_2 ${COMMON_TESTS2}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/poupees.hdr
         -uchar ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---


# add_test(ioTvMultiChannelROI_SPOT5BIL2PNG_2 ${COMMON_TESTS2}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         -startX 100 -startY 100 -sizeX 77 -sizeY 300
#         -channels 1 2 3 )

# ---  SPOT5 (CAI)  ---
# ---  Test sur l'image SPOT 5 4 canaux  ---
if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvMultiChannelROI_SPOT52PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

add_test(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
add_test(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
add_test(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
add_test(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
add_test(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
add_test(ioTvMultiChannelROI_IkonosRED2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
                            ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_red_0000000.tif
         ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

add_test(ioTvMultiChannelROI_IkonosGRN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_grn_0000000.tif
         ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

add_test(ioTvMultiChannelROI_IkonosBLU2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_blu_0000000.tif
         ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

add_test(ioTvMultiChannelROI_IkonosNIR2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  LUM ----
add_test(ioTvMultiChannelROI_IkonosNIR2LUM ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300_nogeoinfo.png
                     ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  BSQ ----
add_test(ioTvMultiChannelROI_IkonosNIR2BSQ ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300_nogeoinfo.png
                                ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird PAN  ---
add_test(ioTvMultiChannelROI_QuickbirdPAN2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )

# ---  Quickbird XS  ---
add_test(ioTvMultiChannelROI_QuickbirdXS2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )
endif()
#         -channels 1 2 3)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvOtbImageTestSAR_ERS ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest
         ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
add_test(ioTvOtbImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}
     ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
     ${TEMP}/ioOtbImageTestRadarsat.txt
   otbImageTest
     ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
     ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
add_test(ioTvOtbVectorImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}
     ${TEMP}/ioOtbImageTestRadarsat.txt
     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
   otbVectorImageLegacyTest
     ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
     ${TEMP}/ioOtbVectorImageTestRadarsat.txt)
set_tests_properties(ioTvOtbVectorImageTestRadarsat PROPERTIES DEPENDS ioTvOtbImageTestRadarsat)

# ---  Otb::VectorImage Test  ---
add_test(ioTvOtbVectorImageTestSpot5 ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/SPOT5/TEHERAN/
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)

add_test(ioTvOtbVectorImageTestFORMOSAT2 ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestFORMOSAT2.txt
                     ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/
        ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt)

add_test(ioTvOtbVectorImageTestCOSMOSKYMED ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestCOSMOSKYMED.txt
                     ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5
        ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt)

endif()

add_test(ioTuVectorImageComplexNew ${IO_TESTS9}
  otbVectorImageComplexNew)

add_test(ioTvVectorImageComplexFloatTest ${IO_TESTS9}
  otbVectorImageComplexFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

add_test(ioTvVectorImageComplexDoubleTest ${IO_TESTS9}
  otbVectorImageComplexDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)

add_test(ioTvImageComplexFloatTest ${IO_TESTS9}
  otbImageComplexFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

add_test(ioTvImageComplexDoubleTest ${IO_TESTS9}
  otbImageComplexDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)

add_test(ioTvVectorImageComplexIntoRealFloatTest ${IO_TESTS9}
  otbVectorImageComplexIntoRealFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

add_test(ioTvVectorImageComplexIntoRealDoubleTest ${IO_TESTS9}
  otbVectorImageComplexIntoRealDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)


# ---  otb::VectorImageReadWriteTest  ---
add_test(ioTvVectorImageFileReaderWriterTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
                   ${INPUTDATA}/qb_RoadExtract.img.hdr
                   ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/qb_RoadExtract.img.hdr
        ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
)

add_test(ioTvVImageFileReader_RESOLUTION_0 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/maur_rgb.tif?&resol=0
        ${TEMP}/ioImageFileReader_RESOLUTION_0.tif )

add_test(ioTvVImageFileReader_RESOLUTION_3 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/maur_rgb.tif?&resol=3
        ${TEMP}/ioImageFileReader_RESOLUTION_3.tif )

if(OTB_DATA_USE_LARGEINPUT)
    add_test(ioTvVImageFileReader_RESOLUTION_4 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF?&resol=4
        ${TEMP}/ioImageFileReader_RESOLUTION_4.tif )
endif()

# ---  otb::StreamingShortImageFileWriterTest  ---
add_test(ioTvStreamingShortImageFileWriterTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
                   ${INPUTDATA}/poupees_sub.png
                   ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
        otbStreamingShortImageFileWriterTest
        ${INPUTDATA}/poupees_sub.png
        ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
        0
)

# ---  otb::StreamingShortImageFileWriterTest  ---
add_test(ioTvStreamingShortImageFileWriterStreamedTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
                   ${INPUTDATA}/poupees_sub.png
                   ${TEMP}/ioTvStreamingShortImageFileWriterStreamedTest.png
        otbStreamingShortImageFileWriterTest
        ${INPUTDATA}/poupees_sub.png
        ${TEMP}/ioTvStreamingShortImageFileWriterStreamedTest.png
        1
        4
)


# ---  STREAMING : GDAL  ---
add_test(ioTvStreamingImageFilterGDAL_ENVI ${IO_TESTS9}
#   --compare-image ${EPSILON_9} ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
   --compare-image ${EPSILON_9} ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         )

# ---  STREAMING : GDAL  ---
add_test(ioTvStreamingImageFilterGDAL_TIFF ${IO_TESTS9}
#   --compare-image ${EPSILON_9} ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
#                      ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
         ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         )

# ---  STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         0 # Streaming
         0
         )
# ---  STREAMING  IMAGE   FILE    WRITER  ---
add_test(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
         1 # Streaming
         100
         )



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS10 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# Test otb::StreamingImageFileWriter streaming

# 1. DEFAULT
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2. SetNumberOfDivisionsStrippedStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
         SetNumberOfDivisionsStrippedStreaming
         10
         )
# 3. SetNumberOfDivisionsTiledStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreamingS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
         SetNumberOfDivisionsTiledStreaming
         10
         )
# 4. SetNumberOfLinesStrippedStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
         SetNumberOfLinesStrippedStreaming
         5
         )
# 5. SetAutomaticStrippedStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
         SetAutomaticStrippedStreaming
         10
         )
# 6. SetTileDimensionTiledStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                            ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
         SetTileDimensionTiledStreaming
         32
         )
# 7. SetAutomaticTiledStreaming
add_test(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                            ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
         SetAutomaticTiledStreaming
                                 10
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS11 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------- Metadata writing tests ------------------------

add_test(ioTvImageMetadataWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvImageMetadataWriterHFA.img
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvImageMetadataWriterHFA.img
        )

add_test(ioTvImageMetadataWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/spot5SubWithGcps.tif
                      ${TEMP}/ioTvImageMetadataWriterTIF.tif
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
        ${TEMP}/ioTvImageMetadataWriterTIF.tif
        )


# Commented because gdal does not seems to write the metadata the same way in both formats
#add_test(ioTvImageMetadataWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#                      ${INPUTDATA}/HFAGeoreferenced.img
#                      ${TEMP}/temp.tif
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#        ${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#add_test(ioTvImageMetadataWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#                      ${INPUTDATA}/spot5SubWithGcps.tif
#                      ${TEMP}/temp.img
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#        ${TEMP}/temp.img
#        )


add_test(ioTvImageMetadataStreamingWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
        )

add_test(ioTvImageMetadataStreamingWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/spot5SubWithGcps.tif
                      ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
        ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
        )

# Commented because gdal does not seems to write the metadata the same way in both formats
# Compare metadata indicates a small differences between the ProjRef
#add_test(ioTvImageMetadataStreamingWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#           ${INPUTDATA}/HFAGeoreferenced.img
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_HFA2TIFF.tif
#     otbImageMetadataStreamingFileWriterTest
#       ${INPUTDATA}/HFAGeoreferenced.img
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_HFA2TIFF.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
# Compare metadata indicates that GCP are lost but they are in filename.aux.xml
#add_test(ioTvImageMetadataStreamingWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#           ${INPUTDATA}/spot5SubWithGcps.tif
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_TIFF2HFA.img
#     otbImageMetadataStreamingFileWriterTest
#       ${INPUTDATA}/spot5SubWithGcps.tif
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_TIFF2HFA.img
#        )

add_test(ioTvPipelineMetadataHandlingTest ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvPipelineMetadataHandling.img
        otbPipelineMetadataHandlingTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvPipelineMetadataHandling.img
        )

# Commented out, since this test fails beceause of an ITK bug (added to our bugtracker for tracking)
# MSD: this test doesn't fail today (UFF = Unary Functor Image Filter)
add_test(ioTvPipelineMetadataHandlingWithUFFilterTest ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
           ${INPUTDATA}/HFAGeoreferenced.img
           ${TEMP}/ioTvPipelineMetadataHandlingWithUFFilterTest.img
     otbPipelineMetadataHandlingWithUFFilterTest
       ${INPUTDATA}/HFAGeoreferenced.img
           ${TEMP}/ioTvPipelineMetadataHandlingWithUFFilterTest.img
        )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS12 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ---  otb::DEMToImageGenerator ---
add_test(ioTuDEMToImageGeneratorNew ${IO_TESTS12} otbDEMToImageGeneratorNew )

add_test(ioTvDEMToImageGeneratorTest ${IO_TESTS12}
--compare-image ${EPSILON_5}  ${BASELINE}/ioTvDEMToImageGenerator.tif
                        ${TEMP}/ioTvDEMToImageGenerator.tif
        otbDEMToImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToImageGenerator.tif
        6.5
        45.5
        500
        500
        0.002
        -0.002
        )

# Pech David - Garonne, Toulouse area
add_test(ioTvDEMToImageGeneratorTest1 ${IO_TESTS12}
   --compare-image ${EPSILON_5}
       ${BASELINE}/ioTvDEMToImageGenerator1.tif
       ${TEMP}/ioTvDEMToImageGenerator1.tif
     otbDEMToImageGeneratorTest
       ${INPUTDATA}/DEM/srtm_directory
       ${TEMP}/ioTvDEMToImageGenerator1.tif
       1.437371 43.559045 512 128 0.000032 -0.000024
        )

#Big extend
add_test(ioTvDEMToImageGeneratorTest2 ${IO_TESTS12}
--compare-image ${EPSILON_5}  ${BASELINE}/ioTvDEMToImageGenerator2.tif
                        ${TEMP}/ioTvDEMToImageGenerator2.tif
        otbDEMToImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToImageGenerator2.tif
         -1.8 52 232 422  0.02 -0.018
#          576378.080 5763132.837 232 422 2000 -2000 30 N
# 3.765220920574784 44.224685913433
)

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvDEMToImageGeneratorFromImageTest_SensorModel ${IO_TESTS12}
    --compare-n-images ${EPSILON_5} 2
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_SM.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM.tif
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_SM.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract.tif
      otbDEMToImageGeneratorFromImageTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract.tif
        2600 4300 512 128
)
endif()

add_test(ioTvDEMToImageGeneratorFromImageTest ${IO_TESTS12}
    --compare-image ${EPSILON_5}
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
      otbDEMToImageGeneratorFromImageTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
)

add_test(ioTvDEMToImageGeneratorFromImageTest2 ${IO_TESTS12}
    --compare-n-images ${EPSILON_5} 2
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_Ortho.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM2.tif
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_Ortho.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract2.tif
      otbDEMToImageGeneratorFromImageTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM2.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract2.tif
        10 15 16 64
)

# otb::MapProjectionAdapter (TODO: remove from IO)
add_test(ioTvMapProjectionAdapterTest ${IO_TESTS12}
        --compare-ascii ${EPSILON_9}
            ${BASELINE_FILES}/ioTvMapProjectionAdapterTest.txt
            ${TEMP}/ioTvMapProjectionAdapterTest.txt
        otbMapProjectionAdapterTest
            ${TEMP}/ioTvMapProjectionAdapterTest.txt
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS13 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if(OTB_USE_JPEG2000_TESTING)

# --------------------------------------------------------------------------------
# old test suite:
if(OTB_USE_JPEG2000)
# ---  JPEG2000 can read ---
add_test(ioTuJ2KImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${INPUTDATA}/bretagne.j2k
         )

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTuJP2ImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${LARGEINPUT}/JPEG2000/QB_multicomp.jpx
         )
endif()

endif()

#NOTE: baseline generated by openjpeg
if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvVectorImageFileReaderWriterJP22TIF ${IO_TESTS9}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput
                        ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${LARGEINPUT}/JPEG2000/QB_multicomp_small.jpx
         ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         )


add_test(ioTvImageFileReaderWriterJP22TIF ${IO_TESTS2}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvImageFileReaderWriterJP22TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         otbDoubleImageIOTest
         ${LARGEINPUT}/JPEG2000/QB_multicomp_small.jpx
         ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         )
endif()

# --------------------------------------------------------------------------------
# New test suite:

# Tests to check if we can open JPEG2000 files specifying the resolution factor.
# no access to data only PrintSelf() method.
add_test(ioTvMultiResolutionReading0 ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading0.txt
                             ${TEMP}/ioTvMultiResolutionReading0.txt
      otbVectorImageTest
      ${INPUTDATA}/bretagne.j2k?&resol=0
      ${TEMP}/ioTvMultiResolutionReading0.txt
      )

add_test(ioTvMultiResolutionReading3 ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading3.txt
                             ${TEMP}/ioTvMultiResolutionReading3.txt
      otbVectorImageTest
      ${INPUTDATA}/bretagne.j2k?&resol=3
      ${TEMP}/ioTvMultiResolutionReading3.txt
      )

# Test to check behavior of function JPEG2000reader->GetResolutionInfo()
# which return info about resolution available in the JPEG2000 file.
add_test(ioTvMultiResolutionReadingInfo ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut.txt
                            ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
      otbMultiResolutionReadingInfo
      ${INPUTDATA}/bretagne.j2k
      ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
      )

add_test(ioTvMultiResolutionReadingInfo_TIFF ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff.txt
                            ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
      otbMultiResolutionReadingInfo
      ${INPUTDATA}/maur_rgb.tif
      ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
      )

if(OTB_DATA_USE_LARGEINPUT)
    add_test(ioTvMultiResolutionReadingInfo_TIFF2 ${IO_TESTS13}
      --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
                               ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
          otbMultiResolutionReadingInfo
          ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
          ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
          )
endif()

# Tests which read data in JPEG2000 files.
# Generate a too big output image to be used in baseline repository (useful only for dev)
#add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944.tif
#                     ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944_OUT.tif
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/bretagne.j2k:0
#         ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944_OUT.tif
#         -startX 0 -startY 0 -sizeX 2592 -sizeY 1944
#         -channels 1 2 3 )


# Read an area inside one tile at resolution 0 (jpeg2000 conformance file with
# specific tile size at different resolution).
add_test(ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
            )

# Read an area inside one tile at resolution 4 (jpeg2000 conformance file with
# specific tile size at different resolution).
add_test(ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res4 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=4
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
            )

# Read an area inside one tile at resolution 0 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
add_test(ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/lena512color.jp2?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
            -startX 150 -startY 75 -sizeX 100 -sizeY 50 )

# Read an area inside one tile at resolution 5 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
add_test(ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_res5.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res5_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/lena512color.jp2?&resol=5
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res5_OUT.tif
            )


# Read an area inside one tile at resolution 0
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res0_1 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
            -startX 700 -startY 1000 -sizeX 50 -sizeY 40 )

# Read an area which need to decode 4 tiles at resolution 0
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res0_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
            -startX 1260 -startY 950 -sizeX 40 -sizeY 20 )

# Read all the image with a resolution 3
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res3 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=3
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif )

# Read area inside one tile at resolution 3
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res3_1 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=3
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
           -startX 90 -startY 70 -sizeX 20 -sizeY 30 )

# Read an area which need to decode 4 tiles at resolution 3
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res3_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=3
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
           -startX 40 -startY 30 -sizeX 80 -sizeY 60 )

# Read all the image with a resolution 5
add_test(ioTvMultiChannelROI_JPEG2000_2_TIF_res5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=5
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5_OUT.tif )

if(OTB_USE_JPEG2000)
# --------------------------------------------------------------------------------
# Test the possibility to modify the size of cache used to reduce the decoding operations
# O tile in the cache (500K)
add_test(ioTvJPEG2000ImageIO_CacheSize_500 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
         500)

# 1 tile in the cache (1000K)
add_test(ioTvJPEG2000ImageIO_CacheSize_1000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
         1000)

# 5 tiles in the cache (5000K)
add_test(ioTvJPEG2000ImageIO_CacheSize_5000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
         5000)

# 135Mb in the cache (set by otbIFR for JPEG2000)
add_test(ioTvJPEG2000ImageIO_CacheSize_25000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif)
endif()

# --------------------------------------------------------------------------------
# Special Case (deactivated by MSD, OpenJPEG now support the larges files so this tests is not relevant
# Between tiles 328 and 329, the codestream offset crosses the 2**31-1 boundary (max of 32 bit signed integer)
# This reveals a problem on 32 bits systems since fseek takes an integer
#if(OTB_DATA_USE_LARGEINPUT)
#
#  add_test(ioTvVectorImageFileReaderWriterJP22TIF_PLEIADES-PRE-R4C1-tile328-329 ${COMMON_TESTS2}
#           --without-threads
#           --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_PLEIADES-PRE-R4C1-tile328-329.tif
#                                         ${TEMP}/ioTvJP22TIF_Extract_PLEIADES-PRE-R4C1-tile328-329_OUT.tif
#           otbMultiChannelExtractROI
#              -ushort ${LARGEINPUT}/PLEIADES-PRE/VERSION2/IMG_PHR_001/IMG_R4C1.JP2:5
#              -ushort ${TEMP}/ioTvJP22TIF_Extract_PLEIADES-PRE-R4C1-tile328-329_OUT.tif
#              -startX 1330 -startY 928 -sizeX 32 -sizeY 32
#              -time
#           )
#
#endif()

# --------------------------------------------------------------------------------
# Tests about special data (larges files, CNES files, SPOT files)
if(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)
  set( GenericTestPHR_TESTNB 0)

foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
  list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
  string(REGEX MATCH "TIFF" PHR_TIFF_FILE ${current_type})

  if((NOT PHR_TIFF_FILE) AND OTB_USE_JPEG2000)
    # CanRead
    add_test(ioTuJ2KImageIOCanRead_${current_type} ${IO_TESTS13}
             otbJPEG2000ImageIOTestCanRead
                ${current_file}
            )
  endif()

    # Read info from the input file
    add_test(ioTvVectorImageReadingInfo_${current_type} ${IO_TESTS13}
             --ignore-order --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}.txt
                                       ${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
             --ignore-lines-with 3 CacheSizeInBytes Driver Corner[ 
             otbVectorImageTest
                ${current_file}?&resol=0
                ${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
             )

  if(NOT PHR_TIFF_FILE)
    # Read info from the input file (res=5)
    add_test(ioTvVectorImageReadingInfo_${current_type}_res5 ${IO_TESTS13}
             --ignore-order --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}_res5.txt
                                       ${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
             --ignore-lines-with 3 CacheSizeInBytes Driver Corner[
             otbVectorImageTest
                ${current_file}?&resol=5
                ${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
             )
  endif()

  if(NOT PHR_TIFF_FILE)
    # Extract resolution of input file
    add_test(ioTvMultiResolutionReadingInfo_${current_type} ${IO_TESTS13}
             --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfo_${current_type}.txt
                                       ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
             otbMultiResolutionReadingInfo
                ${current_file}
                ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
            )


    # Decode an area inside one tile
    add_test(ioTvReaderWriterJP22TIF_${current_type}_1 ${COMMON_TESTS2}
	         --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_1.tif
                                           ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
             otbMultiChannelExtractROI
                -ushort ${current_file}
                -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
                -startX 256 -startY 512 -sizeX 256 -sizeY 128
                -time
            )

    # Decode an area which need 4 tiles
    add_test(ioTvReaderWriterJP22TIF_${current_type}_2 ${COMMON_TESTS2}
             --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_2.tif
                                           ${TEMP}/ioTvJP22TIF_Extract_${current_type}_2_OUT.tif
             otbMultiChannelExtractROI
                -ushort ${current_file}
                -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_2_OUT.tif
                -startX 2000 -startY 2000 -sizeX 256 -sizeY 128
                -time
            )

    # Decode the whole image with the lowest resolution (res=5)
    # (take only one channel to put the file in baseline repository)
    #write only a subpart of the output image using the extended parameter "box"
    add_test(ioTvReaderWriterJP22TIF_${current_type}_res5 ${COMMON_TESTS2}
             --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_res5.tif
                                           ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif
             otbMultiChannelExtractROI
                -ushort ${current_file}?resol=5
                -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif?&box=0:0:128:128
                -channels 1
                -time
         )
  endif()

endforeach()

endif()

endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS14 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(ioTvGDALDriverDoubleWritingTest ${IO_TESTS14}
                --compare-n-images ${EPSILON_9} 4
                        ${INPUTDATA}/poupees.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage1.tif
                        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage2.tif
                        ${INPUTDATA}/poupees.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage3.tif
                        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage4.tif
                otbGDALDriverDoubleWritingTest
                        ${INPUTDATA}/poupees.tif
                        ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage1.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage2.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage3.tif
                        ${TEMP}/ioGdalDriverDoubleWritingTestImage4.tif
)

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvReadingComplexDataIntoComplexImage ${IO_TESTS14}
                otbReadingComplexDataIntoComplexImageTest
                ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
                1000 1000
)

add_test(ioTvWritingComplexDataWithComplexImage ${IO_TESTS14}
--compare-image ${EPSILON_9}
                ${BASELINE}/ioWritingComplexDataWithComplexImageOutput.tif
                ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
                otbWritingComplexDataWithComplexImageTest
                ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
                ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
                1000 1000
                500  500
)
endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS15 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_test(ioTuVectorDataIOFactory ${IO_TESTS15}
         otbVectorDataIOFactory
         ${INPUTDATA}/ToulouseRoad-examples.shp )

add_test(ioTuOGRVectorDataIO ${IO_TESTS15}
        otbOGRVectorDataIONew )

add_test(ioTuVectorDataSourceNew ${IO_TESTS15}
        otbVectorDataSourceNew )

add_test(ioTuVectorDataFileReaderNew ${IO_TESTS15}
        otbVectorDataFileReaderNew )

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvSHPVectorDataFileReader ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader.txt
                        ${TEMP}/ioSHPVectorDataFileReader.txt
        otbVectorDataFileReader
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
        ${TEMP}/ioSHPVectorDataFileReader.txt
)
endif()

add_test(ioTvSHPVectorDataFileReader2 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader2.txt
                        ${TEMP}/ioSHPVectorDataFileReader2.txt
        otbVectorDataFileReader
        ${INPUTDATA}/ToulouseRoad-examples.shp
        ${TEMP}/ioSHPVectorDataFileReader2.txt
)

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvSHPVectorDataFileReader3 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader3.txt
                        ${TEMP}/ioSHPVectorDataFileReader3.txt
        otbVectorDataFileReader
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/ioSHPVectorDataFileReader3.txt
)
endif()

add_test(ioTvKMLVectorDataFileReader ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader.txt
                        ${TEMP}/ioTvKMLVectorDataFileReader.txt
        otbVectorDataFileReader
        ${INPUTDATA}/lines.kml
        ${TEMP}/ioTvKMLVectorDataFileReader.txt
)

add_test(ioTvKMLVectorDataFileReader2 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader2.txt
                        ${TEMP}/ioTvKMLVectorDataFileReader2.txt
        otbVectorDataFileReader
        ${INPUTDATA}/multigeometry.kml
        ${TEMP}/ioTvKMLVectorDataFileReader2.txt
)

add_test(ioTuVectorDataFileWriterNew ${IO_TESTS15}
        otbVectorDataFileWriterNew )


add_test(ioTvVectorDataFileWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterOutput.shp
        otbVectorDataFileWriter
        ${TEMP}/ioTvVectorDataFileWriterOutput.shp
)

add_test(ioTvVectorDataFileWriterTwice ${IO_TESTS15}
        otbVectorDataFileWriter
        ${TEMP}/ioTvVectorDataFileWriterOutput.shp
        )
set_tests_properties(ioTvVectorDataFileWriterTwice PROPERTIES DEPENDS ioTvVectorDataFileWriter)

add_test(ioTvVectorDataFileWriterPolygons ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterPolygonsOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
        otbVectorDataFileWriterPolygons
    ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
    )

add_test(ioTvVectorDataFileWriterMultiPolygons ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
        otbVectorDataFileWriterMultiPolygons
    ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
    )


if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvVectorDataFileReaderWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileReaderWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
        otbVectorDataFileReaderWriter
            ${LARGEINPUT}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
            ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
        )
endif()

add_test(ioTvVectorDataFileGeoReaderWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/ToulouseRoad-examples.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
        )

add_test(ioTvVectorDataFileGeoReaderWriter2 ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/Capitole-Shadows.kml
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
        )

add_test(ioTvKMLVectorDataFileGeoReaderWriter ${IO_TESTS15}
        --compare-ascii  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/ToulouseRoad-examples.shp
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
        )

add_test(ioTvKMLVectorDataFileGeoReaderWriter2 ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/Capitole-Shadows.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
        )

# CanRead OGR GML()
add_test(ioTuOGRVectorDataIOTestCanReadGML ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/adressPoint.gml)

# CanRead OGR MapInfo()
add_test(ioTuOGRVectorDataIOTestCanReadMapInfo ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

# CanWrite OGR GML()
add_test(ioTuOGRVectorDataIOCanWriteGML ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/adressPoint.gml)

# CanWrite OGR MapInfo()
add_test(ioTuOGRVectorDataIOCanWriteMapInfo ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

add_test(ioTuKMLVectorDataIO ${IO_TESTS15}
        otbKMLVectorDataIONew )

# CanRead()
add_test(ioTuKMLVectorDataIOCanRead ${IO_TESTS15}
        otbKMLVectorDataIOTestCanRead
        ${INPUTDATA}/MODIS20040214212340.kml)

# CanWrite()
add_test(ioTuKMLVectorDataIOCanWrite ${IO_TESTS15}
        otbKMLVectorDataIOTestCanWrite
        ${TEMP}/MODIS20040214212340.kml)

# Read a MODIS kml file and then write it
add_test(ioTvKMLVectorDataIOFileReaderExample ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/MODIS20040214212340.kml
        ${TEMP}/MODIS20040214212340.kml)

# Read a multigeometry example file and then write it
add_test(ioTvKMLVectorDataIOFileReaderMultiGeom ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/multigeometry.kml
        ${TEMP}/multigeometry.kml)

# Read a complex line example file and then write it
add_test(ioTvKMLVectorDataIOFileReaderLines ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/lines.kml
        ${TEMP}/lines.kml)

add_test(ioTuVectorDataKeywordlistNew ${IO_TESTS15}
        otbVectorDataKeywordlistNew
         )

add_test(ioTvVectorDataKeywordlist ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}
            ${BASELINE_FILES}/ioTuVectorDataKeywordlist.txt
            ${TEMP}/ioTuVectorDataKeywordlist.txt
        otbVectorDataKeywordlist
            ${INPUTDATA}/waterways.shp
            ${TEMP}/ioTuVectorDataKeywordlist.txt
         )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS16 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ---  RAD  ---

if(OTB_DATA_USE_LARGEINPUT)

# Can read a RAD format (ci2)
add_test(ioTuRADImageIOCanRead_RADCI2 ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${LARGEINPUT}/RAD_CI2/3017_HH.rad)

# Convert a ci2.RAD file into ENVI format
add_test(ioTvImageFileReaderRADCI22ENVI ${IO_TESTS16}
--compare-image ${EPSILON_9}
        ${BASELINE}/ioImageFileReaderRADCI22ENVI.hdr
        ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr
        otbImageFileReaderRADComplexFloatExtract
        ${LARGEINPUT}/RAD_CI2/3017_HH.rad
        ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr )

endif()


# Can read a RAD format (cr4)
add_test(ioTuRADImageIOCanRead_RADCR4sngl ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_image.rad )

# Can read a multi-image RAD format (cr4)${EXAMPLEDATA}
add_test(ioTuRADImageIOCanRead_RADCR4mult ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_multimage.rad )

# Convert a cr4.RAD file into tif
add_test(ioTvImageFileReaderRADCR42TIF ${IO_TESTS16}
        otbImageFileReaderRADComplexFloat
        ${INPUTDATA}/RADCR4_image.rad
        ${TEMP}/ioImageFileReaderRAD2TIF.tif )

# Turn a ci4.RAD file into cr4.file (because of float type)
add_test(ioTvImageFileReaderCI42CR4 ${IO_TESTS16}
        otbImageFileReaderRADComplexFloat
        ${INPUTDATA}/RADCI4_image.rad
        ${TEMP}/ioImageFileReaderCI42CR4.rad )

# Turn a ci2.RAD file into cr4.file (because of float type)
#add_test(ioTvImageFileReaderCI22CR4 ${IO_TESTS16}
#        otbImageFileReaderRADComplexFloat
#        /home3/mathieu/guillaume/CD_CNES/RAD_CI2/3017_HH.rad
#        ${TEMP}/ioImageFileReaderCI22CR4.rad )

# Copy cr4.RAD file
add_test(ioTvImageFileReaderCR42CR8 ${IO_TESTS16}
        otbImageFileReaderRADComplexDouble
        ${INPUTDATA}/RADCR4_image.rad
        ${TEMP}/ioImageFileReaderCR42CR8.rad )

# Convert tif into a RAD file (ci4)
add_test(ioTvImageFileReaderTIF2RADCI4 ${IO_TESTS16}
        otbImageFileReaderRADComplexInt
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${TEMP}/ioImageFileReaderTIF2RADCI4.rad )

# Convert png into a RAD file (pha) -> sortie n'est pas lisible
#add_test(ioTvImageFileReaderTIF2RADPHA ${IO_TESTS16}
#        otbImageFileReaderRADInt
#        ${INPUTDATA}/rcc8_mire1.png
#        ${TEMP}/ioImageFileReaderPNG2RADPHA.rad )

add_test(ioTvImageSeriesFileReader ${IO_TESTS16}
        --compare-n-images ${EPSILON_9} 2
            ${BASELINE}/ioTvImageSeries-0.png
            ${TEMP}/ioTvImageSeries-0.png
            ${BASELINE}/ioTvImageSeries-1.png
            ${TEMP}/ioTvImageSeries-1.png
        otbImageSeriesFileReader
            ${EXAMPLEDATA}/GomaAvantApres
            ${TEMP}/ioTvImageSeries.png
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS18 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


add_test(ioTuSpotImageMetadataInterfaceNew ${IO_TESTS18}
        otbSpotImageMetadataInterfaceNew
)

add_test(ioTuFormosatImageMetadataInterfaceNew ${IO_TESTS18}
        otbFormosatImageMetadataInterfaceNew
)

add_test(ioTuIkonosImageMetadataInterfaceNew ${IO_TESTS18}
        otbIkonosImageMetadataInterfaceNew
)

add_test(ioTuQuickBirdImageMetadataInterfaceNew ${IO_TESTS18}
        otbQuickBirdImageMetadataInterfaceNew
)

add_test(ioTuWorldView2ImageMetadataInterfaceNew ${IO_TESTS18}
        otbWorldView2ImageMetadataInterfaceNew
)

add_test(ioTuTerraSarImageMetadataInterfaceNew ${IO_TESTS18}
        otbTerraSarImageMetadataInterfaceNew
)

add_test(ioTuPleiadesImageMetadataInterfaceNew ${IO_TESTS18}
        otbPleiadesImageMetadataInterfaceNew
)

add_test(ioTuSpot6ImageMetadataInterfaceNew ${IO_TESTS18}
        otbSpot6ImageMetadataInterfaceNew
)

if(OTB_DATA_USE_LARGEINPUT)

#------------ ImageMetadataInterfaceBaseTest ---------------

add_test(ioTvImageMetadataInterfaceBaseTest_QB ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QB.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_QBCevennes ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_QBMUL ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBMUL.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_WV2Pan ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_WV2Mul ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_IKONOS ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_IKONOS.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif
        ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_SPOT ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_SPOT.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
)

# PHR Data
set( GenericTestPHR_TESTNB 0)

foreach( current_file ${GenericTestPHR_DATA_INPUTS} )
  list(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  math(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")

  add_test(ioTvImageMetadataInterfaceBaseTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_${current_type}.txt
                                    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
           otbImageMetadataInterfaceBaseTest
              ${current_file}
              ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
          )

  add_test(ioTvOpticalImageMetadataInterfaceTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_${current_type}.txt
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

  add_test(ioTvImageMetadataInterfaceBaseTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_${current_type}.txt
                                    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
           otbImageMetadataInterfaceBaseTest
              ${current_file}
              ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
          )

  add_test(ioTvOpticalImageMetadataInterfaceTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_${current_type}.txt
                                    ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
           otbOpticalImageMetadataInterfaceTest
              ${current_file}
              ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
          )

  endforeach()

add_test(ioTvImageMetadataInterfaceBaseTest_FORMOSAT2 ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_TSX1dims ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_TSX1TORONTO ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingHHCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingVVCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
)
add_test(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingXmlFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
)


#------------ OpticalImageMetadataInterfaceTest ---------------
add_test(ioTvOpticalImageMetadataInterfaceTest_QB ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QB.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_QBCevennes ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_QBMUL ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBMUL.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_WV2Pan ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_WV2Mul ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_IKONOS ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_IKONOS.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif
        ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
)
add_test(ioTvOpticalImageMetadataInterfaceTest_SPOT ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_SPOT.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
)

add_test(ioTvOpticalImageMetadataInterfaceTest_FORMOSAT2 ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
)

#------------ SarImageMetadataInterfaceTest ---------------
add_test(ioTvSarImageMetadataInterfaceTest_TSX1dims ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1dims.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
)
add_test(ioTvSarImageMetadataInterfaceTest_TSX1TORONTO ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
)
add_test(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingHHCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
)
add_test(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingVVCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
)
add_test(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingXmlFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
)


endif()

add_test(ioTvImageMetadataInterfaceTest ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterface.txt
                         ${TEMP}/ioTvImageMetadataInterface.txt
        otbImageMetadataInterfaceTest2
        ${EXAMPLEDATA}/qb_RoadExtract.tif
        ${TEMP}/ioTvImageMetadataInterface.txt
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS19 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
if(OTB_USE_CURL)

add_test(ioTvImageFileReaderServerNameHTTPTest ${IO_TESTS19}
        otbImageFileReaderServerName
        http://tile.openstreetmap.org/)
set_property(TEST ioTvImageFileReaderServerNameHTTPTest PROPERTY RESOURCE_LOCK web_access)

add_test(ioTvImageFileReaderServerNameOTBTest ${IO_TESTS19}
        otbImageFileReaderServerName
        ${OTB_DATA_ROOT}/Examples/osmfile.otb)
set_property(TEST ioTvImageFileReaderServerNameOTBTest PROPERTY RESOURCE_LOCK web_access)

# We do not perform regression testing here because images on server might change
add_test(ioTvTileMapImageIOFile ${IO_TESTS19}
  otbTileMapImageIOTest
    ${OTB_DATA_ROOT}/Examples/osmfile.otb
    ${TEMP}/ioTvTileMapImageIOFileOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)
set_property(TEST ioTvTileMapImageIOFile PROPERTY RESOURCE_LOCK web_access)

add_test(ioTvTileMapImageIOWeb ${IO_TESTS19}
  otbTileMapImageIOTest
    http://tile.openstreetmap.org/
    ${TEMP}/ioTvTileMapImageIOWebOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)
set_property(TEST ioTvTileMapImageIOWeb PROPERTY RESOURCE_LOCK web_access)

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvTileMapWriter ${IO_TESTS19}
    otbTileMapWriter
       ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
#       ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
       ${OTB_DATA_ROOT}/Input/DEM/srtm_directory
       ${TEMP}
       500 # extract index[0]
       500 # extract index[1]
       200 # extract size[0]
       200 # extract size[1]
)
endif()



# OSM Data To VectorData Generator

add_test(ioTuOSMToVectorDataGenerator ${IO_TESTS19}
  otbOSMToVectorDataGeneratorNew
)

add_test(ioTvOSMToVectorDataGeneratorTest ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterTest_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
  otbOSMToVectorDataGeneratorTest
  ${INPUTDATA}/qb_road.osm
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
)
set_property(TEST ioTvImageFileReaderServerNameHTTPTest PROPERTY RESOURCE_LOCK web_access)

add_test(ioTvOSMToVectorDataGeneratorByName ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterByName_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
  otbOSMToVectorDataGeneratorByName
  -osm ${INPUTDATA}/qb_road.osm
  -vd ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
)
set_property(TEST ioTvOSMToVectorDataGeneratorByName PROPERTY RESOURCE_LOCK web_access)

# ImageToOSMVectorData
add_test(ioTuImageToOSMVectorDataGeneratorNew ${IO_TESTS19}
  otbImageToOSMVectorDataGeneratorNew
)

add_test(ioTvImageToOSMVectorDataGenerator ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
    ${BASELINE_FILES}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
    ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  otbImageToOSMVectorDataGenerator
  -in  ${EXAMPLEDATA}/qb_RoadExtract.tif
  -osm ${INPUTDATA}/qb_road.osm
  -out ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  -key highvay,motorway
)
set_property(TEST ioTvImageToOSMVectorDataGenerator PROPERTY RESOURCE_LOCK web_access)

endif()

#----------------------------------------------------------------------------------

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS20 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

if(OTB_DATA_USE_LARGEINPUT)
add_test(ioTvImageFileReaderTestSensorPixelValueQB ${IO_TESTS20}
otbImageFileReaderTestSensorPixelValue
       ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
       ${BASELINE_FILES}/ioTvSensorsPixelValue.ini
       OPTIC
       QB
       PAN
       1
       1024
       1024
)

add_test(ioTvImageFileReaderTestSensorPixelValueIKONOS ${IO_TESTS20}
otbImageFileReaderTestSensorPixelValue
       ${LARGEINPUT}/IKONOS/PARIS/po_79039_pan_0000000.tif
       ${BASELINE_FILES}/ioTvSensorsPixelValue.ini
       OPTIC
       IKONOS
       PAN
       1
       1024
       1024
)

endif()

add_test(ioTuDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbDefaultImageMetadataInterfaceFactoryNew )

add_test(ioTuDefaultImageMetadataInterface ${IO_TESTS20}
        otbDefaultImageMetadataInterface )

add_test(ioTuOpticalDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterfaceFactoryNew )

add_test(ioTuOpticalDefaultImageMetadataInterfaceNew ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterfaceNew )

add_test(ioTuOpticalDefaultImageMetadataInterface ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterface )

add_test(ioTuSarDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbSarDefaultImageMetadataInterfaceFactoryNew )

add_test(ioTuSarDefaultImageMetadataInterfaceNew ${IO_TESTS20}
        otbSarDefaultImageMetadataInterfaceNew )

add_test(ioTuSarDefaultImageMetadataInterface ${IO_TESTS20}
        otbSarDefaultImageMetadataInterface )

add_test(ioTuScalarBufferToImageFileWriter ${IO_TESTS20}
        otbScalarBufferToImageFileWriterNew )

add_test(ioTvScalarBufferToImageFileWriterTest ${IO_TESTS20}
  --compare-image ${NOTOL}
         ${BASELINE}/ioTvScalarBufferToImageFileWriterTest.tif
         ${TEMP}/ioTvScalarBufferToImageFileWriterTest.tif
  otbScalarBufferToImageFileWriterTest
                4
                50
                75
         ${TEMP}/ioTvScalarBufferToImageFileWriterTest.tif
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS21 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

add_test(ioTuMapFileProductWriterNew ${IO_TESTS21}
  otbMapFileProductWriterNew
)

add_test(ioTvMapFileProductWriter ${IO_TESTS21}
  otbMapFileProductWriter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/mapserver/toulouseExtract.map
  ${TEMP}/mapserver/result               # Shape index path
  http://localhost/cgi-bin/mapserv # cgi executable path
  256
)

add_test(ioTuKmzProductWriterNew ${IO_TESTS21}
  otbKmzProductWriterNew
)

add_test(ioTvKmzProductWriterWithGCP ${IO_TESTS21}
  otbKmzProductWriter
  ${INPUTDATA}/Haiti1.jpeg
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTuHaitiKmzProduct.kmz
  1980.000000 1986.000000 -72.351257 18.555075 0.000000
  1963.000000 1944.000000 -72.352051 18.556946 0.000000
  2078.000000 1566.000000 -72.346130 18.574358 0.000000
  2154.000000 1430.000000 -72.342377 18.580441 0.000000
  2432.000000 1408.000000 -72.328964 18.581335 0.000000
  2558.000000 934.000000 -72.322571 18.602974 0.000000
  3492.000000 1397.000000 -72.278343 18.580887 0.000000
  2890.000000 1439.000000 -72.306908 18.579464 0.000000
  2098.000000 2469.000000 -72.345741 18.532839 0.000000
  1941.000000 2473.000000 -72.353317 18.532595 0.00000
  1644.000000 2409.000000 -72.367691 18.535810 0.000000
  1440.000000 2448.000000 -72.377457 18.534243 0.000000
  687.000000 2005.000000 -72.413315 18.554974 0.000000
  741.000000 2366.000000 -72.410934 18.538496 0.000000
  855.000000 2802.000000 -72.405739 18.518456 0.000000
  1936.000000 2855.000000 -72.353920 18.515240 0.000000
  3338.000000 2824.000000 -72.286240 18.515160 0.000000
  3715.000000 1772.000000 -72.267570 18.563681 0.000000
  3676.000000 1580.000000 -72.269203 18.572529 0.000000
  3395.000000 931.000000 -72.282402 18.602425 0.000000
  3426.000000 856.000000 -72.280792 18.605862 0.000000
  2822.000000 707.000000 -72.309692 18.613203 0.000000
  1891.000000 360.000000 -72.354301 18.629776 0.000000
  3989.000000 2639.000000 -72.255020 18.523664 0.000000
  417.000000 2317.000000 -72.426552 18.540876 0.000000
  )

add_test(ioTvKmzProductWriterWithGCP_Logo_Legend ${IO_TESTS21}
  otbKmzProductWriterWithLogoAndLegend
  -in ${INPUTDATA}/Haiti1.jpeg
  -lo ${INPUTDATA}/cnes.png
  -le ${INPUTDATA}/otb_big.png
  -dem ${INPUTDATA}/DEM/srtm_directory
  -kmz ${TEMP}/ioTuHaitiKmzProductWithLogoAndLegend.kmz
  -gcp  1980.000000 1986.000000 -72.351257 18.555075 0.000000
  1963.000000 1944.000000 -72.352051 18.556946 0.000000
  2078.000000 1566.000000 -72.346130 18.574358 0.000000
  2154.000000 1430.000000 -72.342377 18.580441 0.000000
  2432.000000 1408.000000 -72.328964 18.581335 0.000000
  2558.000000 934.000000 -72.322571 18.602974 0.000000
  3492.000000 1397.000000 -72.278343 18.580887 0.000000
  2890.000000 1439.000000 -72.306908 18.579464 0.000000
  2098.000000 2469.000000 -72.345741 18.532839 0.000000
  1941.000000 2473.000000 -72.353317 18.532595 0.00000
  1644.000000 2409.000000 -72.367691 18.535810 0.000000
  1440.000000 2448.000000 -72.377457 18.534243 0.000000
  687.000000 2005.000000 -72.413315 18.554974 0.000000
  741.000000 2366.000000 -72.410934 18.538496 0.000000
  855.000000 2802.000000 -72.405739 18.518456 0.000000
  1936.000000 2855.000000 -72.353920 18.515240 0.000000
  3338.000000 2824.000000 -72.286240 18.515160 0.000000
  3715.000000 1772.000000 -72.267570 18.563681 0.000000
  3676.000000 1580.000000 -72.269203 18.572529 0.000000
  3395.000000 931.000000 -72.282402 18.602425 0.000000
  3426.000000 856.000000 -72.280792 18.605862 0.000000
  2822.000000 707.000000 -72.309692 18.613203 0.000000
  1891.000000 360.000000 -72.354301 18.629776 0.000000
  3989.000000 2639.000000 -72.255020 18.523664 0.000000
  417.000000 2317.000000 -72.426552 18.540876 0.000000
  )
set_tests_properties(ioTvKmzProductWriterWithGCP_Logo_Legend
  PROPERTIES DEPENDS ioTvKmzProductWriterWithGCP)


add_test(ioTuTileMapImageIOHelperNew ${IO_TESTS21}
  otbTileMapImageIOHelperNew
)

add_test(ioTvTileMapImageIOHelperTest ${IO_TESTS21}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTileMapImageIOHelperTest.txt
                         ${TEMP}/ioTvTileMapImageIOHelperTest.txt
         otbTileMapImageIOHelperTest
              ${TEMP}/ioTvTileMapImageIOHelperTest.txt
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS22 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Tests about multidataset file like hdf4 or hdf5 (hdf, hdf5, hdf4, h5, h4, he5, he4)

if(CHECK_GDAL_BUILT_WITH_HDF)

if(CHECK_HDF4OPEN_SYMBOL)
# Tests to check if we can open hdf files using a otb::VectorImage specifying the sub-dataset index.
# no access to data only PrintSelf() method.
add_test(ioTvMultiDatasetReading1 ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading1.txt
                             ${TEMP}/ioTvMultiDatasetReading1.txt
      otbVectorImageTest
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=0
      ${TEMP}/ioTvMultiDatasetReading1.txt
      )
add_test(ioTvMultiDatasetReading2 ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading2.txt
                             ${TEMP}/ioTvMultiDatasetReading2.txt
      otbVectorImageTest
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
      ${TEMP}/ioTvMultiDatasetReading2.txt
      )

# Test to check behavior of function readerGDAL->GetSubDatasetInfo() which return info about subdataset.
add_test(ioTvMultiDatasetReadingInfo ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReadingInfo.txt
                             ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
      otbMultiDatasetReadingInfo
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf
      ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
      )

# Tests which read data in HDF files.
add_test(ioTvMultiChannelROI_HDF4_2_TIF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1.tif
                     ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
         otbMultiChannelExtractROI
         ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
         ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
         -startX 1400 -startY 3700 -sizeX 100 -sizeY 150
         -channels 1 )
endif()

add_test(ioTvMultiChannelROI_HDF5_2_TIF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1.tif
                     ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
         otbMultiChannelExtractROI
         ${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5?&sdataidx=3
         ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
         -startX 100 -startY 20 -sizeX 200 -sizeY 100
         -channels 1 )

endif()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests23 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Tests about complex reading

set(INPUTFILE_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(READING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(NBBANDS_LIST "2" "3" "4")

foreach(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

  foreach(READING_PIXELTYPE ${READING_PIXELTYPES_LIST})

    add_test(ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandScalarToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    add_test(ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandScalarToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    add_test(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageScalar${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToImageScalar${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    add_test(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    add_test(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageScalar${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToVectorImageScalar${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    add_test(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    # Weird case : only the first 2 bands used
    add_test(ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMultibandScalarToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    # Weird case : one scalar band -> one complex band with imag=0
    # but interleaving real and imaginary parts can only work for an even nb of bands
    add_test(ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMultibandScalarToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    foreach(NBBANDS ${NBBANDS_LIST})

      # Weird case : need specifications to write the test
      # add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageScalar${READING_PIXELTYPE}
      #          ${IO_TESTS23}
      #          otbMultibandComplexToImageScalar${READING_PIXELTYPE}
      #          ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      # Size: 100x100 is to large to store pixel values coded as short.
      if( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageScalar${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageScalar${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      else()

        add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageScalar${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageScalar${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

      endif()

      # Weird case : need specifications to write the test
      # [missing impl]
      # add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageComplex${READING_PIXELTYPE})

      # Size: 100x100 is to large to store pixel values coded as short.
      if( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageComplex${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageComplex${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      else()

        add_test(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageComplex${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageComplex${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

      endif()

    endforeach()

  endforeach()
endforeach()


# Tests with GDAL only to read complex data

#set(INPUTFILE_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(INPUTFILE_PIXELTYPES_LIST "Float" "Double")

foreach(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

add_test(ioTvGDALReadPxlComplex${INPUTFILE_PIXELTYPE} ${IO_TESTS23}
         otbGDALReadPxlComplex${INPUTFILE_PIXELTYPE}
         ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_3bands.tif
         1 5 10 2) #old file hdr sans extesions

endforeach()

# Tests about writing Scalar / Complex data

set(DATA_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
set(WRITING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")

foreach(DATA_PIXELTYPE ${DATA_PIXELTYPES_LIST})

  add_test(ioTvWriteImageScalar${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
           otbImageScalarFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
              100
           )

  add_test(ioTvWriteImageComplex${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
           otbImageComplexFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
              100
           )

  foreach(NBBANDS ${NBBANDS_LIST})
    if( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      add_test(ioTvWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

      add_test(ioTvWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

    else()

      add_test(ioTvWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
             ${IO_TESTS23}
             --compare-image ${NOTOL}
                ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
             otbVectorImageFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                50
                ${NBBANDS}
             )

      add_test(ioTvWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
               otbVectorImageFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  50
                  ${NBBANDS}
               )

    endif()
  endforeach()

############
# Tests about Streaming writing Scalar / Complex data
  add_test(ioTvStreamingWriteImageScalar${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
           otbImageScalarStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
              100
           )

  add_test(ioTvStreamingWriteImageComplex${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
           otbImageComplexStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
              100
           )

  foreach(NBBANDS ${NBBANDS_LIST})
    if( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      add_test(ioTvStreamingWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageStreamingFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS23 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Tests about relative sepctral responses from file or OpticalMetadataInterface

add_test(raTuSpectralSensitivityReaderNew ${IO_TESTS23}
     otbSpectralSensitivityReaderNew
)

add_test(raTvSpectralSensitivityReaderTest ${IO_TESTS23}
     --compare-ascii ${EPSILON_5}
        ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
        ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
     --ignore-lines-with 2 DataPath FileName
     otbSpectralSensitivityReaderTest
        ${INPUTDATA}/Radiometry
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
)

add_test(raTvSpectralSensitivityReaderGenericTest ${IO_TESTS23}
     --compare-ascii ${EPSILON_5}
      ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
        ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
     --ignore-lines-with 2 DataPath FileName
     otbSpectralSensitivityReaderGenericTest
    ${INPUTDATA}/Radiometry/SPOT5/HRG1/rep6S.dat
    ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
)

if(OTB_DATA_USE_LARGEINPUT)
  add_test(raTvSpectralSensitivityReaderSpot4Test ${IO_TESTS23}
    --compare-ascii ${EPSILON_5}
    ${BASELINE_FILES}/raTvSpectralSensitivityReaderSpot4Test.txt
    ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
    --ignore-lines-with 2 DataPath FileName
    otbSpectralSensitivityReaderGenericTest
    ${INPUTDATA}/Radiometry/SPOT4/HRVIR1/rep6S.dat
    ${LARGEINPUT}/SPOT4/ROMANIA/raTvS4_20020518_Reflectance.img
    ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
    )
endif()

      add_test(ioTvStreamingWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageStreamingFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

    else()

      add_test(ioTvStreamingWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
             ${IO_TESTS23}
             --compare-image ${NOTOL}
                ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
             otbVectorImageStreamingFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                50
                ${NBBANDS}
             )


add_test(raTuFilterFunctionValuesNew ${IO_TESTS23}
     otbFilterFunctionValuesNew
)

add_test(raTuWavelengthSpectralBandsNew ${IO_TESTS23}
     otbWavelengthSpectralBandsNew
)

      add_test(ioTvStreamingWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
               otbVectorImageStreamingFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  50
                  ${NBBANDS}
               )

    endif()

  endforeach()

endforeach()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests24 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_test(ioTvExtendedFilenameToReaderOptions_NoOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
         ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
	otbExtendedFilenameToReaderOptions
        /home/data/filename.tif
        ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
        )

add_test(ioTvExtendedFilenameToReaderOptions_FullOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
         ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
    otbExtendedFilenameToReaderOptions
        /home/data/filename.tif?&geom=/home/dev/custom.geom&sdataidx=2&resol=4&skipcarto=On
        ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
        )

add_test(ioTvExtendedFilenameToWriterOptions_NoOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
         ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
	otbExtendedFilenameToWriterOptions
        /home/data/filename.tif
        ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
        )

add_test(ioTvExtendedFilenameToWriterOptions_FullOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
         ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
	otbExtendedFilenameToWriterOptions
        /home/data/filename.tif?&writegeom=ON&gdal:co:QUALITY=75&gdal:co:TILED=YES&gdal:co:BLOCKYSIZE=1024
        ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
        )

add_test(ioTvImageFileReaderExtendedFileName_GEOM ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderWithExternalGEOMFile.txt
         ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
        ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
        ${TEMP}/ioImageFileReaderWithExternalGEOMFile_pr.txt
        )

add_test(ioTvImageFileReaderExtendedFileName_SkipPR ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_SkipPR.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipcarto=true
        ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR_kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
        )

add_test(ioTvImageFileReaderExtendedFileName_SkipGeom ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true
        ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
        )

add_test(ioTvImageFileReaderExtendedFileName_mix1 ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_mix1pr.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/bretagne.j2k?&skipcarto=true&resol=2
        ${TEMP}/ioImageFileReaderExtendedFileName_mix1kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
        )

add_test(ioTvImageFileWriterExtendedFileName_GEOM ${IO_TESTS24}
--ignore-order --compare-ascii ${NOTOL}
         ${INPUTDATA}/ToulouseExtract_WithGeom.geom
         ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.geom
--ignore-lines-with 2 ce90_absolute ce90_relative
	otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.tif?&writegeom=ON
        )

add_test(ioTvImageFileWriterExtendedFileName_gdalco ${IO_TESTS24}
--compare-image ${NOTOL}
         ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg
         ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg
	otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg?&gdal:co:QUALITY=20
        )

# Streaming extended options tests

set(streaming_sizevalue_auto "8")
set(streaming_sizevalue_nbsplits "10")
set(streaming_sizevalue_height "100")

add_test(ioTvImageFileWriterExtendedFileName_StreamingNone ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif?&streaming:type=none)

add_test(ioTvImageFileWriterExtendedFileName_StreamingAuto ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif?&streaming:type=auto&streaming:sizevalue=${streaming_sizevalue_auto})


foreach(streaming_type "Tiled" "Stripped")

foreach(streaming_sizemode "Auto" "Nbsplits" "Height")

string(TOLOWER ${streaming_type} type)
string(TOLOWER ${streaming_sizemode} sizemode)

set(test_name "ioTvImageFileWriterExtendedFileName_Streaming${streaming_type}${streaming_sizemode}")


add_test(${test_name} ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/${test_name}.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/${test_name}.tif?&streaming:type=${type}&streaming:sizemode=${sizemode}&streaming:sizevalue=${streaming_sizevalue_${sizemode}})

endforeach()

endforeach()


#region size options test

add_test(ioTvImageFileWriterExtendedFileName_Box ${IO_TESTS24}
  otbImageFileWriterWithExtendedOptionBox
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_box.tif
        2
        2
        200
        200
        10
        )

#----------------------------------------------------------------------------------
set(BasicIO_SRCS1
otbIOTests1.cxx
otbLUMImageIOTestCanRead.cxx
otbLUMImageIOTestCanWrite.cxx
otbMWImageIOTestCanRead.cxx
otbMWImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderTestFloat.cxx
otbShortImageIOTest.cxx
otbImageIOFactoryNew.cxx
otbImageFileReaderWithComplexPixel.cxx
otbCompareWritingComplexImage.cxx
)
set(BasicIO_SRCS2
otbIOTests2.cxx
otbIntImageIOTest.cxx
otbFloatImageIOTest.cxx
otbImageFileReaderTest.cxx
otbDoubleImageIOTest.cxx
otbStreamingImageFilterTest.cxx
otbStreamingImageFileWriterTest.cxx
otbPipeline.cxx
otbGDALImageIOTest.cxx
otbGDALOverviewsBuilder.cxx
)
set(BasicIO_SRCS3
otbIOTests3.cxx
otbBSQImageIOTestCanRead.cxx
otbBSQImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
otbIntImageIOTest.cxx
otbShortRGBImageIOTest.cxx
otbPNGIndexedNbBandsTest.cxx
)

#Tests requiring access to ossim header
#should not be launched when using an installed version of OTB
if(NOT OTB_TESTING_STANDALONE)
set(BasicIO_SRCS4
otbIOTests4.cxx
otbImageKeywordlist.cxx
otbOssimJpegFileRessourceLeakTest.cxx
)
endif()

set(BasicIO_SRCS5
otbIOTests5.cxx
otbGDALImageIOTestCanRead.cxx
otbThreadsTest.cxx
)

set(BasicIO_SRCS6
otbIOTests6.cxx
otbGDALImageIOTestCanWrite.cxx
otbONERAImageIOTestCanRead.cxx
otbImageFileReaderONERA.cxx
otbImageFileReaderONERAComplex.cxx
otbImageFileWriterONERAComplex.cxx
otbImageFileWriterStreamingONERAComplex.cxx
otbMSTARImageIOTestCanRead.cxx
otbImageFileReaderMSTAR.cxx
otbGDALImageIOTestWriteMetadata.cxx
)

set(BasicIO_SRCS7
otbIOTests7.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterTest.cxx
otbImageFileWriterRGBTest.cxx
)

set(BasicIO_SRCS8
otbIOTests8.cxx
otbCoordinateToNameTest.cxx
)

set(BasicIO_SRCS9
otbIOTests9.cxx
otbImageTest.cxx
otbVectorImageTest.cxx
otbStreamingImageFilterTest.cxx
otbVectorImageFileReaderWriterTest.cxx
otbStreamingShortImageFileWriterTest.cxx
otbComplexImageTests.cxx
)

set(BasicIO_SRCS10
otbIOTests10.cxx
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
)

set(BasicIO_SRCS11
otbIOTests11.cxx
otbImageMetadataFileWriterTest.cxx
otbImageMetadataStreamingFileWriterTest.cxx
otbPipelineMetadataHandlingTest.cxx
otbPipelineMetadataHandlingWithUFFilterTest.cxx
)

set(BasicIO_SRCS12
otbIOTests12.cxx
otbDEMToImageGeneratorNew.cxx
otbDEMToImageGeneratorTest.cxx
otbDEMToImageGeneratorFromImageTest.cxx
otbMapProjectionAdapterTest.cxx
)


if(OTB_USE_JPEG2000_TESTING)
#Reserved for jpeg 2000

set(BasicIO_SRCS13
otbIOTests13.cxx
otbVectorImageTest.cxx
otbMultiResolutionReadingInfo.cxx
)
# add tests specific for JPEG2000ImageIO
if(OTB_USE_JPEG2000)
set(BasicIO_SRCS13 ${BasicIO_SRCS13} 
otbJPEG2000ImageIOTestCanRead.cxx
otbJPEG2000ImageIOTestCanWrite.cxx
otbGenerateClassicalQLWithJPEG2000.cxx
)
endif()

endif()


set(BasicIO_SRCS14
otbIOTests14.cxx
otbGDALDriverDoubleWritingTest.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbWritingComplexDataWithComplexImageTest.cxx
)



set(BasicIO_SRCS15
otbIOTests15.cxx
otbVectorDataIOFactory.cxx
otbVectorDataSourceNew.cxx
otbVectorDataFileReaderNew.cxx
otbVectorDataFileReader.cxx
otbVectorDataFileWriterNew.cxx
otbVectorDataFileWriter.cxx
otbVectorDataFileWriterPolygons.cxx
otbVectorDataFileWriterMultiPolygons.cxx
otbVectorDataFileReaderWriter.cxx
otbVectorDataFileGeoReaderWriter.cxx
otbOGRVectorDataIONew.cxx
otbOGRVectorDataIOCanRead.cxx
otbOGRVectorDataIOCanWrite.cxx
otbKMLVectorDataIONew.cxx
otbKMLVectorDataIOTestCanRead.cxx
otbKMLVectorDataIOTestCanWrite.cxx
otbKMLVectorDataIOTestFileReader.cxx
otbVectorDataKeywordlistTest.cxx
)

set(BasicIO_SRCS16
otbIOTests16.cxx
otbRADImageIOTestCanRead.cxx
otbImageFileReaderRADChar.cxx
otbImageFileReaderRADInt.cxx
otbImageFileReaderRADFloat.cxx
otbImageFileReaderRADComplexInt.cxx
otbImageFileReaderRADComplexFloat.cxx
otbImageFileReaderRADComplexFloatExtract.cxx
otbImageFileReaderRADComplexDouble.cxx
otbImageSeriesFileReader.cxx
)

set(BasicIO_SRCS18
otbIOTests18.cxx
otbSpotImageMetadataInterfaceNew.cxx
otbPleiadesImageMetadataInterfaceNew.cxx
otbSpot6ImageMetadataInterfaceNew.cxx
otbFormosatImageMetadataInterfaceNew.cxx
otbQuickBirdImageMetadataInterfaceNew.cxx
otbWorldView2ImageMetadataInterfaceNew.cxx
otbIkonosImageMetadataInterfaceNew.cxx
otbTerraSarImageMetadataInterfaceNew.cxx
otbImageMetadataInterfaceBaseTest.cxx
otbOpticalImageMetadataInterfaceTest.cxx
otbSarImageMetadataInterfaceTest.cxx
otbImageMetadataInterfaceTest2.cxx
)

if(OTB_USE_CURL)
#Reserved for CURL
set(BasicIO_SRCS19
otbIOTests19.cxx
otbImageFileReaderServerName.cxx
otbTileMapImageIOTest.cxx
otbTileMapWriter.cxx
otbOSMDataToVectorDataTests.cxx
otbImageToOSMVectorDataGenerator.cxx
)
endif()

set(BasicIO_SRCS20
otbIOTests20.cxx
otbImageFileReaderTestSensorPixelValue.cxx
otbDefaultImageMetadataInterfaceFactoryNew.cxx
otbDefaultImageMetadataInterface.cxx
otbOpticalDefaultImageMetadataInterfaceFactoryNew.cxx
otbOpticalDefaultImageMetadataInterfaceNew.cxx
otbOpticalDefaultImageMetadataInterface.cxx
otbSarDefaultImageMetadataInterfaceFactoryNew.cxx
otbSarDefaultImageMetadataInterfaceNew.cxx
otbSarDefaultImageMetadataInterface.cxx
otbScalarBufferToImageFileWriterNew.cxx
otbScalarBufferToImageFileWriterTest.cxx
)

set(BasicIO_SRCS21
otbIOTests21.cxx
otbMapFileProductWriterNew.cxx
otbMapFileProductWriter.cxx
otbKmzProductWriter.cxx
otbTileMapImageIOHelperTest.cxx
otbComplexImageManipulationTest.cxx
otbGDALReadPxlComplex.cxx
)

if(CHECK_GDAL_BUILT_WITH_HDF)
set(BasicIO_SRCS22
otbIOTests22.cxx
otbVectorImageTest.cxx
otbMultiDatasetReadingInfo.cxx
)
endif()

set(BasicIO_SRCS23
otbIOTests23.cxx
otbSpectralSensitivityReaderTest.cxx
otbWavelengthSpectralBandsTest.cxx
otbComplexImageManipulationTest.cxx
otbImageFileWriterTestWithoutInput.cxx
otbVectorImageFileWriterTestWithoutInput.cxx
otbImageStreamingFileWriterTestWithoutInput.cxx
otbVectorImageStreamingFileWriterTestWithoutInput.cxx
otbGDALReadPxlComplex.cxx
)

set(BasicIO_SRCS24
otbIOTests24.cxx
otbExtendedFilenameToReaderOptionsTest.cxx
otbExtendedFilenameToWriterOptionsTest.cxx
otbExtendedFilenameTest.cxx
otbImageFileWriterWithExtendedOptionBox.cxx
)

if(OTB_USE_EXTERNAL_OSSIM)
  include_directories( ${OSSIM_INCLUDE_DIR} )
else()
  include_directories( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
endif()

OTB_ADD_EXECUTABLE(otbIOTests1 "${BasicIO_SRCS1}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests2 "${BasicIO_SRCS2}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests3 "${BasicIO_SRCS3}" "OTBIO;OTBTesting")

if(NOT OTB_TESTING_STANDALONE)
OTB_ADD_EXECUTABLE(otbIOTests4 "${BasicIO_SRCS4}" "OTBIO;OTBTesting")
endif()

OTB_ADD_EXECUTABLE(otbIOTests5 "${BasicIO_SRCS5}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests6 "${BasicIO_SRCS6}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests7 "${BasicIO_SRCS7}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests8 "${BasicIO_SRCS8}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests9 "${BasicIO_SRCS9}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests10 "${BasicIO_SRCS10}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests11 "${BasicIO_SRCS11}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests12 "${BasicIO_SRCS12}" "OTBIO;OTBTesting")

if(OTB_USE_JPEG2000_TESTING)
   if(NOT BUILD_SHARED_LIBS)
      add_definitions(-DOPJ_STATIC)
   endif()

   OTB_ADD_EXECUTABLE(otbIOTests13 "${BasicIO_SRCS13}" "OTBIO;OTBTesting")
endif()

OTB_ADD_EXECUTABLE(otbIOTests14 "${BasicIO_SRCS14}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests15 "${BasicIO_SRCS15}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests16 "${BasicIO_SRCS16}" "OTBIO;OTBTesting")

OTB_ADD_EXECUTABLE(otbIOTests18 "${BasicIO_SRCS18}" "OTBIO;OTBTesting")

if(OTB_USE_CURL)
  OTB_ADD_EXECUTABLE(otbIOTests19 "${BasicIO_SRCS19}" "OTBIO;OTBTesting")
endif()

OTB_ADD_EXECUTABLE(otbIOTests20 "${BasicIO_SRCS20}" "OTBIO;OTBTesting")

endif()

OTB_ADD_EXECUTABLE(otbIOTests21 "${BasicIO_SRCS21}" "OTBIO;OTBTesting")

if(CHECK_GDAL_BUILT_WITH_HDF)
  OTB_ADD_EXECUTABLE(otbIOTests22 "${BasicIO_SRCS22}" "OTBIO;OTBTesting")
endif()

OTB_ADD_EXECUTABLE(otbIOTests23 "${BasicIO_SRCS23}" "OTBIO;OTBTesting")

OTB_ADD_EXECUTABLE(otbIOTests24 "${BasicIO_SRCS24}" "OTBIO;OTBTesting")
