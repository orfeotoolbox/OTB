
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


IF(OTB_DATA_USE_SPOTPHRINPUT)
  SET(SPOTPHRINPUT ${OTB_DATA_SPOTPHRINPUT_ROOT} )
ENDIF(OTB_DATA_USE_SPOTPHRINPUT)

IF(OTB_DATA_USE_CNESPHRINPUT)
  SET(CNESPHRINPUT ${OTB_DATA_CNESPHRINPUT_ROOT} )
ENDIF(OTB_DATA_USE_CNESPHRINPUT)

SET(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
SET(COMMON_TESTS3 ${CXX_TEST_PATH}/otbCommonTests3)
SET(IO_TESTS1     ${CXX_TEST_PATH}/otbIOTests1)
SET(IO_TESTS2     ${CXX_TEST_PATH}/otbIOTests2)
SET(IO_TESTS3     ${CXX_TEST_PATH}/otbIOTests3)
SET(IO_TESTS4     ${CXX_TEST_PATH}/otbIOTests4)
SET(IO_TESTS5     ${CXX_TEST_PATH}/otbIOTests5)
SET(IO_TESTS6     ${CXX_TEST_PATH}/otbIOTests6)
SET(IO_TESTS7     ${CXX_TEST_PATH}/otbIOTests7)
SET(IO_TESTS8     ${CXX_TEST_PATH}/otbIOTests8)
SET(IO_TESTS9     ${CXX_TEST_PATH}/otbIOTests9)
SET(IO_TESTS10    ${CXX_TEST_PATH}/otbIOTests10)
SET(IO_TESTS11    ${CXX_TEST_PATH}/otbIOTests11)
SET(IO_TESTS12    ${CXX_TEST_PATH}/otbIOTests12)

IF(OTB_USE_JPEG2000)
   SET(IO_TESTS13    ${CXX_TEST_PATH}/otbIOTests13)
ENDIF(OTB_USE_JPEG2000)

SET(IO_TESTS14    ${CXX_TEST_PATH}/otbIOTests14)
SET(IO_TESTS15    ${CXX_TEST_PATH}/otbIOTests15)
SET(IO_TESTS16    ${CXX_TEST_PATH}/otbIOTests16)

IF(OTB_USE_LIBLAS)
  SET(IO_TESTS17    ${CXX_TEST_PATH}/otbIOTests17)
ENDIF(OTB_USE_LIBLAS)

SET(IO_TESTS18    ${CXX_TEST_PATH}/otbIOTests18)

IF(OTB_USE_CURL)
  SET(IO_TESTS19    ${CXX_TEST_PATH}/otbIOTests19)
ENDIF(OTB_USE_CURL)

SET(IO_TESTS20    ${CXX_TEST_PATH}/otbIOTests20)
SET(IO_TESTS21    ${CXX_TEST_PATH}/otbIOTests21)

IF(CHECK_GDAL_BUILT_WITH_HDF)
  SET(IO_TESTS22    ${CXX_TEST_PATH}/otbIOTests22)
ENDIF(CHECK_GDAL_BUILT_WITH_HDF)

SET(IO_TESTS23    ${CXX_TEST_PATH}/otbIOTests23)
SET(IO_TESTS24    ${CXX_TEST_PATH}/otbIOTests24)

# --------------------------------------------------------------------------------
# Manage PHR data input   
IF(OTB_DATA_USE_LARGEINPUT)
   SET(GenericTestPHR_DATA_INPUTS
      ${GenericTestPHR_DATA_INPUTS}
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits/IMG_PHR1Z_PMS_N_001/IMG_PHR1A_PMS-N_201006181052297_ORT_IPU_20111011_0619-001_R1C1.JP2
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20111109_7807-004_R1C1.TIF
      ${LARGEINPUT}/PLEIADES-PRE/TLSE_TIFF_PRIMARY_DIMAPv2_MS_lossly_12bits/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20111025_3077-004_R1C1.TIF
      ${LARGEINPUT}/PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R2C1.JP2
      ${LARGEINPUT}/PLEIADES/LASVEGAS_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits/IMGPHR_PHR1A_SENSOR_20120217_IPU_20120217_5374-001_V1/IMG_R1C1.JP2
      ${LARGEINPUT}/PLEIADES/PARIS_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits/IMGPHR_PHR1A_ORTHO_20120502_IPU_20120430_6347-003_V1/IMG_R1C1.JP2
   )
   
   SET(GenericTestPHR_DATA_TYPES
      ${GenericTestPHR_DATA_TYPES}
      SIMUL_PHR_JP2_ORTHO_DIMAPv2_PMS-N_lossy_12bits
      SIMUL_PHR_TIFF_ORTHO_DIMAPv2_MS_lossless_8bits
      SIMUL_PHR_TIFF_PRIMARY_DIMAPv2_MS_lossy_12bits
      PHR_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_PRIMARY_PMS_lossless_12bits
      PHR_JP2_DIMAPv1_ORTHO_PMS_lossless_12bits
   )      
ENDIF(OTB_DATA_USE_LARGEINPUT)   

IF(OTB_DATA_USE_SPOTPHRINPUT)
  SET(GenericTestPHR_SPOT_DATA_INPUTS
      ${SPOTPHRINPUT}/ORTHO_UTM_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_ORT_IPU_20120221_8798-001_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_UTM_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20120221_8798-002_R1C1.JP2
      ${SPOTPHRINPUT}/PRIMARY_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_SEN_IPU_20120221_8686-001_R1C1.JP2
      ${SPOTPHRINPUT}/PRIMARY_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_SEN_IPU_20120221_8686-002_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_GEOGRAPHIC_BUNDLE/IMG_PHR1A_P_001/IMG_PHR1A_P_201006181052297_ORT_IPU_20120221_8718-001_R1C1.JP2
      ${SPOTPHRINPUT}/ORTHO_GEOGRAPHIC_BUNDLE/IMG_PHR1A_MS_002/IMG_PHR1A_MS_201006181052297_ORT_IPU_20120221_8718-002_R1C1.JP2
      )

  SET(GenericTestPHR_SPOT_DATA_TYPES
	  SPOT_PHR_ORTHO_UTM_P
	  SPOT_PHR_ORTHO_UTM_MS
	  SPOT_PHR_ORTHO_GEO_P
      SPOT_PHR_ORTHO_GEO_MS
	  SPOT_PHR_PRIMARY_MS
	  SPOT_PHR_PRIMARY_P
	 )
	 
   SET(GenericTestPHR_DATA_INPUTS ${GenericTestPHR_DATA_INPUTS} ${GenericTestPHR_SPOT_DATA_INPUTS})	  
   SET(GenericTestPHR_DATA_TYPES ${GenericTestPHR_DATA_TYPES} ${GenericTestPHR_SPOT_DATA_TYPES})   
ENDIF(OTB_DATA_USE_SPOTPHRINPUT)

IF(OTB_DATA_USE_CNESPHRINPUT)
  SET(GenericTestPHR_CNES_DATA_INPUTS
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201212716417737721/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120127_5579-001_R1C1.JP2
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2
      ${CNESPHRINPUT}/PRIMARY_PMS/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R3C1.JP2
      )

  SET(GenericTestPHR_CNES_DATA_TYPES
	  CNES_PHR_PRIMARY_PMS_R1C1_2012-01-27
	  CNES_PHR_PRIMARY_PMS_R1C1_2012-02-22
	  CNES_PHR_PRIMARY_PMS_R3C1_2012-02-22
	 )
  
  SET(GenericTestPHR_DATA_INPUTS ${GenericTestPHR_DATA_INPUTS} ${GenericTestPHR_CNES_DATA_INPUTS})	 
  SET(GenericTestPHR_DATA_TYPES ${GenericTestPHR_DATA_TYPES} ${GenericTestPHR_CNES_DATA_TYPES})    	 	 
ENDIF(OTB_DATA_USE_CNESPHRINPUT)

# Use in debug to show the variables
#SET( GenericTestPHR_TESTNB 0)  
#FOREACH( current_file ${GenericTestPHR_DATA_INPUTS} )
#  LIST(GET GenericTestPHR_DATA_TYPES  ${GenericTestPHR_TESTNB} current_type   )
#  MATH(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
#  
#  MESSAGE(STATUS ${current_file})
#  MESSAGE(STATUS ${current_type})
#
#ENDFOREACH()

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)

# ---  RADARSAT (GDAL)  ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT2ENVI ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROI
         -ushort ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  dir name ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI ${COMMON_TESTS3}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
#                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         -startX 0 -startY 0 -sizeX ParseHdfSubsetName337 -sizeY 280 )
SET_TESTS_PROPERTIES(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI PROPERTIES DEPENDS ioTvMultiMonoChannelROI_RADARSAT2ENVI)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  ENVI (GDAL) ---
ADD_TEST(ioTvMultiMonoChannelROI_ENVI2PNG ${COMMON_TESTS3}
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

IF(OTB_DATA_USE_LARGEINPUT)
# ---  SAR_ERS2_SLCI (GDAL)  ---
ADD_TEST(ioTvImageFileReaderWithComplexPixel_PALSAR ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
            ${TEMP}/ioImageFileReaderWithComplexPixel_PALSAR_l1.1.tif
            0 0 100 100)

ADD_TEST(ioTvImageFileReaderWithComplexPixel_ERS ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/
            ${TEMP}/ioImageFileReaderWithComplexPixel_ERS_SLCI.tif
            0 0 100 100)

ADD_TEST(ioTvImageFileReaderWithComplexPixel_RADARSAT ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
        otbImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT.tif
            0 0 100 100)

ADD_TEST(ioTvVectorImageFileReaderWithComplexPixel_RADARSAT2 ${IO_TESTS1}
        --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
        otbVectorImageFileReaderWithComplexPixelTest
            ${LARGEINPUT}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
            ${TEMP}/ioImageFileReaderWithComplexPixel_RADARSAT2.tif
            0 0 100 100)

ADD_TEST(ioTvCompareWritingComplexImage_TIF ${IO_TESTS1}
        otbCompareWritingComplexImageTest
            ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
            ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.tif
            0 0 100 100)

ADD_TEST(ioTvCompareWritingComplexImage_HDR ${IO_TESTS1}
         otbCompareWritingComplexImageTest
         ${LARGEINPUT}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/ioTvImageFileReaderCompareComplexPixelWithFloatPixel_PALSAR.hdr
         0 0 100 100)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ------- TEST IO COMMON   ------------------------------


# -------            otb::LUMImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuLUMImageIOCanRead ${IO_TESTS1} otbLUMImageIOTestCanRead
        ${INPUTDATA}/poupees.lum)

# CanWrite()
ADD_TEST(ioTuLUMImageIOCanWrite ${IO_TESTS1} otbLUMImageIOTestCanWrite
        ${TEMP}/poupees.lum)

ADD_TEST(ioTvImageFileReaderHDR2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_1canal.c1.hdr
                                ${TEMP}/ioImageFileReaderHDR2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.c1.hdr
        ${TEMP}/ioImageFileReaderHDR2LUM.lum )

ADD_TEST(ioTvImageFileReaderPNG2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2LUM.lum )

ADD_TEST(ioTvImageFileReaderLUM2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioImageFileReaderLUM2LUM.lum
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderPNG2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2LUM.lum )
SET_TESTS_PROPERTIES(ioTvImageFileReaderLUM2LUM PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

ADD_TEST(ioTvImageFileWriterLUM2HDR ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderLUM2HDR.hdr
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderLUM2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2HDR.hdr )
SET_TESTS_PROPERTIES(ioTvImageFileWriterLUM2HDR PROPERTIES DEPENDS ioTvImageFileReaderLUM2LUM)

# --- Image I2
ADD_TEST(ioTvImageFileReaderShortLUM2HDR ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum
        otbShortImageIOTest
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum )
SET_TESTS_PROPERTIES(ioTvImageFileReaderShortHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderShortLUM2HDR)

ADD_TEST(ioTvImageFileReaderShortLUM2LUM ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum )

# -------            otb::MWImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuMWImageIOCanReadIMG ${IO_TESTS1} otbMWImageIOTestCanRead
        ${BASELINE}/QB_Toulouse_Ortho_PAN.img)

ADD_TEST(ioTuMWImageIOCanReadMW ${IO_TESTS1} otbMWImageIOTestCanRead
        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw)

ADD_TEST(ioTvImageFileReaderMW2TIFIMG ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                ${TEMP}/ioImageFileReaderMW2TIFIMG.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderMW2TIFIMG.tif )

ADD_TEST(ioTvImageFileReaderMW2TIFMW ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw
                                ${TEMP}/ioImageFileReaderMW2TIFMW.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderMW2TIFMW.tif )


# CanWrite()
ADD_TEST(ioTuMWImageIOCanWriteMW ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.mw)

ADD_TEST(ioTuMWImageIOCanWriteIMG ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.img)
SET_TESTS_PROPERTIES(ioTuMWImageIOCanWriteIMG PROPERTIES WILL_FAIL true)

ADD_TEST(ioTvImageFileReaderTIF2MW ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                ${TEMP}/ioImageFileReaderTIF2MW.mw
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderTIF2MW.mw )


# We use the original file as a baseline for the comparison
 ADD_TEST(ioTvImageFileReaderPCI2TIF ${IO_TESTS1}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                    ${TEMP}/ioImageFileReaderPCI2TIF.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix
        ${TEMP}/ioImageFileReaderPCI2TIF.tif )

 ADD_TEST(ioTvImageFileReaderTIF2PCI ${IO_TESTS1}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                    ${TEMP}/ioImageFileReaderTIF2PCI.pix
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderTIF2PCI.pix )

#-------------  otbImageIOFactoryNew
ADD_TEST(ioTuImageIOFactoryNew ${IO_TESTS1}
        otbImageIOFactoryNew )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.lum
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderIntHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum )
SET_TESTS_PROPERTIES(ioTvImageFileReaderIntHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderIntLUM2HDR)

# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.lum
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr )

ADD_TEST(ioTvImageFileReaderFloatHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum )
SET_TESTS_PROPERTIES(ioTvImageFileReaderFloatHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderFloatLUM2HDR)

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleLUM2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.lum
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr )

ADD_TEST(ioTvImageFileReaderDoubleHDR2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum )
SET_TESTS_PROPERTIES(ioTvImageFileReaderDoubleHDR2LUM PROPERTIES DEPENDS ioTvImageFileReaderDoubleLUM2HDR)



ADD_TEST(ioTvImageFileReaderFloatPNG2LUM ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum
        otbFloatImageIOTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum )


# ---  STREAMING : LUM  ---
ADD_TEST(ioTvStreamingImageFilterLUM2PNG ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFilterTestLUM.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFilterTestLUM.png
         )
SET_TESTS_PROPERTIES(ioTvStreamingImageFilterLUM2PNG PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         0 # StreamingImageFileWriter
         0 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingIFWriterLUMWithoutStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  With STREAMING IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         1 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingIFWriterLUMWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  LUM STREAMING  With IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingWithIFWriterLUMWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingWithImageFileWriterLUM2LUMWithStreaming_10.lum
         0 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingWithIFWriterLUMWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2LUM)

# ---  STREAMING  Pipeline Example ---
ADD_TEST(ioTvStreamingPipeline_TIF2PNG ${IO_TESTS2}
         otbPipeline
         ${INPUTDATA}/cthead1.png #ROI_IKO_PAN_LesHalles.tif
         ${TEMP}/ioPipeline.png
         )

# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatBSQ2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.hd
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderFloatHDR2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd )
SET_TESTS_PROPERTIES(ioTvImageFileReaderFloatHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderFloatBSQ2HDR)

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleBSQ2HDR ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.hd
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr )

ADD_TEST(ioTvImageFileReaderDoubleHDR2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd )
SET_TESTS_PROPERTIES(ioTvImageFileReaderDoubleHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderDoubleBSQ2HDR)

# ---  STREAMING FILTER : BSQ  ---
ADD_TEST(ioTvImageFileReaderPNG2BSQ ${IO_TESTS2}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2BSQ.hd
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2BSQ.hd )

ADD_TEST(ioTvStreamingImageFilterBSQ2PNG ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFilterTestBSQ.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFilterTestBSQ.png
         )
SET_TESTS_PROPERTIES(ioTvStreamingImageFilterBSQ2PNG PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         0 # StreamingImageFileWriter
         0 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingIFWriterBSQWithoutStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         1 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingIFWriterBSQWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingWithIFWriterBSQWithStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingWithImageFileWriterBSQ2BSQWithStreaming_10.hd
         0 # StreamingImageFileWriter
         10 # NumberOfStreamDivisions
         )
SET_TESTS_PROPERTIES(ioTvStreamingWithIFWriterBSQWithStreaming PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)


ADD_TEST(ioTvGDALImageIO_Tiff_NoOption ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         )

ADD_TEST(ioTvGDALImageIO_Tiff_12bits ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_12bits.tif
         "NBITS=12"
         )

ADD_TEST(ioTvGDALImageIO_Tiff_Stripped ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_stripped.tif
         "BLOCKYSIZE=10"
         )

ADD_TEST(ioTvGDALImageIO_Tiff_Tiled_16x16 ${IO_TESTS2}
   --compare-image ${NOTOL} ${INPUTDATA}/maur_rgb.tif
                            ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
         otbGDALImageIOTest_uint16
         ${INPUTDATA}/maur_rgb.tif
         ${TEMP}/ioTvGDALImageIO_Tiff_tiled_16x16.tif
         "TILED=YES"
         "BLOCKXSIZE=16"
         "BLOCKYSIZE=16"
         )

ADD_TEST(ioTvGDALImageIO_Tiff_JPEG_20 ${IO_TESTS2}
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

ADD_TEST(ioTvGDALImageIO_Tiff_JPEG_99 ${IO_TESTS2}
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

ADD_TEST(ioTvGDALImageIO_JPEG_20 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg
                            ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_JPEG_20.jpg
         "QUALITY=20"
         )

ADD_TEST(ioTvGDALImageIO_JPEG_99 ${IO_TESTS2}
   --compare-image ${NOTOL} ${BASELINE}/ioTvGDALImageIO_JPEG_99.jpg
                            ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
         otbGDALImageIOTest_uint8
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioTvGDALImageIO_JPEG_99.jpg
         "QUALITY=99"
         )


ADD_TEST(ioTuGDALOverviewsBuilder ${IO_TESTS2}
         otbGDALOverviewsBuilderNew)
         
ADD_TEST(ioTvGDALOverviewsBuilder_TIFF ${IO_TESTS2}
         otbGDALOverviewsBuilder
         ${TEMP}/ioTvGDALImageIO_Tiff_NoOption.tif
         4
         )
SET_TESTS_PROPERTIES(ioTvGDALOverviewsBuilder_TIFF PROPERTIES DEPENDS ioTvGDALImageIO_Tiff_NoOption)
         
                  
         

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::BSQImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuBSQImageIOCanRead ${IO_TESTS3} otbBSQImageIOTestCanRead
        ${INPUTDATA}/poupees.hd)

# CanWrite()
ADD_TEST(ioTuBSQImageIOCanWrite ${IO_TESTS3} otbBSQImageIOTestCanWrite
        ${TEMP}/poupees.hd)

ADD_TEST(ioTvImageFileReaderPNG2ENVI ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioImageFileReaderPNG2ENVI.hdr
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2ENVI.hdr )
SET_TESTS_PROPERTIES(ioTvImageFileReaderPNG2ENVI PROPERTIES DEPENDS ioTvImageFileReaderPNG2BSQ)


ADD_TEST(ioTvImageFileReaderBSQ2PNG ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_1canal.hd
                                ${TEMP}/ioImageFileReaderBSQ2PNG.png
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.hd
        ${TEMP}/ioImageFileReaderBSQ2PNG.png )

ADD_TEST(ioTvImageFileReaderRGB_PNG2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/couleurs_extrait.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd )

ADD_TEST(ioTvImageFileReaderRGB_PNG2ENVI ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/couleurs_extrait.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr )

# FIXME Desactivated until http://bugs.orfeo-toolbox.org/view.php?id=103
# is solved
#add a new test
 ADD_TEST(ioTvCheckNbBandsPNGIndexee ${IO_TESTS3}
         otbPNGIndexedNbBandsTest
         ${INPUTDATA}/sbuv_indexee.png
         4 )


# --- Image I2
ADD_TEST(ioTvImageFileReaderShortRGB_BSQ2HDR ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        otbShortRGBImageIOTest
        ${INPUTDATA}/poupees_I2.hd
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd
        otbShortRGBImageIOTest
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd )
SET_TESTS_PROPERTIES(ioTvImageFileReaderShortHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderShortRGB_BSQ2HDR)

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntBSQ2HDR ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.hd
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr )

ADD_TEST(ioTvImageFileReaderIntHDR2BSQ ${IO_TESTS3}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd )
SET_TESTS_PROPERTIES(ioTvImageFileReaderIntHDR2BSQ PROPERTIES DEPENDS ioTvImageFileReaderIntBSQ2HDR)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(NOT OTB_TESTING_STANDALONE)

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvImageKeywordlistSPOT5 ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistFORMOSAT2 ${IO_TESTS4}
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


ADD_TEST(ioTvImageKeywordlistQuickbird ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistIkonos ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRapidEye ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistEnvisatAsar ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistAlosPalsar ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRadarSat1_GOMA ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRadarSat1_GOMA_vdf ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRadarSat1_SARDEGNA ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRadarSat1_SARDEGNA_vdf ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistRadarSat2 ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistTerraSarX ${IO_TESTS4}
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

ADD_TEST(ioTvImageKeywordlistTerraSarX_Upsala ${IO_TESTS4}
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
ADD_TEST(ioTvImageKeywordlistTerraSarX_Toronto ${IO_TESTS4}
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

ADD_TEST(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingXmlFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)
ADD_TEST(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingHHFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)
ADD_TEST(ioTuImageKeywordlistTerraSarX_PANGKALANBUUNUsingVVFile ${IO_TESTS4}
    otbImageKeywordlist
         ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Pangkalanbuun_saveState_loadState.txt
)



ADD_TEST(ioTvImageKeywordlistErs ${IO_TESTS4}
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
ENDIF(OTB_DATA_USE_LARGEINPUT)


IF(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)
  
  SET( GenericTestPHR_TESTNB 0)
  
  FOREACH( current_file ${GenericTestPHR_DATA_INPUTS} )
    LIST(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type )
    MATH(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
    
      # Get and manipulate ImageKeywordlist 
      ADD_TEST(ioTvImageKeywordlist_${current_type} ${IO_TESTS4}
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

  ENDFOREACH()

ENDIF(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)


ADD_TEST(ioTvOssimJpegFileRessourceLeak ${IO_TESTS4}
        otbOssimJpegFileRessourceLeakTest
        ${INPUTDATA}/poupees.jpg
        1200
)
ENDIF(NOT OTB_TESTING_STANDALONE)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::GDALImageIO   ------------------------------

# READER

ADD_TEST(ioTuGDALImageIOCanRead_ENVI ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupees_1canal.c1.hdr )
        ${INPUTDATA}/poupees_1canal.c1 )


ADD_TEST(ioTuGDALImageIOCanRead_PDS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${INPUTDATA}/pdsImage.img )

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuGDALImageIOCanRead_RADARSAT ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/ )
        ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/ )
        ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_WV2_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_WV2_MUL ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_WV2_PSH ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_pansharpened_natural_color_8bit/052298834010_01_P001_PSH/09DEC10103019-S2AS-052298834010_01_P001.TIF )


ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_pan_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_GRN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_grn_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_BLU ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_blu_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_RED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_red_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT4 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/ )
        ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT )

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_FORMOSAT2 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF)

ADD_TEST(ioTuGDALImageIOCanRead_COSMOSKYMED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5)

ADD_TEST(ioTuGDALImageIOCanRead_COSMOSKYMED2 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_SCS_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5)

ADD_TEST(ioTuGDALImageIOCanRead_COSMOSKYMED3 ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS2_GTC_B_S2_08_HH_RD_SF_20110417180326_20110417180333.h5)

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT5TIF ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupeesTIF/ )
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_HFA ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${INPUTDATA}/HFAGeoreferenced.img)

ADD_TEST(ioTuGDALImageIOCanRead_PCI ${IO_TESTS5} otbGDALImageIOTestCanRead
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.pix)

ADD_TEST(ioTvThreadsTest ${IO_TESTS5} otbThreadsTest)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# WRITER

ADD_TEST(ioTuGDALImageIOCanWrite_TIFF ${IO_TESTS6} otbGDALImageIOTestCanWrite
        ${INPUTDATA}/testGDALWriter.tif)
ADD_TEST(ioTuGDALImageIOCanWrite_HFA ${IO_TESTS6} otbGDALImageIOTestCanWrite
        ${INPUTDATA}/HFAGeoreferenced.img)

# Pb with copy of some infos into the dataset
# cases with no ProjRef
ADD_TEST(ioTvGDALImageIOWriteMetadata ${IO_TESTS6}
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
IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuONERAImageIOCanRead ${IO_TESTS6} otbONERAImageIOTestCanRead
        ${LARGEINPUT}/ONERA/spa3_0215_rad.ent)

# Read one ONERAimage type CFloat32 with IFreaderScalar and write to a 2 bands hdr file OK
# crop 10 10 100 100
ADD_TEST(ioTvONERAImageFileReader ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderONERA.hdr
            ${TEMP}/ioImageFileReaderONERA.hdr
         otbImageFileReaderONERATest
            ${LARGEINPUT}/ONERA/spa3_0215_rad.ent
            ${TEMP}/ioImageFileReaderONERA.hdr
            10 10 100 100 )

# Read one ONERAimage type CFloat32 with IFreaderComplex and write to 2 hdr file (Re and Im)  => OK
# crop 10 10 100 100
ADD_TEST(ioTvONERAImageFileReaderComplex ${IO_TESTS6}
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
ADD_TEST(ioTvONERAImageFileWriterComplex ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
         otbImageFileWriterONERAComplex
            ${LARGEINPUT}/ONERA/spa3_0215_rad.ent
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

ADD_TEST(ioTvONERAImageStreamWriter ${IO_TESTS6}
         --compare-image ${EPSILON_9}
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
                          ${TEMP}/ioImageFileReaderONERAStreamWrite.ent
         otbImageFileWriterStreamingONERAComplex
            ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
            ${TEMP}/ioImageFileReaderONERAStreamWrite.ent )
SET_TESTS_PROPERTIES(ioTvONERAImageStreamWriter PROPERTIES DEPENDS ioTvONERAImageFileWriterComplex)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::MSTARImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuMSTARImageIOCanRead1 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB19896.016)

ADD_TEST(ioTuMSTARImageIOCanRead2 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB20010.000)

ADD_TEST(ioTvImageFileReaderMSTAR ${IO_TESTS6}
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
ADD_TEST(ioTvImageFileReaderPNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png )

# RGB
ADD_TEST(ioTvImageFileReaderRGB_PNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2PNG.png )

ADD_TEST(ioTvImageFileReaderRGB_PNG2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_PNG2TIFF.tiff
                    ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileReaderRGB_PNG2TIFF.tiff )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/
        ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )

ADD_TEST(ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

# Suppression CAI
# ADD_TEST(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS7}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#                     ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#         otbImageFileReaderRGBTest
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )



# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )

ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS7}
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr )

ADD_TEST(ioTvImageFileReaderENVI2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
        otbImageFileReaderTest
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png )
SET_TESTS_PROPERTIES(ioTvImageFileReaderENVI2PNG PROPERTIES DEPENDS ioTvImageFileWriterPNG2BSQ)

ADD_TEST(ioTvImageFileReaderPDS2TIFF ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/pdsImage.img
                           ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif
        otbImageFileReaderTest
        ${INPUTDATA}/pdsImage.img
        ${TEMP}/ioTvImageFileReaderPDS2TIFF.tif)
# RGB

ADD_TEST(ioTvImageFileWriterRGB_PNG2PNG ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileWriterRGB_PNG2PNG.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png
        otbImageFileWriterRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileWriterRGB_PNG2PNG.png )

ADD_TEST(ioTvImageFileWriterRGB_PNG2BSQ ${IO_TESTS7}
        otbImageFileWriterRGBTest
        ${INPUTDATA}/couleurs_extrait.png
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG_2 ${IO_TESTS7}
  --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs_extrait.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png
        otbImageFileReaderRGBTest
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ.hdr
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ_2.png )
SET_TESTS_PROPERTIES(ioTvImageFileReaderRGB_BSQ2PNG_2 PROPERTIES DEPENDS ioTvImageFileWriterRGB_PNG2BSQ)

# WRITER GDAL
ADD_TEST(ioTvGDALImageFileWriterTIF2TIF ${IO_TESTS7}
        otbImageFileWriterRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/poupees.tiff )

# -------            otb::ImageFileReader pour GDAL  ------------------------------
# -------            otb::ImageFileWriter pour GDAL  ------------------------------


ADD_TEST(ioTvImageReaderWriterRgbPNG2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png )

ADD_TEST(ioTvImageReaderWriterRgbPNG2TIF ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif )
ADD_TEST(ioTvImageReaderWriterRgbPNG2ENVI ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees.hdr )
ADD_TEST(ioTvImageReaderWriterRgbPNG2JPEG ${IO_TESTS7}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9} ${INPUTDATA}/poupees.png
#                 ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg )

ADD_TEST(ioTvImageReaderWriterRgbTIF2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees.tif
                     ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.tif
        ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbENVI2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees
                     ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees
        ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2PNG ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2TIF ${IO_TESTS7}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF( OTB_USE_CURL )

ADD_TEST(coTvCoordinateToNameTest ${IO_TESTS8}
    --compare-ascii ${NOTOL}
         ${BASELINE}/CoordinateToNameTest.txt
         ${TEMP}/CoordinateToNameTest.txt
    otbCoordinateToNameTest
         103.78 1.29
         ${TEMP}/CoordinateToNameTest.txt
)

#this test intentionaly uses the same baseline as the previous one
ADD_TEST(coTvCoordinateToNameMultithreadTest ${IO_TESTS8}
    --compare-ascii ${NOTOL}
         ${BASELINE}/CoordinateToNameTest.txt
         ${TEMP}/CoordinateToNameMultithreadTest.txt
    otbCoordinateToNameTest
         103.78 1.29
         ${TEMP}/CoordinateToNameMultithreadTest.txt
         1
)

ENDIF( OTB_USE_CURL )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MultiChannelExtractROI   ------------------------------


IF(OTB_DATA_USE_LARGEINPUT)
# ---  SPOT4 (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT4/RIO_DE_JANEIRO/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  ENVI  ---
ADD_TEST(ioTvMultiChannelROI_ENVI2PNG_2 ${COMMON_TESTS2}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/poupees.hdr
         -uchar ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---


# ADD_TEST(ioTvMultiChannelROI_SPOT5BIL2PNG_2 ${COMMON_TESTS2}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         -startX 100 -startY 100 -sizeX 77 -sizeY 300
#         -channels 1 2 3 )

# ---  SPOT5 (CAI)  ---
# ---  Test sur l'image SPOT 5 4 canaux  ---
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
ADD_TEST(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
ADD_TEST(ioTvMultiChannelROI_IkonosRED2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
                            ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_red_0000000.tif
         ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosGRN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_grn_0000000.tif
         ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosBLU2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_blu_0000000.tif
         ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosNIR2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  LUM ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2LUM ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  BSQ ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2BSQ ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                                ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         otbMultiChannelExtractROI
         ${LARGEINPUT}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird PAN  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdPAN2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )

# ---  Quickbird XS  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdXS2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)
#         -channels 1 2 3)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvOtbImageTestSAR_ERS ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest
         ${LARGEINPUT}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
ADD_TEST(ioTvOtbImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}
     ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
     ${TEMP}/ioOtbImageTestRadarsat.txt
   otbImageTest
     ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
     ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTvOtbVectorImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}
     ${TEMP}/ioOtbImageTestRadarsat.txt
     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
   otbVectorImageLegacyTest
     ${LARGEINPUT}/RADARSAT1/GOMA/SCENE01/
     ${TEMP}/ioOtbVectorImageTestRadarsat.txt)
SET_TESTS_PROPERTIES(ioTvOtbVectorImageTestRadarsat PROPERTIES DEPENDS ioTvOtbImageTestRadarsat)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTvOtbVectorImageTestSpot5 ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/SPOT5/TEHERAN/
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)

ADD_TEST(ioTvOtbVectorImageTestFORMOSAT2 ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestFORMOSAT2.txt
                     ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/
        ${TEMP}/ioOtbVectorImageTestFORMOSAT2.txt)

ADD_TEST(ioTvOtbVectorImageTestCOSMOSKYMED ${IO_TESTS9}
   --compare-ascii ${EPSILON_3}  ${BASELINE_FILES}/ioOtbVectorImageTestCOSMOSKYMED.txt
                     ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt
        otbVectorImageLegacyTest
        ${LARGEINPUT}/COSMOSKYMED/Toulouse_spotlight/CSKS3_GTC_B_S2_08_HH_RD_SF_20110418180325_20110418180332.h5
        ${TEMP}/ioOtbVectorImageTestCOSMOSKYMED.txt)

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuVectorImageComplexNew ${IO_TESTS9}
  otbVectorImageComplexNew)

ADD_TEST(ioTvVectorImageComplexFloatTest ${IO_TESTS9}
  otbVectorImageComplexFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

ADD_TEST(ioTvVectorImageComplexDoubleTest ${IO_TESTS9}
  otbVectorImageComplexDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)

ADD_TEST(ioTvImageComplexFloatTest ${IO_TESTS9}
  otbImageComplexFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

ADD_TEST(ioTvImageComplexDoubleTest ${IO_TESTS9}
  otbImageComplexDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)

ADD_TEST(ioTvVectorImageComplexIntoRealFloatTest ${IO_TESTS9}
  otbVectorImageComplexIntoRealFloatTest
    ${INPUTDATA}/multibandComplexFloat_2bands.hdr
)

ADD_TEST(ioTvVectorImageComplexIntoRealDoubleTest ${IO_TESTS9}
  otbVectorImageComplexIntoRealDoubleTest
    ${INPUTDATA}/multibandComplexDouble_2bands.hdr
)


# ---  otb::VectorImageReadWriteTest  ---
ADD_TEST(ioTvVectorImageFileReaderWriterTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
                   ${INPUTDATA}/qb_RoadExtract.img.hdr
                   ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/qb_RoadExtract.img.hdr
        ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
)

ADD_TEST(ioTvVImageFileReader_RESOLUTION_0 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/maur_rgb.tif?&resol=0
        ${TEMP}/ioImageFileReader_RESOLUTION_0.tif )

ADD_TEST(ioTvVImageFileReader_RESOLUTION_3 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/maur_rgb.tif?&resol=3
        ${TEMP}/ioImageFileReader_RESOLUTION_3.tif )

IF(OTB_DATA_USE_LARGEINPUT)
    ADD_TEST(ioTvVImageFileReader_RESOLUTION_4 ${IO_TESTS9}
        otbVectorImageFileReaderWriterTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF?&resol=4
        ${TEMP}/ioImageFileReader_RESOLUTION_4.tif )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  otb::StreamingShortImageFileWriterTest  ---
ADD_TEST(ioTvStreamingShortImageFileWriterTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
                   ${INPUTDATA}/poupees_sub.png
                   ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
        otbStreamingShortImageFileWriterTest
        ${INPUTDATA}/poupees_sub.png
        ${TEMP}/ioTvStreamingShortImageFileWriterTest.png
        0
)

# ---  otb::StreamingShortImageFileWriterTest  ---
ADD_TEST(ioTvStreamingShortImageFileWriterStreamedTest ${IO_TESTS9}
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
ADD_TEST(ioTvStreamingImageFilterGDAL_ENVI ${IO_TESTS9}
#   --compare-image ${EPSILON_9} ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
   --compare-image ${EPSILON_9} ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         )

# ---  STREAMING : GDAL  ---
ADD_TEST(ioTvStreamingImageFilterGDAL_TIFF ${IO_TESTS9}
#   --compare-image ${EPSILON_9} ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
#                      ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
         ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         )

# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS2}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         0 # Streaming
         0
         )
# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS2}
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
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2. SetNumberOfDivisionsStrippedStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsStrippedStreaming.tif
         SetNumberOfDivisionsStrippedStreaming
         10
         )
# 3. SetNumberOfDivisionsTiledStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreamingS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                                      ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfDivisionsTiledStreaming.tif
         SetNumberOfDivisionsTiledStreaming
         10
         )
# 4. SetNumberOfLinesStrippedStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetNumberOfLinesStrippedStreaming.tif
         SetNumberOfLinesStrippedStreaming
         5
         )
# 5. SetAutomaticStrippedStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticStrippedStreaming.tif
         SetAutomaticStrippedStreaming
         10
         )
# 6. SetTileDimensionTiledStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                            ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetTileDimensionTiledStreaming.tif
         SetTileDimensionTiledStreaming
         32
         )
# 7. SetAutomaticTiledStreaming
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                            ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
         otbImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_SetAutomaticTiledStreaming.tif
         SetAutomaticTiledStreaming
                                 10
         )



# Test otb::ImageFileWriter streaming
# 1.bis WITH IMAGEFILEWRITER : DEFAULT
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2.bis WITH IMAGEFILEWRITER : AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
         )
# 3.bis WITH IMAGEFILEWRITER : NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         NUMBER_OF_STREAM_DIVISIONS
         10
         )
# 4.bis WITH IMAGEFILEWRITER BUFFER_MEMORY_SIZE
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         BUFFER_MEMORY_SIZE
         8*2000*10
         )
# 5.bis WITH IMAGEFILEWRITER  BUFFER_NUMBER_OF_LINES
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         BUFFER_NUMBER_OF_LINES
         1000
         )

ADD_TEST(ioTvStreamingIFWriterWithFilter ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioStreamingImageFileWriterWithFilter_1_100.hdr
                            ${TEMP}/ioStreamingImageFileWriterWithFilter_1_100.hdr
         otbImageFileWriterWithFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterWithFilter_1_100.hdr
         1
         1 # Streaming
         100
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS11 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------- Metadata writing tests ------------------------

ADD_TEST(ioTvImageMetadataWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvImageMetadataWriterHFA.img
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvImageMetadataWriterHFA.img
        )

ADD_TEST(ioTvImageMetadataWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/spot5SubWithGcps.tif
                      ${TEMP}/ioTvImageMetadataWriterTIF.tif
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
        ${TEMP}/ioTvImageMetadataWriterTIF.tif
        )


# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#                      ${INPUTDATA}/HFAGeoreferenced.img
#                      ${TEMP}/temp.tif
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#        ${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#                      ${INPUTDATA}/spot5SubWithGcps.tif
#                      ${TEMP}/temp.img
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#        ${TEMP}/temp.img
#        )


ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvImageMetadataStreamingWriterHFA.img
        )

ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/spot5SubWithGcps.tif
                      ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
        ${TEMP}/ioTvImageMetadataStreamingWriterTIF.tif
        )

# Commented because gdal does not seems to write the metadata the same way in both formats
# Compare metadata indicates a small differences between the ProjRef
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#           ${INPUTDATA}/HFAGeoreferenced.img
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_HFA2TIFF.tif
#     otbImageMetadataStreamingFileWriterTest
#       ${INPUTDATA}/HFAGeoreferenced.img
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_HFA2TIFF.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
# Compare metadata indicates that GCP are lost but they are in filename.aux.xml
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#           ${INPUTDATA}/spot5SubWithGcps.tif
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_TIFF2HFA.img
#     otbImageMetadataStreamingFileWriterTest
#       ${INPUTDATA}/spot5SubWithGcps.tif
#           ${TEMP}/ioTvImageMetadataStreamingWriterTest_TIFF2HFA.img
#        )

ADD_TEST(ioTvPipelineMetadataHandlingTest ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
                      ${INPUTDATA}/HFAGeoreferenced.img
                      ${TEMP}/ioTvPipelineMetadataHandling.img
        otbPipelineMetadataHandlingTest
        ${INPUTDATA}/HFAGeoreferenced.img
        ${TEMP}/ioTvPipelineMetadataHandling.img
        )

# Commented out, since this test fails beceause of an ITK bug (added to our bugtracker for tracking)
# MSD: this test doesn't fail today (UFF = Unary Functor Image Filter)
ADD_TEST(ioTvPipelineMetadataHandlingWithUFFilterTest ${IO_TESTS11}
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
ADD_TEST(ioTuDEMToImageGeneratorNew ${IO_TESTS12} otbDEMToImageGeneratorNew )

ADD_TEST(ioTvDEMToImageGeneratorTest ${IO_TESTS12}
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
ADD_TEST(ioTvDEMToImageGeneratorTest1 ${IO_TESTS12}
   --compare-image ${EPSILON_5}
       ${BASELINE}/ioTvDEMToImageGenerator1.tif
       ${TEMP}/ioTvDEMToImageGenerator1.tif
     otbDEMToImageGeneratorTest
       ${INPUTDATA}/DEM/srtm_directory
       ${TEMP}/ioTvDEMToImageGenerator1.tif
       1.437371 43.559045 512 128 0.000032 -0.000024
        )

#Big extend
ADD_TEST(ioTvDEMToImageGeneratorTest2 ${IO_TESTS12}
--compare-image ${EPSILON_5}  ${BASELINE}/ioTvDEMToImageGenerator2.tif
                        ${TEMP}/ioTvDEMToImageGenerator2.tif
        otbDEMToImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToImageGenerator2.tif
         -1.8 52 232 422  0.02 -0.018
#          576378.080 5763132.837 232 422 2000 -2000 30 N
# 3.765220920574784 44.224685913433
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvDEMToImageGeneratorFromImageTest_SensorModel ${IO_TESTS12}
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
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvDEMToImageGeneratorFromImageTest ${IO_TESTS12}
    --compare-image ${EPSILON_5}
        ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest.tif
        ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
      otbDEMToImageGeneratorFromImageTest
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
)

ADD_TEST(ioTvDEMToImageGeneratorFromImageTest2 ${IO_TESTS12}
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
ADD_TEST(ioTvMapProjectionAdapterTest ${IO_TESTS12}
        --compare-ascii ${EPSILON_9}
            ${BASELINE_FILES}/ioTvMapProjectionAdapterTest.txt
            ${TEMP}/ioTvMapProjectionAdapterTest.txt
        otbMapProjectionAdapterTest
            ${TEMP}/ioTvMapProjectionAdapterTest.txt
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS13 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IF(OTB_USE_JPEG2000)

# --------------------------------------------------------------------------------
# old test suite:

# ---  JPEG2000 can read ---
ADD_TEST(ioTuJ2KImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${INPUTDATA}/bretagne.j2k
         )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTuJP2ImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${LARGEINPUT}/JPEG2000/QB_multicomp.jpx
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#NOTE: baseline generated by openjpeg
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorImageFileReaderWriterJP22TIF ${IO_TESTS9}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput
                        ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${LARGEINPUT}/JPEG2000/QB_multicomp_small.jpx
         ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         )


ADD_TEST(ioTvImageFileReaderWriterJP22TIF ${IO_TESTS2}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvImageFileReaderWriterJP22TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         otbDoubleImageIOTest
         ${LARGEINPUT}/JPEG2000/QB_multicomp_small.jpx
         ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# --------------------------------------------------------------------------------
# New test suite:

# Tests to check if we can open JPEG2000 files specifying the resolution factor.
# no access to data only PrintSelf() method.
ADD_TEST(ioTvMultiResolutionReading0 ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading0.txt
                             ${TEMP}/ioTvMultiResolutionReading0.txt
      otbVectorImageTest
      ${INPUTDATA}/bretagne.j2k?&resol=0
      ${TEMP}/ioTvMultiResolutionReading0.txt
      )
     
ADD_TEST(ioTvMultiResolutionReading3 ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading3.txt
                             ${TEMP}/ioTvMultiResolutionReading3.txt
      otbVectorImageTest
      ${INPUTDATA}/bretagne.j2k?&resol=3
      ${TEMP}/ioTvMultiResolutionReading3.txt
      )

# Test to check behavior of function JPEG2000reader->GetResolutionInfo()
# which return info about resolution available in the JPEG2000 file.
ADD_TEST(ioTvMultiResolutionReadingInfo ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut.txt
                            ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
      otbMultiResolutionReadingInfo
      ${INPUTDATA}/bretagne.j2k
      ${TEMP}/ioTvMultiResolutionReadingInfoOut.txt
      )
      
ADD_TEST(ioTvMultiResolutionReadingInfo_TIFF ${IO_TESTS13}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff.txt
                            ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
      otbMultiResolutionReadingInfo
      ${INPUTDATA}/maur_rgb.tif
      ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff.txt
      )   

IF(OTB_DATA_USE_LARGEINPUT)
    ADD_TEST(ioTvMultiResolutionReadingInfo_TIFF2 ${IO_TESTS13}
      --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
                               ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
          otbMultiResolutionReadingInfo
          ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
          ${TEMP}/ioTvMultiResolutionReadingInfoOut_tiff2.txt
          ) 
ENDIF(OTB_DATA_USE_LARGEINPUT)

# Tests which read data in JPEG2000 files.
# Generate a too big output image to be used in baseline repository (useful only for dev)
#ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
#   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944.tif
#                     ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944_OUT.tif
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/bretagne.j2k:0
#         ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_0_0_2592_1944_OUT.tif
#         -startX 0 -startY 0 -sizeX 2592 -sizeY 1944
#         -channels 1 2 3 )


# Read an area inside one tile at resolution 0 (jpeg2000 conformance file with
# specific tile size at different resolution).
ADD_TEST(ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_OUT.tif
            )

# Read an area inside one tile at resolution 4 (jpeg2000 conformance file with
# specific tile size at different resolution).
ADD_TEST(ioTvMultiChannelROI_p1_06_JPEG2000_2_TIF_res4 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/jpeg2000_conf_p1_06.j2k?&resol=4
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_p1_06_res4_OUT.tif
            )

# Read an area inside one tile at resolution 0 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
ADD_TEST(ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res0 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/lena512color.jp2?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_150_75_100_50_OUT.tif
            -startX 150 -startY 75 -sizeX 100 -sizeY 50 )

# Read an area inside one tile at resolution 5 (quite similar coding parameter with pleiade
# except it a lossless image with no quatization style).
ADD_TEST(ioTvMultiChannelROI_lena_JPEG2000_2_TIF_res5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_lena_res5.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res5_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/lena512color.jp2?&resol=5
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_lena_res5_OUT.tif
            )


# Read an area inside one tile at resolution 0
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res0_1 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_700_1000_50_40_OUT.tif
            -startX 700 -startY 1000 -sizeX 50 -sizeY 40 )

# Read an area which need to decode 4 tiles at resolution 0
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res0_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=0
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_1260_950_40_20_OUT.tif
            -startX 1260 -startY 950 -sizeX 40 -sizeY 20 )

# Read all the image with a resolution 3
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res3 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif
         otbMultiChannelExtractROI
            ${INPUTDATA}/bretagne.j2k?&resol=3
            ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_OUT.tif )

# Read area inside one tile at resolution 3
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res3_1 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=3
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_90_70_20_30_OUT.tif
           -startX 90 -startY 70 -sizeX 20 -sizeY 30 )

# Read an area which need to decode 4 tiles at resolution 3
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res3_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=3
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res3_40_30_80_60_OUT.tif
           -startX 40 -startY 30 -sizeX 80 -sizeY 60 )

# Read all the image with a resolution 5
ADD_TEST(ioTvMultiChannelROI_JPEG2000_2_TIF_res5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5.tif
                                  ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5_OUT.tif
         otbMultiChannelExtractROI
           ${INPUTDATA}/bretagne.j2k?&resol=5
           ${TEMP}/ioExtractROI_JPEG2K_2_TIF_bretagne_res5_OUT.tif )

# --------------------------------------------------------------------------------
# Test the possibility to modify the size of cache used to reduce the decoding operations
# O tile in the cache (500K)
ADD_TEST(ioTvJPEG2000ImageIO_CacheSize_500 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_500.tif
         500)

# 1 tile in the cache (1000K)
ADD_TEST(ioTvJPEG2000ImageIO_CacheSize_1000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_1000.tif
         1000)

# 5 tiles in the cache (5000K)
ADD_TEST(ioTvJPEG2000ImageIO_CacheSize_5000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_5000.tif
         5000)

# 135Mb in the cache (set by otbIFR for JPEG2000)
ADD_TEST(ioTvJPEG2000ImageIO_CacheSize_25000 ${IO_TESTS13}
   --compare-image ${EPSILON_9}
         ${BASELINE}/ioClassicalQLJPEG2K_bretagne.tif
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif
         otbGenerateClassicalQLWithJPEG2000
         ${INPUTDATA}/bretagne.j2k
         ${TEMP}/ioClassicalQLJPEG2K_bretagne_OUT.tif)

# --------------------------------------------------------------------------------
# Special Case (deactivated by MSD, OpenJPEG now support the larges files so this tests is not relevant
# Between tiles 328 and 329, the codestream offset crosses the 2**31-1 boundary (max of 32 bit signed integer)
# This reveals a problem on 32 bits systems since fseek takes an integer
#IF(OTB_DATA_USE_LARGEINPUT)
#
#  ADD_TEST(ioTvVectorImageFileReaderWriterJP22TIF_PLEIADES-PRE-R4C1-tile328-329 ${COMMON_TESTS2}
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
#ENDIF(OTB_DATA_USE_LARGEINPUT)

# --------------------------------------------------------------------------------
# Tests about special data (larges files, CNES files, SPOT files)
IF(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)
  SET( GenericTestPHR_TESTNB 0)

FOREACH( current_file ${GenericTestPHR_DATA_INPUTS} )
  LIST(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  MATH(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
  STRING(REGEX MATCH "TIFF" PHR_TIFF_FILE ${current_type})
  
  IF (NOT PHR_TIFF_FILE)
    # CanRead
    ADD_TEST(ioTuJ2KImageIOCanRead_${current_type} ${IO_TESTS13}
             otbJPEG2000ImageIOTestCanRead
                ${current_file}
            )
  ENDIF (NOT PHR_TIFF_FILE)
    
    # Read info from the input file
    ADD_TEST(ioTvVectorImageReadingInfo_${current_type} ${IO_TESTS13}
             --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}.txt
                                       ${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
             otbVectorImageTest
                ${current_file}?&resol=0
                ${TEMP}/ioTvMultiResolutionReading_${current_type}_OUT.txt
             )

  IF (NOT PHR_TIFF_FILE)
    # Read info from the input file (res=5)
    ADD_TEST(ioTvVectorImageReadingInfo_${current_type}_res5 ${IO_TESTS13}
             --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReading_${current_type}_res5.txt
                                       ${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
             otbVectorImageTest
                ${current_file}?&resol=5
                ${TEMP}/ioTvMultiResolutionReading_${current_type}_res5_OUT.txt
             )
  ENDIF (NOT PHR_TIFF_FILE)
            
  IF (NOT PHR_TIFF_FILE)          
    # Extract resolution of input file        
    ADD_TEST(ioTvMultiResolutionReadingInfo_${current_type} ${IO_TESTS13}
             --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiResolutionReadingInfo_${current_type}.txt
                                       ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
             otbMultiResolutionReadingInfo
                ${current_file}
                ${TEMP}/ioTvMultiResolutionReadingInfo_${current_type}_OUT.txt
            )

  
    # Decode an area inside one tile
    ADD_TEST(ioTvReaderWriterJP22TIF_${current_type}_1 ${COMMON_TESTS2}
	         --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_1.tif
                                           ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
             otbMultiChannelExtractROI
                -ushort ${current_file}
                -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_1_OUT.tif
                -startX 256 -startY 512 -sizeX 256 -sizeY 128
                -time
            )
            
    # Decode an area which need 4 tiles
    ADD_TEST(ioTvReaderWriterJP22TIF_${current_type}_2 ${COMMON_TESTS2}
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
    ADD_TEST(ioTvReaderWriterJP22TIF_${current_type}_res5 ${COMMON_TESTS2}
             --compare-image ${EPSILON_9}  ${BASELINE}/ioTvJP22TIF_Extract_${current_type}_res5.tif
                                           ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif
             otbMultiChannelExtractROI
                -ushort ${current_file}?resol=5
                -ushort ${TEMP}/ioTvJP22TIF_Extract_${current_type}_res5_OUT.tif
                -channels 1
                -time
         )
  ENDIF (NOT PHR_TIFF_FILE)
                           
ENDFOREACH()

ENDIF(OTB_DATA_USE_LARGEINPUT OR OTB_DATA_USE_SPOTPHRINPUT OR OTB_DATA_USE_CNESPHRINPUT)

ENDIF(OTB_USE_JPEG2000)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS14 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(ioTvGDALDriverDoubleWritingTest ${IO_TESTS14}
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

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvReadingComplexDataIntoComplexImage ${IO_TESTS14}
                otbReadingComplexDataIntoComplexImageTest
                ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
                1000 1000
)

ADD_TEST(ioTvWritingComplexDataWithComplexImage ${IO_TESTS14}
--compare-image ${EPSILON_9}
                ${BASELINE}/ioWritingComplexDataWithComplexImageOutput.tif
                ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
                otbWritingComplexDataWithComplexImageTest
                ${LARGEINPUT}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
                ${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
                1000 1000
                500  500
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS15 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADD_TEST(ioTuVectorDataIOFactory ${IO_TESTS15}
         otbVectorDataIOFactory
         ${INPUTDATA}/ToulouseRoad-examples.shp )

ADD_TEST(ioTuOGRVectorDataIO ${IO_TESTS15}
        otbOGRVectorDataIONew )

ADD_TEST(ioTuVectorDataSourceNew ${IO_TESTS15}
        otbVectorDataSourceNew )

ADD_TEST(ioTuVectorDataFileReaderNew ${IO_TESTS15}
        otbVectorDataFileReaderNew )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvSHPVectorDataFileReader ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader.txt
                        ${TEMP}/ioSHPVectorDataFileReader.txt
        otbVectorDataFileReader
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
        ${TEMP}/ioSHPVectorDataFileReader.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvSHPVectorDataFileReader2 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader2.txt
                        ${TEMP}/ioSHPVectorDataFileReader2.txt
        otbVectorDataFileReader
        ${INPUTDATA}/ToulouseRoad-examples.shp
        ${TEMP}/ioSHPVectorDataFileReader2.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvSHPVectorDataFileReader3 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader3.txt
                        ${TEMP}/ioSHPVectorDataFileReader3.txt
        otbVectorDataFileReader
        ${LARGEINPUT}/VECTOR/MidiPyrenees/roads.shp
        ${TEMP}/ioSHPVectorDataFileReader3.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvKMLVectorDataFileReader ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader.txt
                        ${TEMP}/ioTvKMLVectorDataFileReader.txt
        otbVectorDataFileReader
        ${INPUTDATA}/lines.kml
        ${TEMP}/ioTvKMLVectorDataFileReader.txt
)

ADD_TEST(ioTvKMLVectorDataFileReader2 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader2.txt
                        ${TEMP}/ioTvKMLVectorDataFileReader2.txt
        otbVectorDataFileReader
        ${INPUTDATA}/multigeometry.kml
        ${TEMP}/ioTvKMLVectorDataFileReader2.txt
)

ADD_TEST(ioTuVectorDataFileWriterNew ${IO_TESTS15}
        otbVectorDataFileWriterNew )


ADD_TEST(ioTvVectorDataFileWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterOutput.shp
        otbVectorDataFileWriter
        ${TEMP}/ioTvVectorDataFileWriterOutput.shp
)

ADD_TEST(ioTvVectorDataFileWriterTwice ${IO_TESTS15}
        otbVectorDataFileWriter
        ${TEMP}/ioTvVectorDataFileWriterOutput.shp
        )
SET_TESTS_PROPERTIES(ioTvVectorDataFileWriterTwice PROPERTIES DEPENDS ioTvVectorDataFileWriter)

ADD_TEST(ioTvVectorDataFileWriterPolygons ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterPolygonsOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
        otbVectorDataFileWriterPolygons
    ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
    )

ADD_TEST(ioTvVectorDataFileWriterMultiPolygons ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
            ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
        otbVectorDataFileWriterMultiPolygons
    ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
    )


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorDataFileReaderWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileReaderWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
        otbVectorDataFileReaderWriter
            ${LARGEINPUT}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
            ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvVectorDataFileGeoReaderWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/ToulouseRoad-examples.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
        )

ADD_TEST(ioTvVectorDataFileGeoReaderWriter2 ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/Capitole-Shadows.kml
            ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
        )

ADD_TEST(ioTvKMLVectorDataFileGeoReaderWriter ${IO_TESTS15}
        --compare-ascii  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/ToulouseRoad-examples.shp
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
        )

ADD_TEST(ioTvKMLVectorDataFileGeoReaderWriter2 ${IO_TESTS15}
        --compare-ascii  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
        otbVectorDataFileGeoReaderWriter
            ${INPUTDATA}/Capitole-Shadows.kml
            ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
        )

# CanRead OGR GML()
ADD_TEST(ioTuOGRVectorDataIOTestCanReadGML ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/adressPoint.gml)

# CanRead OGR MapInfo()
ADD_TEST(ioTuOGRVectorDataIOTestCanReadMapInfo ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

# CanRead OGR PostGIS()
IF(OTB_USE_PQXX)
#The original string is PG:"dbname='orfeotoolbox_test' host='localhost' port='5432' user='orfeotoolbox_test_user' password='Bidfeud0'"
#but we need to work with cmake/ctest escaping
SET(CONNSTRING "PG:dbname=orfeotoolbox_test\ host=localhost\ port=5432\ user=orfeotoolbox_test_user\ password=Bidfeud0")
ADD_TEST(ioTuOGRVectorDataIOTestCanReadPostGIS ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${CONNSTRING})
ENDIF(OTB_USE_PQXX)

# CanWrite OGR GML()
ADD_TEST(ioTuOGRVectorDataIOCanWriteGML ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/adressPoint.gml)

# CanWrite OGR MapInfo()
ADD_TEST(ioTuOGRVectorDataIOCanWriteMapInfo ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

# Canwrite OGR PostGIS()
IF(OTB_USE_PQXX)
ADD_TEST(ioTuOGRVectorDataIOCanWritePostGIS ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        PG:"dbname='orfeotoolbox_test' host='localhost' port='5432' user='orfeotoolbox_test_user' password='Bidfeud0'")
ENDIF(OTB_USE_PQXX)

ADD_TEST(ioTuKMLVectorDataIO ${IO_TESTS15}
        otbKMLVectorDataIONew )

# CanRead()
ADD_TEST(ioTuKMLVectorDataIOCanRead ${IO_TESTS15}
        otbKMLVectorDataIOTestCanRead
        ${INPUTDATA}/MODIS20040214212340.kml)

# CanWrite()
ADD_TEST(ioTuKMLVectorDataIOCanWrite ${IO_TESTS15}
        otbKMLVectorDataIOTestCanWrite
        ${TEMP}/MODIS20040214212340.kml)

# Read a MODIS kml file and then write it
ADD_TEST(ioTvKMLVectorDataIOFileReaderExample ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/MODIS20040214212340.kml
        ${TEMP}/MODIS20040214212340.kml)

# Read a multigeometry example file and then write it
ADD_TEST(ioTvKMLVectorDataIOFileReaderMultiGeom ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/multigeometry.kml
        ${TEMP}/multigeometry.kml)

# Read a complex line example file and then write it
ADD_TEST(ioTvKMLVectorDataIOFileReaderLines ${IO_TESTS15}
        otbKMLVectorDataIOTestFileReader
        ${INPUTDATA}/lines.kml
        ${TEMP}/lines.kml)

ADD_TEST(ioTuVectorDataKeywordlistNew ${IO_TESTS15}
        otbVectorDataKeywordlistNew
         )

ADD_TEST(ioTvVectorDataKeywordlist ${IO_TESTS15}
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

IF(OTB_DATA_USE_LARGEINPUT)

# Can read a RAD format (ci2)
ADD_TEST(ioTuRADImageIOCanRead_RADCI2 ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${LARGEINPUT}/RAD_CI2/3017_HH.rad)

# Convert a ci2.RAD file into ENVI format
ADD_TEST(ioTvImageFileReaderRADCI22ENVI ${IO_TESTS16}
--compare-image ${EPSILON_9}
        ${BASELINE}/ioImageFileReaderRADCI22ENVI.hdr
        ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr
        otbImageFileReaderRADComplexFloatExtract
        ${LARGEINPUT}/RAD_CI2/3017_HH.rad
        ${TEMP}/ioImageFileReaderRADCI22ENVI.hdr )

ENDIF(OTB_DATA_USE_LARGEINPUT)


# Can read a RAD format (cr4)
ADD_TEST(ioTuRADImageIOCanRead_RADCR4sngl ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_image.rad )

# Can read a multi-image RAD format (cr4)${EXAMPLEDATA}
ADD_TEST(ioTuRADImageIOCanRead_RADCR4mult ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_multimage.rad )

# Convert a cr4.RAD file into tif
ADD_TEST(ioTvImageFileReaderRADCR42TIF ${IO_TESTS16}
        otbImageFileReaderRADComplexFloat
        ${INPUTDATA}/RADCR4_image.rad
        ${TEMP}/ioImageFileReaderRAD2TIF.tif )

# Turn a ci4.RAD file into cr4.file (because of float type)
ADD_TEST(ioTvImageFileReaderCI42CR4 ${IO_TESTS16}
        otbImageFileReaderRADComplexFloat
        ${INPUTDATA}/RADCI4_image.rad
        ${TEMP}/ioImageFileReaderCI42CR4.rad )

# Turn a ci2.RAD file into cr4.file (because of float type)
#ADD_TEST(ioTvImageFileReaderCI22CR4 ${IO_TESTS16}
#        otbImageFileReaderRADComplexFloat
#        /home3/mathieu/guillaume/CD_CNES/RAD_CI2/3017_HH.rad
#        ${TEMP}/ioImageFileReaderCI22CR4.rad )

# Copy cr4.RAD file
ADD_TEST(ioTvImageFileReaderCR42CR8 ${IO_TESTS16}
        otbImageFileReaderRADComplexDouble
        ${INPUTDATA}/RADCR4_image.rad
        ${TEMP}/ioImageFileReaderCR42CR8.rad )

# Convert tif into a RAD file (ci4)
ADD_TEST(ioTvImageFileReaderTIF2RADCI4 ${IO_TESTS16}
        otbImageFileReaderRADComplexInt
        ${INPUTDATA}/RSAT_imagery_VH.tif
        ${TEMP}/ioImageFileReaderTIF2RADCI4.rad )

# Convert png into a RAD file (pha) -> sortie n'est pas lisible
#ADD_TEST(ioTvImageFileReaderTIF2RADPHA ${IO_TESTS16}
#        otbImageFileReaderRADInt
#        ${INPUTDATA}/rcc8_mire1.png
#        ${TEMP}/ioImageFileReaderPNG2RADPHA.rad )

ADD_TEST(ioTvImageSeriesFileReader ${IO_TESTS16}
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
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS17 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_USE_LIBLAS)
ADD_TEST(ioTuPointSetFileReaderNew ${IO_TESTS17}
        otbPointSetFileReaderNew )

ADD_TEST(ioTvPointSetFileReader ${IO_TESTS17}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioPointSetFileReader.txt
                        ${TEMP}/ioPointSetFileReader.txt
        otbPointSetFileReader
  ${INPUTDATA}/srs.las
  ${TEMP}/ioPointSetFileReader.txt)

ADD_TEST(ioTvPointSetFileReader2 ${IO_TESTS17}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioPointSetFileReader2.txt
                        ${TEMP}/ioPointSetFileReader2.txt
        otbPointSetFileReader2
  ${INPUTDATA}/srs.las
  ${TEMP}/ioPointSetFileReader2.txt)

ENDIF(OTB_USE_LIBLAS)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS18 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(ioTuSpotImageMetadataInterfaceNew ${IO_TESTS18}
        otbSpotImageMetadataInterfaceNew
)

ADD_TEST(ioTuFormosatImageMetadataInterfaceNew ${IO_TESTS18}
        otbFormosatImageMetadataInterfaceNew
)

ADD_TEST(ioTuIkonosImageMetadataInterfaceNew ${IO_TESTS18}
        otbIkonosImageMetadataInterfaceNew
)

ADD_TEST(ioTuQuickBirdImageMetadataInterfaceNew ${IO_TESTS18}
        otbQuickBirdImageMetadataInterfaceNew
)

ADD_TEST(ioTuWorldView2ImageMetadataInterfaceNew ${IO_TESTS18}
        otbWorldView2ImageMetadataInterfaceNew
)

ADD_TEST(ioTuTerraSarImageMetadataInterfaceNew ${IO_TESTS18}
        otbTerraSarImageMetadataInterfaceNew
)

ADD_TEST(ioTuPleiadesImageMetadataInterfaceNew ${IO_TESTS18}
        otbPleiadesImageMetadataInterfaceNew
)

IF(OTB_DATA_USE_LARGEINPUT)

#------------ ImageMetadataInterfaceBaseTest ---------------

ADD_TEST(ioTvImageMetadataInterfaceBaseTest_QB ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QB.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QB.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_QBCevennes ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QBCevennes.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_QBMUL ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_QBMUL.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_QBMUL.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_WV2Pan ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomePan.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_WV2Mul ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_WV2RomeMul.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_IKONOS ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_IKONOS.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif
        ${TEMP}/ioTvImageMetadataInterfaceBase_IKONOS.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_SPOT ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_SPOT.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_SPOT.txt
)

# PHR Data
SET( GenericTestPHR_TESTNB 0)

FOREACH( current_file ${GenericTestPHR_DATA_INPUTS} )
  LIST(GET GenericTestPHR_DATA_TYPES   ${GenericTestPHR_TESTNB} current_type   )
  MATH(EXPR GenericTestPHR_TESTNB "${GenericTestPHR_TESTNB} + 1")
   
  ADD_TEST(ioTvImageMetadataInterfaceBaseTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_${current_type}.txt
                                    ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
           otbImageMetadataInterfaceBaseTest
              ${current_file}
              ${TEMP}/ioTvImageMetadataInterfaceBase_${current_type}_OUT.txt
          )

  ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_${current_type} ${IO_TESTS18}
           --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_${current_type}.txt
                                    ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
           otbOpticalImageMetadataInterfaceTest
              ${current_file}
              ${TEMP}/ioTvOpticalImageMetadataInterface_${current_type}_OUT.txt
          )

ENDFOREACH()

ADD_TEST(ioTvImageMetadataInterfaceBaseTest_FORMOSAT2 ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/ioTvImageMetadataInterfaceBase_FORMOSAT2.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_TSX1dims ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1dims.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_TSX1TORONTO ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1TORONTO.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingHHCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingHHCosFile.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingVVCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PANGKALANBUUNUsingVVCosFile.txt
)
ADD_TEST(ioTvImageMetadataInterfaceBaseTest_TSX1PANGKALANBUUNUsingXmlFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
                         ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
        otbImageMetadataInterfaceBaseTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
        ${TEMP}/ioTvImageMetadataInterfaceBase_TSX1PangkalanbuunuUsingXmlFile.txt
)


#------------ OpticalImageMetadataInterfaceTest ---------------

ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_QB ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QB.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QB.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_QBCevennes ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QBCevennes.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_QBMUL ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_QBMUL.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_QBMUL.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_WV2Pan ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomePan.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_WV2Mul ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_MUL/09DEC10103019-M2AS-052298844010_01_P001.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_WV2RomeMul.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_IKONOS ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_IKONOS.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif
        ${TEMP}/ioTvOpticalImageMetadataInterface_IKONOS.txt
)
ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_SPOT ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_SPOT.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_SPOT.txt
)

ADD_TEST(ioTvOpticalImageMetadataInterfaceTest_FORMOSAT2 ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
                         ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
        otbOpticalImageMetadataInterfaceTest
        ${LARGEINPUT}/FORMOSAT/Sudouest_20071013_MS_fmsat/IMAGERY.TIF
        ${TEMP}/ioTvOpticalImageMetadataInterface_FORMOSAT2.txt
)

#------------ SarImageMetadataInterfaceTest ---------------
ADD_TEST(ioTvSarImageMetadataInterfaceTest_TSX1dims ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1dims.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1dims.txt
)
ADD_TEST(ioTvSarImageMetadataInterfaceTest_TSX1TORONTO ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1TORONTO.txt
)
ADD_TEST(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingHHCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingHHCosFile.txt
)
ADD_TEST(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingVVCosFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_VV_SRA_stripFar_008.cos
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PANGKALANBUUNUsingVVCosFile.txt
)
ADD_TEST(ioTvSarImageMetadataInterfaceTest_TSX1PANGKALANBUUNUsingXmlFile ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
                         ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
        otbSarImageMetadataInterfaceTest
        ${LARGEINPUT}/TERRASARX/PANGKALANBUUN/TSX1_SAR__SSC______SM_D_SRA_20080313T221955_20080313T222003.xml
        ${TEMP}/ioTvSarImageMetadataInterface_TSX1PangkalanbuunuUsingXmlFile.txt
)


ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvImageMetadataInterfaceTest ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterface.txt
                         ${TEMP}/ioTvImageMetadataInterface.txt
        otbImageMetadataInterfaceTest2
        ${EXAMPLEDATA}/qb_RoadExtract.tif
        ${TEMP}/ioTvImageMetadataInterface.txt
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS19 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IF(OTB_USE_CURL)

ADD_TEST(ioTvImageFileReaderServerNameHTTPTest ${IO_TESTS19}
        otbImageFileReaderServerName
        http://tile.openstreetmap.org/)
SET_PROPERTY(TEST ioTvImageFileReaderServerNameHTTPTest PROPERTY RESOURCE_LOCK web_access)

ADD_TEST(ioTvImageFileReaderServerNameOTBTest ${IO_TESTS19}
        otbImageFileReaderServerName
        ${OTB_DATA_ROOT}/Examples/osmfile.otb)
SET_PROPERTY(TEST ioTvImageFileReaderServerNameOTBTest PROPERTY RESOURCE_LOCK web_access)

# We do not perform regression testing here because images on server might change
ADD_TEST(ioTvTileMapImageIOFile ${IO_TESTS19}
  otbTileMapImageIOTest
    ${OTB_DATA_ROOT}/Examples/osmfile.otb
    ${TEMP}/ioTvTileMapImageIOFileOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)
SET_PROPERTY(TEST ioTvTileMapImageIOFile PROPERTY RESOURCE_LOCK web_access)

ADD_TEST(ioTvTileMapImageIOWeb ${IO_TESTS19}
  otbTileMapImageIOTest
    http://tile.openstreetmap.org/
    ${TEMP}/ioTvTileMapImageIOWebOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)
SET_PROPERTY(TEST ioTvTileMapImageIOWeb PROPERTY RESOURCE_LOCK web_access)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvTileMapWriter ${IO_TESTS19}
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
ENDIF(OTB_DATA_USE_LARGEINPUT)



# OSM Data To VectorData Generator

ADD_TEST(ioTuOSMToVectorDataGenerator ${IO_TESTS19}
  otbOSMToVectorDataGeneratorNew
)

ADD_TEST(ioTvOSMToVectorDataGeneratorTest ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterTest_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
  otbOSMToVectorDataGeneratorTest
  ${INPUTDATA}/qb_road.osm
  ${TEMP}/ioTvOSMToVectorDataFilterTest_highway.shp
)
SET_PROPERTY(TEST ioTvImageFileReaderServerNameHTTPTest PROPERTY RESOURCE_LOCK web_access)

ADD_TEST(ioTvOSMToVectorDataGeneratorByName ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
  ${BASELINE_FILES}/ioTvOSMToVectorDataFilterByName_highway.shp
  ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
  otbOSMToVectorDataGeneratorByName
  -osm ${INPUTDATA}/qb_road.osm
  -vd ${TEMP}/ioTvOSMToVectorDataFilterByName_highway.shp
)
SET_PROPERTY(TEST ioTvOSMToVectorDataGeneratorByName PROPERTY RESOURCE_LOCK web_access)

# ImageToOSMVectorData
ADD_TEST(ioTuImageToOSMVectorDataGeneratorNew ${IO_TESTS19}
  otbImageToOSMVectorDataGeneratorNew
)

ADD_TEST(ioTvImageToOSMVectorDataGenerator ${IO_TESTS19}
  --compare-ogr ${EPSILON_9}
    ${BASELINE_FILES}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
    ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  otbImageToOSMVectorDataGenerator
  -in  ${EXAMPLEDATA}/qb_RoadExtract.tif
  -osm ${INPUTDATA}/qb_road.osm
  -out ${TEMP}/ioTvImageToOSMVectorDataGenerator_highway_motorway.shp
  -key highvay,motorway
)
SET_PROPERTY(TEST ioTvImageToOSMVectorDataGenerator PROPERTY RESOURCE_LOCK web_access)

ENDIF(OTB_USE_CURL)

#----------------------------------------------------------------------------------

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS20 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvImageFileReaderTestSensorPixelValueQB ${IO_TESTS20}
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

ADD_TEST(ioTvImageFileReaderTestSensorPixelValueIKONOS ${IO_TESTS20}
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

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbDefaultImageMetadataInterfaceFactoryNew )

ADD_TEST(ioTuDefaultImageMetadataInterface ${IO_TESTS20}
        otbDefaultImageMetadataInterface )

ADD_TEST(ioTuOpticalDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterfaceFactoryNew )

ADD_TEST(ioTuOpticalDefaultImageMetadataInterfaceNew ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterfaceNew )

ADD_TEST(ioTuOpticalDefaultImageMetadataInterface ${IO_TESTS20}
        otbOpticalDefaultImageMetadataInterface )

ADD_TEST(ioTuSarDefaultImageMetadataInterfaceFactoryNew ${IO_TESTS20}
        otbSarDefaultImageMetadataInterfaceFactoryNew )

ADD_TEST(ioTuSarDefaultImageMetadataInterfaceNew ${IO_TESTS20}
        otbSarDefaultImageMetadataInterfaceNew )

ADD_TEST(ioTuSarDefaultImageMetadataInterface ${IO_TESTS20}
        otbSarDefaultImageMetadataInterface )

ADD_TEST(ioTuScalarBufferToImageFileWriter ${IO_TESTS20}
        otbScalarBufferToImageFileWriterNew )

ADD_TEST(ioTvScalarBufferToImageFileWriterTest ${IO_TESTS20}
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

ADD_TEST(ioTuMapFileProductWriterNew ${IO_TESTS21}
  otbMapFileProductWriterNew
)

ADD_TEST(ioTvMapFileProductWriter ${IO_TESTS21}
  otbMapFileProductWriter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/mapserver/toulouseExtract.map
  ${TEMP}/mapserver/result               # Shape index path
  http://localhost/cgi-bin/mapserv # cgi executable path
  256
)

ADD_TEST(ioTuKmzProductWriterNew ${IO_TESTS21}
  otbKmzProductWriterNew
)

ADD_TEST(ioTvKmzProductWriterWithGCP ${IO_TESTS21}
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

ADD_TEST(ioTvKmzProductWriterWithGCP_Logo_Legend ${IO_TESTS21}
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
SET_TESTS_PROPERTIES(ioTvKmzProductWriterWithGCP_Logo_Legend
  PROPERTIES DEPENDS ioTvKmzProductWriterWithGCP)


ADD_TEST(ioTuTileMapImageIOHelperNew ${IO_TESTS21}
  otbTileMapImageIOHelperNew
)

ADD_TEST(ioTvTileMapImageIOHelperTest ${IO_TESTS21}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTileMapImageIOHelperTest.txt
                         ${TEMP}/ioTvTileMapImageIOHelperTest.txt
         otbTileMapImageIOHelperTest
              ${TEMP}/ioTvTileMapImageIOHelperTest.txt
)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS22 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Tests about multidataset file like hdf4 or hdf5 (hdf, hdf5, hdf4, h5, h4, he5, he4)

IF(CHECK_GDAL_BUILT_WITH_HDF)

IF(CHECK_HDF4OPEN_SYMBOL)
# Tests to check if we can open hdf files using a otb::VectorImage specifying the sub-dataset index.
# no access to data only PrintSelf() method.
ADD_TEST(ioTvMultiDatasetReading1 ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading1.txt
                             ${TEMP}/ioTvMultiDatasetReading1.txt
      otbVectorImageTest
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=0
      ${TEMP}/ioTvMultiDatasetReading1.txt
      )
ADD_TEST(ioTvMultiDatasetReading2 ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReading2.txt
                             ${TEMP}/ioTvMultiDatasetReading2.txt
      otbVectorImageTest
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
      ${TEMP}/ioTvMultiDatasetReading2.txt
      )
      
# Test to check behavior of function readerGDAL->GetSubDatasetInfo() which return info about subdataset.
ADD_TEST(ioTvMultiDatasetReadingInfo ${IO_TESTS22}
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/ioTvMultiDatasetReadingInfo.txt
                             ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
      otbMultiDatasetReadingInfo
      ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf
      ${TEMP}/ioTvMultiDatasetReadingInfoOut.txt
      )

# Tests which read data in HDF files.
ADD_TEST(ioTvMultiChannelROI_HDF4_2_TIF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1.tif
                     ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
         otbMultiChannelExtractROI
         ${INPUTDATA}/MOD09Q1G_EVI.A2006233.h07v03.005.2008338190308.hdf?&sdataidx=5
         ${TEMP}/ioExtractROI_HDF2TIF_MOD09Q1G_20_25_100_150_channel_1_OUT.tif
         -startX 1400 -startY 3700 -sizeX 100 -sizeY 150
         -channels 1 )
ENDIF(CHECK_HDF4OPEN_SYMBOL)

ADD_TEST(ioTvMultiChannelROI_HDF5_2_TIF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1.tif
                     ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
         otbMultiChannelExtractROI
         ${INPUTDATA}/GSSTF_NCEP.2b.2008.12.31.he5?&sdataidx=3
         ${TEMP}/ioExtractROI_HDF2TIF_GSSTF_NCEP_100_20_200_100_channel_1_OUT.tif
         -startX 100 -startY 20 -sizeX 200 -sizeY 100
         -channels 1 )

ENDIF(CHECK_GDAL_BUILT_WITH_HDF)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests23 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Tests about complex reading

SET(INPUTFILE_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
SET(READING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
SET(NBBANDS_LIST "2" "3" "4")

FOREACH(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

  FOREACH(READING_PIXELTYPE ${READING_PIXELTYPES_LIST})

    ADD_TEST(ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandScalarToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    ADD_TEST(ioTvMonobandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandScalarToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monoband${INPUTFILE_PIXELTYPE}.tif)

    ADD_TEST(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageScalar${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToImageScalar${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    ADD_TEST(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    ADD_TEST(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageScalar${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToVectorImageScalar${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    ADD_TEST(ioTvMonobandComplex${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMonobandComplexToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/monobandComplex${INPUTFILE_PIXELTYPE}.tif) #hdr)

    # Weird case : only the first 2 bands used
    ADD_TEST(ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMultibandScalarToImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    # Weird case : one scalar band -> one complex band with imag=0
    # but interleaving real and imaginary parts can only work for an even nb of bands
    ADD_TEST(ioTvMultibandScalar${INPUTFILE_PIXELTYPE}ToVectorImageComplex${READING_PIXELTYPE}
             ${IO_TESTS23}
             otbMultibandScalarToVectorImageComplex${READING_PIXELTYPE}
             ${INPUTDATA}/multiband${INPUTFILE_PIXELTYPE}.tif)

    FOREACH(NBBANDS ${NBBANDS_LIST})

      # Weird case : need specifications to write the test
      # ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageScalar${READING_PIXELTYPE}
      #          ${IO_TESTS23}
      #          otbMultibandComplexToImageScalar${READING_PIXELTYPE}
      #          ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      # Size: 100x100 is to large to store pixel values coded as short.
      IF ( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageScalar${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageScalar${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      ELSE ( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageScalar${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageScalar${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

      ENDIF (  NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

      # Weird case : need specifications to write the test
      # [missing impl]
      # ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToImageComplex${READING_PIXELTYPE})

      # Size: 100x100 is to large to store pixel values coded as short.
      IF ( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageComplex${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageComplex${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands.tif) #hdr)

      ELSE ( NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

        ADD_TEST(ioTvMultibandComplex${INPUTFILE_PIXELTYPE}${NBBANDS}BandsToVectorImageComplex${READING_PIXELTYPE}
                 ${IO_TESTS23}
                 otbMultibandComplexToVectorImageComplex${READING_PIXELTYPE}
                 ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_${NBBANDS}bands_50x50.tif) #hdr)

      ENDIF (  NOT ${INPUTFILE_PIXELTYPE} MATCHES "Short" )

    ENDFOREACH(NBBANDS ${NBBANDS_LIST})

  ENDFOREACH(READING_PIXELTYPE ${READING_PIXELTYPES_LIST})
ENDFOREACH(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})


# Tests with GDAL only to read complex data

#SET(INPUTFILE_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
SET(INPUTFILE_PIXELTYPES_LIST "Float" "Double")

FOREACH(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

ADD_TEST(ioTvGDALReadPxlComplex${INPUTFILE_PIXELTYPE} ${IO_TESTS23}
         otbGDALReadPxlComplex${INPUTFILE_PIXELTYPE}
         ${INPUTDATA}/multibandComplex${INPUTFILE_PIXELTYPE}_3bands.tif
         1 5 10 2) #old file hdr sans extesions

ENDFOREACH(INPUTFILE_PIXELTYPE ${INPUTFILE_PIXELTYPES_LIST})

# Tests about writing Scalar / Complex data

SET(DATA_PIXELTYPES_LIST "Int" "Short" "Float" "Double")
SET(WRITING_PIXELTYPES_LIST "Int" "Short" "Float" "Double")

FOREACH(DATA_PIXELTYPE ${DATA_PIXELTYPES_LIST})

  ADD_TEST(ioTvWriteImageScalar${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
           otbImageScalarFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandOut${DATA_PIXELTYPE}.tif
              100
           )

  ADD_TEST(ioTvWriteImageComplex${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
           otbImageComplexFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandComplexOut${DATA_PIXELTYPE}.tif
              100
           )

  FOREACH(NBBANDS ${NBBANDS_LIST})
    IF ( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      ADD_TEST(ioTvWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

      ADD_TEST(ioTvWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

    ELSE ( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      ADD_TEST(ioTvWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
             ${IO_TESTS23}
             --compare-image ${NOTOL}
                ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
             otbVectorImageFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                ${TEMP}/multibandOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                50
                ${NBBANDS}
             )

      ADD_TEST(ioTvWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
               otbVectorImageFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  50
                  ${NBBANDS}
               )

    ENDIF (  NOT ${DATA_PIXELTYPE} MATCHES "Short" )
  ENDFOREACH(NBBANDS ${NBBANDS_LIST})

############
# Tests about Streaming writing Scalar / Complex data
  ADD_TEST(ioTvStreamingWriteImageScalar${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monoband${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
           otbImageScalarStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandStrOut${DATA_PIXELTYPE}.tif
              100
           )

  ADD_TEST(ioTvStreamingWriteImageComplex${DATA_PIXELTYPE}
           ${IO_TESTS23}
           --compare-image ${NOTOL}
              ${INPUTDATA}/monobandComplex${DATA_PIXELTYPE}.tif
              ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
           otbImageComplexStreamingFileWriterTestWithoutInput${DATA_PIXELTYPE}
              ${TEMP}/monobandStrComplexOut${DATA_PIXELTYPE}.tif
              100
           )

  FOREACH(NBBANDS ${NBBANDS_LIST})
    IF ( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      ADD_TEST(ioTvStreamingWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
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

ADD_TEST(raTuSpectralSensitivityReaderNew ${IO_TESTS23}
     otbSpectralSensitivityReaderNew
)

ADD_TEST(raTvSpectralSensitivityReaderTest ${IO_TESTS23}
     --compare-ascii ${EPSILON_5}
        ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
        ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
     --ignore-lines-with 2 DataPath FileName
     otbSpectralSensitivityReaderTest
        ${INPUTDATA}/Radiometry
        ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/rcTuSpectralSensitivityReaderTest.txt
)

ADD_TEST(raTvSpectralSensitivityReaderGenericTest ${IO_TESTS23}
     --compare-ascii ${EPSILON_5}
      ${BASELINE_FILES}/raTvSpectralSensitivityReaderGenericTest.txt
        ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
     --ignore-lines-with 2 DataPath FileName
     otbSpectralSensitivityReaderGenericTest
    ${INPUTDATA}/Radiometry/SPOT5/HRG1/rep6S.dat
    ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
        ${TEMP}/raTvSpectralSensitivityReaderGenericTest.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
  ADD_TEST(raTvSpectralSensitivityReaderSpot4Test ${IO_TESTS23}
    --compare-ascii ${EPSILON_5}
    ${BASELINE_FILES}/raTvSpectralSensitivityReaderSpot4Test.txt
    ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
    --ignore-lines-with 2 DataPath FileName
    otbSpectralSensitivityReaderGenericTest
    ${INPUTDATA}/Radiometry/SPOT4/HRVIR1/rep6S.dat
    ${LARGEINPUT}/SPOT4/ROMANIA/raTvS4_20020518_Reflectance.img
    ${TEMP}/raTvSpectralSensitivityReaderSpot4Test.txt
    )
ENDIF(OTB_DATA_USE_LARGEINPUT)

      ADD_TEST(ioTvStreamingWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
               otbVectorImageStreamingFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands.tif
                  100
                  ${NBBANDS}
               )

    ELSE ( NOT ${DATA_PIXELTYPE} MATCHES "Short" )

      ADD_TEST(ioTvStreamingWriteVectorImageScalar${DATA_PIXELTYPE}${NBBANDS}Bands
             ${IO_TESTS23}
             --compare-image ${NOTOL}
                ${INPUTDATA}/multiband${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
             otbVectorImageStreamingFileWriterScalarTestWithoutInput${DATA_PIXELTYPE}
                ${TEMP}/multibandStrOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                50
                ${NBBANDS}
             )


ADD_TEST(raTuFilterFunctionValuesNew ${IO_TESTS23}
     otbFilterFunctionValuesNew
)

ADD_TEST(raTuWavelengthSpectralBandsNew ${IO_TESTS23}
     otbWavelengthSpectralBandsNew
)

      ADD_TEST(ioTvStreamingWriteVectorImageComplex${DATA_PIXELTYPE}${NBBANDS}Bands
               ${IO_TESTS23}
               --compare-image ${NOTOL}
                  ${INPUTDATA}/multibandComplex${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
               otbVectorImageStreamingFileWriterComplexTestWithoutInput${DATA_PIXELTYPE}
                  ${TEMP}/multibandStrComplexOut${DATA_PIXELTYPE}_${NBBANDS}bands_50x50.tif
                  50
                  ${NBBANDS}
               )

    ENDIF (  NOT ${DATA_PIXELTYPE} MATCHES "Short" )

  ENDFOREACH(NBBANDS ${NBBANDS_LIST})

ENDFOREACH(DATA_PIXELTYPE ${DATA_PIXELTYPES_LIST})

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests24 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADD_TEST(ioTvExtendedFilenameToReaderOptions_NoOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
         ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
	otbExtendedFilenameToReaderOptions
        /home/data/filename.tif
        ${TEMP}/ioTvExtendedFilenameToReaderOptions_NoOptions.txt
        )

ADD_TEST(ioTvExtendedFilenameToReaderOptions_FullOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
         ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
    otbExtendedFilenameToReaderOptions
        /home/data/filename.tif?&geom=/home/dev/custom.geom&sdataidx=2&resol=4&skipcarto=On
        ${TEMP}/ioTvExtendedFilenameToReaderOptions_FullOptions.txt
        )
        
ADD_TEST(ioTvExtendedFilenameToWriterOptions_NoOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
         ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
	otbExtendedFilenameToWriterOptions
        /home/data/filename.tif
        ${TEMP}/ioTvExtendedFilenameToWriterOptions_NoOptions.txt
        )

ADD_TEST(ioTvExtendedFilenameToWriterOptions_FullOptions ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
         ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
	otbExtendedFilenameToWriterOptions
        /home/data/filename.tif?&writegeom=ON&gdal:co:QUALITY=75&gdal:co:TILED=YES&gdal:co:BLOCKYSIZE=1024
        ${TEMP}/ioTvExtendedFilenameToWriterOptions_FullOptions.txt
        )
        
ADD_TEST(ioTvImageFileReaderExtendedFileName_GEOM ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderWithExternalGEOMFile.txt
         ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
        ${TEMP}/ioImageFileReaderWithExternalGEOMFile.txt
        ${TEMP}/ioImageFileReaderWithExternalGEOMFile_pr.txt        
        )

ADD_TEST(ioTvImageFileReaderExtendedFileName_SkipPR ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_SkipPR.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipcarto=true
        ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR_kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_SkipPR.txt
        )

ADD_TEST(ioTvImageFileReaderExtendedFileName_SkipGeom ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true
        ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_Skipgeom_pr.txt
        )

ADD_TEST(ioTvImageFileReaderExtendedFileName_mix1 ${IO_TESTS24}
--compare-ascii ${NOTOL}
         ${BASELINE}/ioImageFileReaderExtendedFileName_mix1pr.txt
         ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
	otbImageFileReaderWithExtendedFilename
        ${INPUTDATA}/bretagne.j2k?&skipcarto=true&resol=2
        ${TEMP}/ioImageFileReaderExtendedFileName_mix1kwl.txt
        ${TEMP}/ioImageFileReaderExtendedFileName_mix1pr.txt
        )

ADD_TEST(ioTvImageFileWriterExtendedFileName_GEOM ${IO_TESTS24}
--ignore-order --compare-ascii ${NOTOL}
         ${INPUTDATA}/ToulouseExtract_WithGeom.geom         
         ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.geom
--ignore-lines-with 2 ce90_absolute ce90_relative
	otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/ToulouseExtract_WithGeom.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_GEOM.tif?&writegeom=ON
        )
        
ADD_TEST(ioTvImageFileWriterExtendedFileName_gdalco ${IO_TESTS24}
--compare-image ${NOTOL}
         ${BASELINE}/ioTvGDALImageIO_JPEG_20.jpg      
         ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg
	otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_gdalco.jpg?&gdal:co:QUALITY=20
        )

# Streaming extended options tests

SET(streaming_sizevalue_auto "8")
SET(streaming_sizevalue_nbsplits "10")
SET(streaming_sizevalue_height "100")

ADD_TEST(ioTvImageFileWriterExtendedFileName_StreamingNone ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_streamingNone.tif?&streaming:type=none)

ADD_TEST(ioTvImageFileWriterExtendedFileName_StreamingAuto ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/ioImageFileWriterExtendedFileName_streamingAuto.tif?&streaming:type=auto&streaming:sizevalue=${streaming_sizevalue_auto})


FOREACH(streaming_type "Tiled" "Stripped")

FOREACH(streaming_sizemode "Auto" "Nbsplits" "Height")

STRING(TOLOWER ${streaming_type} type)
STRING(TOLOWER ${streaming_sizemode} sizemode)

SET(test_name "ioTvImageFileWriterExtendedFileName_Streaming${streaming_type}${streaming_sizemode}")


ADD_TEST(${test_name} ${IO_TESTS24}
--compare-image ${NOTOL}
         ${INPUTDATA}/maur_rgb_24bpp.tif
         ${TEMP}/${test_name}.tif
  otbImageFileWriterWithExtendedFilename
        ${INPUTDATA}/maur_rgb_24bpp.tif
        ${TEMP}/${test_name}.tif?&streaming:type=${type}&streaming:sizemode=${sizemode}&streaming:sizevalue=${streaming_sizevalue_${sizemode}})

ENDFOREACH(streaming_sizemode)

ENDFOREACH(streaming_type)



        
#----------------------------------------------------------------------------------
SET(BasicIO_SRCS1
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
SET(BasicIO_SRCS2
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
SET(BasicIO_SRCS3
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
IF(NOT OTB_TESTING_STANDALONE)
SET(BasicIO_SRCS4
otbIOTests4.cxx
otbImageKeywordlist.cxx
otbOssimJpegFileRessourceLeakTest.cxx
)
ENDIF(NOT OTB_TESTING_STANDALONE)

SET(BasicIO_SRCS5
otbIOTests5.cxx
otbGDALImageIOTestCanRead.cxx
otbThreadsTest.cxx
)

SET(BasicIO_SRCS6
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

SET(BasicIO_SRCS7
otbIOTests7.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterTest.cxx
otbImageFileWriterRGBTest.cxx
)

SET(BasicIO_SRCS8
otbIOTests8.cxx
otbCoordinateToNameTest.cxx
)

SET(BasicIO_SRCS9
otbIOTests9.cxx
otbImageTest.cxx
otbVectorImageTest.cxx
otbStreamingImageFilterTest.cxx
otbVectorImageFileReaderWriterTest.cxx
otbStreamingShortImageFileWriterTest.cxx
otbComplexImageTests.cxx
)

SET(BasicIO_SRCS10
otbIOTests10.cxx
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions.cxx
)

SET(BasicIO_SRCS11
otbIOTests11.cxx
otbImageMetadataFileWriterTest.cxx
otbImageMetadataStreamingFileWriterTest.cxx
otbPipelineMetadataHandlingTest.cxx
otbPipelineMetadataHandlingWithUFFilterTest.cxx
)

SET(BasicIO_SRCS12
otbIOTests12.cxx
otbDEMToImageGeneratorNew.cxx
otbDEMToImageGeneratorTest.cxx
otbDEMToImageGeneratorFromImageTest.cxx
otbMapProjectionAdapterTest.cxx
)


IF(OTB_USE_JPEG2000)
#Reserved for jpeg 2000
SET(BasicIO_SRCS13
otbIOTests13.cxx
otbJPEG2000ImageIOTestCanRead.cxx
otbJPEG2000ImageIOTestCanWrite.cxx
otbVectorImageTest.cxx
otbMultiResolutionReadingInfo.cxx
otbGenerateClassicalQLWithJPEG2000.cxx
)
ENDIF(OTB_USE_JPEG2000)


SET(BasicIO_SRCS14
otbIOTests14.cxx
otbGDALDriverDoubleWritingTest.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbWritingComplexDataWithComplexImageTest.cxx
)



SET(BasicIO_SRCS15
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

SET(BasicIO_SRCS16
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

#Reserved for liblas
SET(BasicIO_SRCS17
otbIOTests17.cxx
otbPointSetFileReaderNew.cxx
otbPointSetFileReader.cxx
otbPointSetFileReader2.cxx
)

SET(BasicIO_SRCS18
otbIOTests18.cxx
otbSpotImageMetadataInterfaceNew.cxx
otbPleiadesImageMetadataInterfaceNew.cxx
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

IF(OTB_USE_CURL)
#Reserved for CURL
SET(BasicIO_SRCS19
otbIOTests19.cxx
otbImageFileReaderServerName.cxx
otbTileMapImageIOTest.cxx
otbTileMapWriter.cxx
otbOSMDataToVectorDataTests.cxx
otbImageToOSMVectorDataGenerator.cxx
)
ENDIF(OTB_USE_CURL)

SET(BasicIO_SRCS20
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

SET(BasicIO_SRCS21
otbIOTests21.cxx
otbMapFileProductWriterNew.cxx
otbMapFileProductWriter.cxx
otbKmzProductWriter.cxx
otbTileMapImageIOHelperTest.cxx
otbComplexImageManipulationTest.cxx
otbGDALReadPxlComplex.cxx
)

IF(CHECK_GDAL_BUILT_WITH_HDF)
SET(BasicIO_SRCS22
otbIOTests22.cxx
otbVectorImageTest.cxx
otbMultiDatasetReadingInfo.cxx
)
ENDIF(CHECK_GDAL_BUILT_WITH_HDF)

SET(BasicIO_SRCS23
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

SET(BasicIO_SRCS24
otbIOTests24.cxx
otbExtendedFilenameToReaderOptionsTest.cxx
otbExtendedFilenameToWriterOptionsTest.cxx
otbExtendedFilenameTest.cxx
)

IF(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OSSIM_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
ENDIF(OTB_USE_EXTERNAL_OSSIM)

OTB_ADD_EXECUTABLE(otbIOTests1 "${BasicIO_SRCS1}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests2 "${BasicIO_SRCS2}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests3 "${BasicIO_SRCS3}" "OTBIO;OTBTesting")

IF(NOT OTB_TESTING_STANDALONE)
OTB_ADD_EXECUTABLE(otbIOTests4 "${BasicIO_SRCS4}" "OTBIO;OTBTesting")
ENDIF(NOT OTB_TESTING_STANDALONE)

OTB_ADD_EXECUTABLE(otbIOTests5 "${BasicIO_SRCS5}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests6 "${BasicIO_SRCS6}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests7 "${BasicIO_SRCS7}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests8 "${BasicIO_SRCS8}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests9 "${BasicIO_SRCS9}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests10 "${BasicIO_SRCS10}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests11 "${BasicIO_SRCS11}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests12 "${BasicIO_SRCS12}" "OTBIO;OTBTesting")

IF(OTB_USE_JPEG2000)
   IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DOPJ_STATIC)
   ENDIF(NOT BUILD_SHARED_LIBS)

   OTB_ADD_EXECUTABLE(otbIOTests13 "${BasicIO_SRCS13}" "OTBIO;OTBTesting")
ENDIF(OTB_USE_JPEG2000)

OTB_ADD_EXECUTABLE(otbIOTests14 "${BasicIO_SRCS14}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests15 "${BasicIO_SRCS15}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests16 "${BasicIO_SRCS16}" "OTBIO;OTBTesting")

IF(OTB_USE_LIBLAS)
  OTB_ADD_EXECUTABLE(otbIOTests17 "${BasicIO_SRCS17}" "OTBIO;OTBTesting")
ENDIF(OTB_USE_LIBLAS)

OTB_ADD_EXECUTABLE(otbIOTests18 "${BasicIO_SRCS18}" "OTBIO;OTBTesting")

IF(OTB_USE_CURL)
  OTB_ADD_EXECUTABLE(otbIOTests19 "${BasicIO_SRCS19}" "OTBIO;OTBTesting")
ENDIF(OTB_USE_CURL)

OTB_ADD_EXECUTABLE(otbIOTests20 "${BasicIO_SRCS20}" "OTBIO;OTBTesting")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

OTB_ADD_EXECUTABLE(otbIOTests21 "${BasicIO_SRCS21}" "OTBIO;OTBTesting")

IF(CHECK_GDAL_BUILT_WITH_HDF)
  OTB_ADD_EXECUTABLE(otbIOTests22 "${BasicIO_SRCS22}" "OTBIO;OTBTesting")
ENDIF(CHECK_GDAL_BUILT_WITH_HDF)

OTB_ADD_EXECUTABLE(otbIOTests23 "${BasicIO_SRCS23}" "OTBIO;OTBTesting")

OTB_ADD_EXECUTABLE(otbIOTests24 "${BasicIO_SRCS24}" "OTBIO;OTBTesting")
