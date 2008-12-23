# $Id: CMakeLists.txt 4312 2008-09-26 13:46:16Z guillaume $

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance on  pixel image diff
SET(TOL 0.000000001)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

# !!!!!  Pointe sur un exe généré dans le repertoire Common
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

IF(OTB_COMPILE_JPEG2000)
   SET(IO_TESTS13    ${CXX_TEST_PATH}/otbIOTests13)
ENDIF(OTB_COMPILE_JPEG2000)

SET(IO_TESTS14    ${CXX_TEST_PATH}/otbIOTests14)
SET(IO_TESTS15    ${CXX_TEST_PATH}/otbIOTests15)
SET(IO_TESTS16    ${CXX_TEST_PATH}/otbIOTests16)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)

# ---  RADARSAT (GDAL)  ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT2ENVI ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROI
         -ushort ${LARGEDATA}/RADARSAT1/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  Nom r�pertoire ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI ${COMMON_TESTS3}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
#                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         -startX 0 -startY 0 -sizeX 337 -sizeY 280 )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  ENVI (GDAL) ---
ADD_TEST(ioTvMultiMonoChannelROI_ENVI2PNG ${COMMON_TESTS3}
   --compare-image ${TOL}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
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
ADD_TEST(ioTvImageFileReaderERS ${IO_TESTS1}
   --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderERS_SLCI.tif
                            ${TEMP}/ioImageFileReaderERS_SLCI.tif
         otbImageFileReaderERS
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioImageFileReaderERS_SLCI.tif)

ADD_TEST(ioTvImageFileReaderRADARSAT ${IO_TESTS1}
   --compare-image ${TOL} ${BASELINE}/ioImageFileReaderRADARSAT.png
                           ${TEMP}/ioImageFileReaderRADARSAT.png
         otbImageFileReaderERS
         ${LARGEDATA}/RADARSAT1/SCENE01/
         ${TEMP}/ioImageFileReaderRADARSAT.png)

ADD_TEST(ioTvImageFileReaderPALSAR ${IO_TESTS1}
   --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderPALSAR_l1.1.tif
                            ${TEMP}/ioImageFileReaderPALSAR_l1.1.tif
         otbImageFileReaderERS
         ${LARGEDATA}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/ioImageFileReaderPALSAR_l1.1.tif)

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
  --compare-image ${TOL}        ${INPUTDATA}/poupees_1canal.c1.hdr
                                ${TEMP}/ioImageFileReaderHDR2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.c1.hdr
        ${TEMP}/ioImageFileReaderHDR2LUM.lum )

ADD_TEST(ioTvImageFileReaderPNG2LUM ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2LUM.lum )

ADD_TEST(ioTvImageFileReaderLUM2LUM ${IO_TESTS1}
  --compare-image ${TOL}        ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioImageFileReaderLUM2LUM.lum
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderPNG2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2LUM.lum )

ADD_TEST(ioTvImageFileWriterLUM2HDR ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderLUM2HDR.hdr
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderLUM2LUM.lum
        ${TEMP}/ioImageFileReaderLUM2HDR.hdr )

# --- Image I2
ADD_TEST(ioTvImageFileReaderShortLUM2HDR ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2LUM ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum
        otbShortImageIOTest
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum )

ADD_TEST(ioTvImageFileReaderShortLUM2LUM ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
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
  --compare-image ${TOL}        ${BASELINE}/QB_Toulouse_Ortho_PAN.img
                                ${TEMP}/ioImageFileReaderMW2TIF.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderMW2TIF.tif )

ADD_TEST(ioTvImageFileReaderMW2TIFMW ${IO_TESTS1}
  --compare-image ${TOL}        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw
                                ${TEMP}/ioImageFileReaderMW2TIF.tif
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderMW2TIF.tif )



# CanWrite()
ADD_TEST(ioTuMWImageIOCanWrite ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.mw)

ADD_TEST(ioTvImageFileReaderTIF2MW ${IO_TESTS1}
  --compare-image ${TOL}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                ${TEMP}/ioImageFileReaderTIF2MW.mw
        otbImageFileReaderTestFloat
        ${BASELINE}/QB_Toulouse_Ortho_PAN.mw
        ${TEMP}/ioImageFileReaderTIF2MW.mw )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntLUM2HDR ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.lum
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderIntHDR2LUM ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum )

# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatLUM2HDR ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.lum
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderFloatHDR2LUM ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum )

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleLUM2HDR ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.lum
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderDoubleHDR2LUM ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum )




ADD_TEST(ioTvImageFileReaderFloatPNG2LUM ${IO_TESTS2}
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum
        otbFloatImageIOTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum )


# ---  STREAMING : LUM  ---
ADD_TEST(ioTvStreamingImageFilterLUM2PNG ${IO_TESTS2}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFilterTestLUM.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFilterTestLUM.png
         )

# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithoutStreaming ${IO_TESTS2}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         0 # Streaming
         )
# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithStreaming ${IO_TESTS2}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         1 # Streaming
         10
         )

# ---  STREAMING  Pipeline Example ---
ADD_TEST(ioTvStreamingPipeline_TIF2PNG ${IO_TESTS2}
         otbPipeline
         ${INPUTDATA}/cthead1.png #ROI_IKO_PAN_LesHalles.tif
         ${TEMP}/ioPipeline.png
         )


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

ADD_TEST(ioTvImageFileReaderPNG2BSQ ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2BSQ.hd
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2BSQ.hd )

ADD_TEST(ioTvImageFileReaderPNG2ENVI ${IO_TESTS3}
  --compare-image ${TOL}        ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioImageFileReaderPNG2ENVI.hdr
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2ENVI.hdr )

ADD_TEST(ioTvImageFileReaderBSQ2PNG ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_1canal.hd
                                ${TEMP}/ioImageFileReaderBSQ2PNG.png
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.hd
        ${TEMP}/ioImageFileReaderBSQ2PNG.png )

ADD_TEST(ioTvImageFileReaderRGB_PNG2BSQ ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/sbuv.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd )

ADD_TEST(ioTvImageFileReaderRGB_PNG2ENVI ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/sbuv.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr )

# --- Image I2
ADD_TEST(ioTvImageFileReaderShortRGB_BSQ2HDR ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        otbShortRGBImageIOTest
        ${INPUTDATA}/poupees_I2.hd
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2BSQ ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd
        otbShortRGBImageIOTest
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd )

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntBSQ2HDR ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.hd
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderIntHDR2BSQ ${IO_TESTS3}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatBSQ2HDR ${IO_TESTS4}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.hd
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderFloatHDR2BSQ ${IO_TESTS4}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd )

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleBSQ2HDR ${IO_TESTS4}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.hd
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderDoubleHDR2BSQ ${IO_TESTS4}
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd )


# ---  STREAMING FILTER : BSQ  ---
ADD_TEST(ioTvStreamingImageFilterBSQ2PNG ${IO_TESTS4}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFilterTestBSQ.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFilterTestBSQ.png
         )

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS4}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         0 # Streaming
         )
# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS4}
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         1 # Streaming
         10
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------            otb::GDALImageIO   ------------------------------

# READER

ADD_TEST(ioTuGDALImageIOCanRead_ENVI ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupees_1canal.c1.hdr )
        ${INPUTDATA}/poupees_1canal.c1 )

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuGDALImageIOCanRead_RADARSAT ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEDATA}/RADARSAT1/SCENE01/ )
        ${LARGEDATA}/RADARSAT1/SCENE01/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/ )
        ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/PARIS/Ikonos/po_79039_pan_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_GRN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/PARIS/Ikonos/po_79039_grn_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_BLU ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/PARIS/Ikonos/po_79039_blu_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_RED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/PARIS/Ikonos/po_79039_red_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT4 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEDATA}/SPOT4_SCENE01/ )
        ${LARGEDATA}/SPOT4_SCENE01/IMAG_01.DAT )

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT5TIF ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${INPUTDATA}/poupeesTIF/ )
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_HFA ${IO_TESTS5} otbGDALImageIOTestCanRead
	${INPUTDATA}/HFAGeoreferenced.img)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# WRITER

ADD_TEST(ioTuGDALImageIOCanWrite_TIFF ${IO_TESTS6} otbGDALImageIOTestCanWrite
	${INPUTDATA}/testGDALWriter.tif)
ADD_TEST(ioTuGDALImageIOCanWrite_HFA ${IO_TESTS6} otbGDALImageIOTestCanWrite
	${INPUTDATA}/HFAGeoreferenced.img)


# -------            otb::ONERAImageIO   ------------------------------
# CanRead()
IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuONERAImageIOCanRead ${IO_TESTS6} otbONERAImageIOTestCanRead
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent)


ADD_TEST(ioTvONERAImageFileReader ${IO_TESTS6}
  --compare-image ${TOL}  ${BASELINE}/ioImageFileReaderONERA.hdr
                          ${TEMP}/ioImageFileReaderONERA.hdr
        otbImageFileReaderONERATest
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERA.hdr )

ADD_TEST(ioTvONERAImageFileReaderComplex ${IO_TESTS6}
  --compare-n-images ${TOL} 2
			  ${BASELINE}/ioImageFileReaderONERAComplexReal.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
			  ${BASELINE}/ioImageFileReaderONERAComplexImag.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexImag.hdr
        otbImageFileReaderONERAComplex
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
	${TEMP}/ioImageFileReaderONERAComplexImag.hdr )

ADD_TEST(ioTvONERAImageFileWriterComplex ${IO_TESTS6}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
                           ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
        otbImageFileWriterONERAComplex
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

ADD_TEST(ioTvONERAImageStreamWriter ${IO_TESTS6}
  --compare-image ${TOL}   ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
  			   ${TEMP}/ioImageFileReaderONERAStreamWrite.ent
        otbImageFileWriterStreamingONERAComplex
        ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
        ${TEMP}/ioImageFileReaderONERAStreamWrite.ent )

ENDIF(OTB_DATA_USE_LARGEINPUT)

# -------            otb::ONERAImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuMSTARImageIOCanRead1 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB19896.016)

ADD_TEST(ioTuMSTARImageIOCanRead2 ${IO_TESTS6} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB20010.000)

ADD_TEST(ioTvImageFileReaderMSTAR ${IO_TESTS6}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderMSTAR_rescal_chanel1.png
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
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png )
# RGB
ADD_TEST(ioTvImageFileReaderRGB_PNG2PNG ${IO_TESTS7}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_PNG2PNG_sbuv.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2PNG_sbuv.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileReaderRGB_PNG2PNG_sbuv.png )
ADD_TEST(ioTvImageFileReaderRGB_PNG2TIFF ${IO_TESTS7}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff
                    ${TEMP}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG ${IO_TESTS7}
  --compare-image ${TOL}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS7}
  --compare-image ${TOL}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr
        ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/
        ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )

ADD_TEST(ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG ${IO_TESTS7}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

# Suppression CAI
# ADD_TEST(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS7}
#   --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#                     ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#         otbImageFileReaderRGBTest
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTests8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS8}
  --compare-image ${TOL}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )

ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS8}
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr )
ADD_TEST(ioTvImageFileReaderENVI2PNG ${IO_TESTS8}
  --compare-image ${TOL}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
        otbImageFileReaderTest
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png )

# RGB

ADD_TEST(ioTvImageFileWriterRGB_PNG2PNG ${IO_TESTS8}
  --compare-image ${TOL}   ${BASELINE}/ioImageFileWriterRGB_PNG2PNG_sbuv.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2PNG_sbuv.png
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileWriterRGB_PNG2PNG_sbuv.png )

ADD_TEST(ioTvImageFileWriterRGB_PNG2BSQ ${IO_TESTS8}
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hdr )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG_2 ${IO_TESTS8}
  --compare-image ${TOL}   ${INPUTDATA}/sbuv.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png
        otbImageFileReaderRGBTest
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hdr
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png )

# WRITER GDAL
ADD_TEST(ioTvGDALImageFileWriterTIF2TIF ${IO_TESTS8}
	otbImageFileWriterRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/poupees.tiff )


# -------            otb::ImageFileReader pour GDAL  ------------------------------
# -------            otb::ImageFileWriter pour GDAL  ------------------------------


ADD_TEST(ioTvImageReaderWriterRgbPNG2PNG ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png )

ADD_TEST(ioTvImageReaderWriterRgbPNG2TIF ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif )
ADD_TEST(ioTvImageReaderWriterRgbPNG2ENVI ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees.hdr )
ADD_TEST(ioTvImageReaderWriterRgbPNG2JPEG ${IO_TESTS8}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL} ${INPUTDATA}/poupees.png
#                 ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg )

ADD_TEST(ioTvImageReaderWriterRgbTIF2PNG ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/poupees.tif
                     ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.tif
        ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbENVI2PNG ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/poupees
                     ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees
        ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2PNG ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2TIF ${IO_TESTS8}
   --compare-image ${TOL}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif )


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------            otb::MultiChannelExtractROI   ------------------------------


IF(OTB_DATA_USE_LARGEINPUT)
# ---  SPOT4 (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT4_SCENE01/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT4_SCENE01/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  ENVI  ---
ADD_TEST(ioTvMultiChannelROI_ENVI2PNG_2 ${COMMON_TESTS2}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/poupees.hdr
         -uchar ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---


# ADD_TEST(ioTvMultiChannelROI_SPOT5BIL2PNG_2 ${COMMON_TESTS2}
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
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
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
ADD_TEST(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
ADD_TEST(ioTvMultiChannelROI_IkonosRED2PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
                            ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_red_0000000.tif
         ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosGRN2PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_grn_0000000.tif
         ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosBLU2PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_blu_0000000.tif
         ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosNIR2PNG ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  LUM ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2LUM ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  BSQ ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2BSQ ${COMMON_TESTS2}
   --compare-image ${TOL}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                                ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         otbMultiChannelExtractROI
         ${LARGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird PAN  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdPAN2TIFF ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )

# ---  Quickbird XS  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdXS2TIFF ${COMMON_TESTS2}
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)
#         -channels 1 2 3)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTuOtbImageTestSAR_ERS ${IO_TESTS9}
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
ADD_TEST(ioTuOtbImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbImageTestRadarsat.txt
        otbImageTest
       	${LARGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPS}  ${TEMP}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
        otbVectorImageTest
        ${LARGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbVectorImageTestRadarsat.txt)


# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestSpot5 ${IO_TESTS9}
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageTest
        ${LARGEDATA}/SPOT5_SCENE01/
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  otb::VectorImageReadWriteTest  ---
ADD_TEST(ioTvVectorImageFileReaderWriterTest ${IO_TESTS9}
   --compare-image ${TOL}
		   ${INPUTDATA}/qb_RoadExtract.img.hdr
                   ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/qb_RoadExtract.img.hdr
        ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
)


# ---  STREAMING : GDAL  ---
ADD_TEST(ioTvStreamingImageFilterGDAL_ENVI ${IO_TESTS9}
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
   --compare-image ${TOL} ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         )

# ---  STREAMING : GDAL  ---
ADD_TEST(ioTvStreamingImageFilterGDAL_TIFF ${IO_TESTS9}
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
#                      ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
         ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         )


# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS9}
   --compare-image ${TOL}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         0 # Streaming
         )
# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS9}
   --compare-image ${TOL}   ${INPUTDATA}/poupees_1canal.c1.hdr
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

# ---  STREAMING  IMAGE   FILE    WRITER  ---
# ---  TESTS DIFFERENT ACTIVATE STREAMING PARAMETER  ---
# 1. DEFAULT
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2. AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
         )
# 3. NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         NUMBER_OF_STREAM_DIVISIONS
         10
         )
# 4. BUFFER_MEMORY_SIZE
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         BUFFER_MEMORY_SIZE
         8*2000*10
         )
# 5. BUFFER_NUMBER_OF_LINES
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         BUFFER_NUMBER_OF_LINES
         1000
         )
# 6. TILING_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
			    ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS.tif
         TILING_STREAM_DIVISIONS
         )
# 7. TILING_STREAM_DIVISIONS with number of divisions
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
			    ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS.tif
         TILING_NUMBER_OF_STREAM_DIVISIONS
				 100
         )

ADD_TEST(ioTvStreamingIFWriterWithFilter ${IO_TESTS10}
   --compare-image ${TOL}   ${BASELINE}/ioStreamingImageFileWriterWithFilter_1_100.hdr
                            ${TEMP}/ioStreamingImageFileWriterWithFilter_1_100.hdr
         otbStreamingImageFileWriterWithFilterTest
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
   --compare-metadata ${TOL}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

ADD_TEST(ioTvImageMetadataWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${TOL}
		      ${INPUTDATA}/spot5SubWithGcps.tif
		      ${TEMP}/temp.tif
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
	${TEMP}/temp.tif
        )


# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${TOL}
#		      ${INPUTDATA}/HFAGeoreferenced.img
#		      ${TEMP}/temp.tif
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#	${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${TOL}
#		      ${INPUTDATA}/spot5SubWithGcps.tif
#		      ${TEMP}/temp.img
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#	${TEMP}/temp.img
#        )


ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${TOL}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${TOL}
		      ${INPUTDATA}/spot5SubWithGcps.tif
		      ${TEMP}/temp.tif
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
	${TEMP}/temp.tif
        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${TOL}
#		      ${INPUTDATA}/HFAGeoreferenced.img
#		      ${TEMP}/temp.tif
#        otbImageMetadataStreamingFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#	${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${TOL}
#		      ${INPUTDATA}/spot5SubWithGcps.tif
#		      ${TEMP}/temp.img
#        otbImageMetadataStreamingFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#	${TEMP}/temp.img
#        )

ADD_TEST(ioTvPipelineMetadataHandlingTest ${IO_TESTS11}
   --compare-metadata ${TOL}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbPipelineMetadataHandlingTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

# Commented out, since this test fails beceause of an ITK bug (added to our bugtracker for tracking)
#ADD_TEST(ioTvPipelineMetadataHandlingWithUFFilterTest ${IO_TESTS11}
#   --compare-metadata ${TOL}
#		      ${INPUTDATA}/HFAGeoreferenced.img
#		      ${TEMP}/temp.img
#        otbPipelineMetadataHandlingWithUFFilterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#	${TEMP}/temp.img
#        )

# -------            otb::DXFToSpatialObjectGroupFilter -------------------------------------------
ADD_TEST(ioTuDXFToSpatialObjectGroupFilterNew ${IO_TESTS11}
         otbDXFToSpatialObjectGroupFilterNew)

# -------            otb::SpatialObjectDXFReader -------------------------------------------
ADD_TEST(ioTuSpatialObjectDXFReaderNew ${IO_TESTS11}
         otbSpatialObjectDXFReaderNew)

ADD_TEST(ioTvSpatialObjectDXFReader ${IO_TESTS11}
--compare-image ${TOL}
	 ${BASELINE}/ioTvSpatialObjectDXFReaderOutput.png
	 ${TEMP}/ioTvSpatialObjectDXFReaderOutput.png
         otbSpatialObjectDXFReader
	 ${INPUTDATA}/Lardenne.dxf
	 ${TEMP}/ioTvSpatialObjectDXFReaderOutput.png
	 TOIT
	 50
)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS12 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------     Test pour la lecture des m�tadonn�es OSSIM  ------------------------

# ---  Quickbird Metadata ---
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvOSSIMImageQuickbirdMetaDataReader ${IO_TESTS12}
   --compare-n-ascii ${EPS} 2           ${BASELINE_FILES}/ioOSSIMImageQuickbirdMetaDataReader.txt
                                        ${TEMP}/ioOSSIMImageQuickbirdMetaDataReader.txt
                                        ${BASELINE_FILES}/ioOSSIMImageQuickbirdMetaDataReader.txt
                                        ${TEMP}/ioOSSIMVectorImageQuickbirdMetaDataReader.txt
         otbOSSIMImageMetaDataReaderTest
         ${LARGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
	 ${TEMP}/ioOSSIMImageQuickbirdMetaDataReader.txt
	 ${TEMP}/ioOSSIMVectorImageQuickbirdMetaDataReader.txt
         )

# ---  Radarsat Metadata ---
ADD_TEST(ioTvOSSIMImageRadarsatMetaDataReader ${IO_TESTS12}
   --compare-n-ascii ${EPS} 2           ${BASELINE_FILES}/ioOSSIMImageRadarsatMetaDataReader.txt
                                        ${TEMP}/ioOSSIMImageRadarsatMetaDataReader.txt
                                        ${BASELINE_FILES}/ioOSSIMImageRadarsatMetaDataReader.txt
                                        ${TEMP}/ioOSSIMVectorImageRadarsatMetaDataReader.txt
         otbOSSIMImageMetaDataReaderTest
         ${LARGEDATA}/RADARSAT2/SCENE01/DAT_01.001
	 ${TEMP}/ioOSSIMImageRadarsatMetaDataReader.txt
	 ${TEMP}/ioOSSIMVectorImageRadarsatMetaDataReader.txt
         )

# ---  ERS2 Metadata ---
ADD_TEST(ioTvOSSIMImageERS2MetaDataReader ${IO_TESTS12}
   --compare-n-ascii ${EPS} 2           ${BASELINE_FILES}/ioOSSIMImageERS2MetaDataReader.txt
                                        ${TEMP}/ioOSSIMImageERS2MetaDataReader.txt
                                        ${BASELINE_FILES}/ioOSSIMImageERS2MetaDataReader.txt
                                        ${TEMP}/ioOSSIMVectorImageERS2MetaDataReader.txt
         otbOSSIMImageMetaDataReaderTest
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
	 ${TEMP}/ioOSSIMImageERS2MetaDataReader.txt
	 ${TEMP}/ioOSSIMVectorImageERS2MetaDataReader.txt
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  otb::DEMHandler ---
ADD_TEST(ioTuDEMHandlerNew ${IO_TESTS12} otbDEMHandlerNew )

ADD_TEST(ioTvDEMHandler ${IO_TESTS12}
--compare-ascii ${TOL} 	${BASELINE_FILES}/ioDEMGetHeightAboveMSL.txt
                        ${TEMP}/ioDEMGetHeightAboveMSL.txt
        otbDEMHandlerTest
        ${INPUTDATA}/DEM/srtm_directory
        ${TEMP}/ioDEMGetHeightAboveMSL.txt
	3.6999 44.08
        )

# ---  otb::ImageGeometryHandler ---
ADD_TEST(ioTuImageGeometryHandlerNew ${IO_TESTS12} otbImageGeometryHandlerNew )

# ---  otb::DEMToImageGenerator ---
ADD_TEST(ioTuDEMToImageGeneratorNew ${IO_TESTS12} otbDEMToImageGeneratorNew )

ADD_TEST(ioTvDEMToImageGeneratorTest ${IO_TESTS12}
--compare-image ${TOL}  ${BASELINE}/ioTvDEMToImageGenerator.tif
                        ${TEMP}/ioTvDEMToImageGenerator.tif
        otbDEMToImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToImageGenerator.tif
        6.5
        44.5
        500
        500
        0.002
        0.002
        )


ADD_TEST(ioTvossimElevManagerTest ${IO_TESTS12}
--compare-image ${TOL}  ${BASELINE}/ioTvossimElevManagerTest.tif
                        ${TEMP}/ioTvossimElevManagerTest.tif
        otbOssimElevManagerTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvossimElevManagerTest.tif
				 6.5
         44.5
				 0.002
				 0.002
				  500
				  500
         )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#----------------------------------------------------------------------------------
# Exemples de tests sur les grosses images
# Tests non lanc�s pour raison taille des fichiers produits
#        ADD_TEST(ioTvStreamingIFWriterPARIS_SPOT5THR_WithoutStreaming ${IO_TESTS12}
#                 otbStreamingImageFileWriterTest
#                 ${LARGEDATA}/PARIS/SPOT5/THR
#                 ${TEMP}/ioStreamingImageFileWriterTestCAI_ParisSpot5THR_WithoutStreaming.hd
#                 0 # Test sans streaming
#                 )
#        ADD_TEST(ioTvStreamingIFWriterPARIS_SPOT5THR_WithStreaming_1000 ${IO_TESTS12}
#                 otbStreamingImageFileWriterTest
#                 ${LARGEDATA}/PARIS/SPOT5/THR
#                 ${TEMP}/ioStreamingImageFileWriterTestCAI_ParisSpot5THR_WithStreaming_1000.hd
#                 1 # Test sans streaming
#                 1000 # NbDivisionsStreaming
#                 )
#----------------------------------------------------------------------------------


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS13 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_COMPILE_JPEG2000)

# ---  JPEG2000 can read ---
ADD_TEST(ioTuJ2KImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${INPUTDATA}/bretagne.j2k
         )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTuJP2ImageIOCanRead ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanRead
         ${LARGEDATA}/JPEG2000/QB_multicomp.jpx
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvVectorImageFileReaderWriterJ2K2TIF ${IO_TESTS9}
--compare-image ${TOL}  ${BASELINE}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
                        ${TEMP}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${INPUTDATA}/bretagne.j2k
	 ${TEMP}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
         )

ADD_TEST(ioTvImageFileReaderWriterJ2K2TIF ${IO_TESTS2}
--compare-image ${TOL}  ${BASELINE}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
         otbDoubleImageIOTest
         ${INPUTDATA}/bretagne.j2k
	 ${TEMP}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
        )

#NOTE: we are comparing with the kakadu output in raw format.
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorImageFileReaderWriterJP22TIF ${IO_TESTS9}
--compare-image ${TOL}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput
                        ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${LARGEDATA}/JPEG2000/QB_multicomp_small.jpx
	 ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         )


ADD_TEST(ioTvImageFileReaderWriterJP22TIF ${IO_TESTS2}
--compare-image ${TOL}  ${BASELINE}/ioTvImageFileReaderWriterJP22TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         otbDoubleImageIOTest
         ${LARGEDATA}/JPEG2000/QB_multicomp_small.jpx
	 ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)



# ---  JPEG2000 can write ---
ADD_TEST(ioTuJPEG2000ImageIOCanWrite ${IO_TESTS13}
         otbJPEG2000ImageIOTestCanWrite
         ${TEMP}/toto.j2k
         )

ENDIF(OTB_COMPILE_JPEG2000)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS14 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTuOGRDriverTest ${IO_TESTS14}
				otbOGRDriverTest
				${LARGEDATA}/TOULOUSE/QuickBird/GIS_FILES/000000128955_01_PRODUCT_SHAPE.shp
				foo
				)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuOGRDriver2Test ${IO_TESTS14}
				otbOGRDriverTest
				${INPUTDATA}/france_coastline.shp
				foo
				)

ADD_TEST(ioTvGDALDriverDoubleWritingTest ${IO_TESTS14}
		--compare-n-images ${TOL} 4
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
		${LARGEDATA}/RADARSAT2/SCENE01/DAT_01.001
		1000 1000
)

ADD_TEST(ioTvWritingComplexDataWithComplexImage ${IO_TESTS14}
--compare-image ${TOL}
		${BASELINE}/ioWritingComplexDataWithComplexImageOutput.tif
		${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
		otbWritingComplexDataWithComplexImageTest
		${LARGEDATA}/RADARSAT2/SCENE01/DAT_01.001
		${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
		1000 1000
		500  500
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS15 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(ioTuOGRVectorDataIO ${IO_TESTS15}
        otbSHPVectorDataIONew )

ADD_TEST(ioTuVectorDataSourceNew ${IO_TESTS15}
        otbVectorDataSourceNew )

ADD_TEST(ioTuVectorDataFileReaderNew ${IO_TESTS15}
        otbVectorDataFileReaderNew )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvSHPVectorDataFileReader ${IO_TESTS15}
        --compare-ascii ${TOL}  ${BASELINE_FILES}/ioSHPVectorDataFileReader.txt
                        ${TEMP}/ioSHPVectorDataFileReader.txt
        otbVectorDataFileReader
	${LARGEDATA}/TOULOUSE/QuickBird/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
	${TEMP}/ioSHPVectorDataFileReader.txt)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTuVectorDataFileWriterNew ${IO_TESTS15}
        otbVectorDataFileWriterNew )

# If the file exists, try remove it to prevent test failure
FILE(REMOVE ${TEMP}/ioTvVectorDataFileWriterOutput.shp)

ADD_TEST(ioTvVectorDataFileWriter ${IO_TESTS15}
        otbVectorDataFileWriter
	${TEMP}/ioTvVectorDataFileWriterOutput.shp
	)

# If the file exists, try remove it to prevent test failure
FILE(REMOVE ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorDataFileReaderWriter ${IO_TESTS15}
        otbVectorDataFileReaderWriter
	${LARGEDATA}/TOULOUSE/QuickBird/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
	${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
	)
ENDIF(OTB_DATA_USE_LARGEINPUT)

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



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS16 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# ---  RAD  ---

# Can read a RAD format (ci2)
ADD_TEST(ioTuRADImageIOCanRead_RADCI2 ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCI2_image_simu_poupees.rad )

# Can read a RAD format (cr4)
ADD_TEST(ioTuRADImageIOCanRead_RADCR4sngl ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_image.rad )

# Can read a multi-image RAD format (cr4)
ADD_TEST(ioTuRADImageIOCanRead_RADCR4mult ${IO_TESTS16}
        otbRADImageIOTestCanRead
        ${INPUTDATA}/RADCR4_multimage.rad )



# Convert a ci2.RAD file into ENVI format
#ADD_TEST(ioTvImageFileReaderRADCI22ENVI ${IO_TESTS16}
#        otbImageFileReaderRADComplexFloat
#        /home3/mathieu/guillaume/CD_CNES/RAD_CI2/3017_HH.rad
#        ${TEMP}/ioImageFileReaderRAD2ENVI.hdr )

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


ADD_TEST(ioTuPointSetFileReaderNew ${IO_TESTS16}
        otbPointSetFileReaderNew )

ADD_TEST(ioTvPointSetFileReader ${IO_TESTS16}
        --compare-ascii ${TOL}  ${BASELINE_FILES}/ioPointSetFileReader.txt
                        ${TEMP}/ioPointSetFileReader.txt
        otbPointSetFileReader
  ${INPUTDATA}/srs.las
  ${TEMP}/ioPointSetFileReader.txt)


#----------------------------------------------------------------------------------
SET(BasicIO_SRCS1
otbImageFileReaderERS.cxx
otbLUMImageIOTestCanRead.cxx
otbLUMImageIOTestCanWrite.cxx
otbMWImageIOTestCanRead.cxx
otbMWImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderTestFloat.cxx
otbShortImageIOTest.cxx
)
SET(BasicIO_SRCS2
otbIntImageIOTest.cxx
otbFloatImageIOTest.cxx
otbDoubleImageIOTest.cxx
otbStreamingImageFilterTest.cxx
otbStreamingImageFileWriterTest.cxx
otbPipeline.cxx
)
SET(BasicIO_SRCS3
otbBSQImageIOTestCanRead.cxx
otbBSQImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
otbIntImageIOTest.cxx
otbShortRGBImageIOTest.cxx
)
#SET(BasicIO_SRCS4
#)
#CONTAINS THE BasicIO_SRCS2 include
SET(BasicIO_SRCS5
otbGDALImageIOTestCanRead.cxx
)
SET(BasicIO_SRCS6
otbGDALImageIOTestCanWrite.cxx
otbONERAImageIOTestCanRead.cxx
otbImageFileReaderONERA.cxx
otbImageFileReaderONERAComplex.cxx
otbImageFileWriterONERAComplex.cxx
otbImageFileWriterStreamingONERAComplex.cxx
otbMSTARImageIOTestCanRead.cxx
otbImageFileReaderMSTAR.cxx
)
SET(BasicIO_SRCS7
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
)
SET(BasicIO_SRCS8
otbImageFileWriterTest.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
)
SET(BasicIO_SRCS9
otbImageTest.cxx
otbVectorImageTest.cxx
otbStreamingImageFileWriterTest.cxx
otbStreamingImageFilterTest.cxx
otbVectorImageFileReaderWriterTest.cxx
otbStreamingShortImageFileWriterTest.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
)
SET(BasicIO_SRCS10
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
)
SET(BasicIO_SRCS11
otbImageMetadataFileWriterTest.cxx
otbImageMetadataStreamingFileWriterTest.cxx
otbPipelineMetadataHandlingTest.cxx
otbPipelineMetadataHandlingWithUFFilterTest.cxx
otbDXFToSpatialObjectGroupFilterNew.cxx
otbSpatialObjectDXFReaderNew.cxx
otbSpatialObjectDXFReader.cxx
)
SET(BasicIO_SRCS12
#otbMultiToMonoChannelExtractROISAR.cxx
otbOSSIMImageMetaDataReaderTest.cxx
otbDEMHandlerNew.cxx
otbDEMHandlerTest.cxx
otbDEMToImageGeneratorNew.cxx
otbDEMToImageGeneratorTest.cxx
otbImageGeometryHandlerNew.cxx
otbOssimElevManagerTest.cxx
)


IF(OTB_COMPILE_JPEG2000)
SET(BasicIO_SRCS13
otbJPEG2000ImageIOTestCanRead.cxx
otbJPEG2000ImageIOTestCanWrite.cxx
)
ENDIF(OTB_COMPILE_JPEG2000)


SET(BasicIO_SRCS14
otbOGRDriverTest.cxx
otbGDALDriverDoubleWritingTest.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbWritingComplexDataWithComplexImageTest.cxx
)



SET(BasicIO_SRCS15
otbVectorDataSourceNew.cxx
otbVectorDataFileReaderNew.cxx
otbVectorDataFileReader.cxx
otbVectorDataFileWriterNew.cxx
otbVectorDataFileWriter.cxx
otbVectorDataFileReaderWriter.cxx
otbSHPVectorDataIONew.cxx
otbKMLVectorDataIONew.cxx
otbKMLVectorDataIOTestCanRead.cxx
otbKMLVectorDataIOTestCanWrite.cxx
otbKMLVectorDataIOTestFileReader.cxx
)

SET(BasicIO_SRCS16
otbRADImageIOTestCanRead.cxx
otbImageFileReaderRADChar.cxx
otbImageFileReaderRADInt.cxx
otbImageFileReaderRADFloat.cxx
otbImageFileReaderRADComplexInt.cxx
otbImageFileReaderRADComplexFloat.cxx
otbImageFileReaderRADComplexDouble.cxx
otbPointSetFileReaderNew.cxx
otbPointSetFileReader.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbIOTests1 otbIOTests1.cxx ${BasicIO_SRCS1})
TARGET_LINK_LIBRARIES(otbIOTests1 OTBIO)

ADD_EXECUTABLE(otbIOTests2 otbIOTests2.cxx ${BasicIO_SRCS2})
TARGET_LINK_LIBRARIES(otbIOTests2 OTBIO  )

ADD_EXECUTABLE(otbIOTests3 otbIOTests3.cxx ${BasicIO_SRCS3})
TARGET_LINK_LIBRARIES(otbIOTests3 OTBIO  )

ADD_EXECUTABLE(otbIOTests4 otbIOTests4.cxx ${BasicIO_SRCS2})
TARGET_LINK_LIBRARIES(otbIOTests4 OTBIO  )

ADD_EXECUTABLE(otbIOTests5 otbIOTests5.cxx ${BasicIO_SRCS5})
TARGET_LINK_LIBRARIES(otbIOTests5 OTBIO  )

ADD_EXECUTABLE(otbIOTests6 otbIOTests6.cxx ${BasicIO_SRCS6})
TARGET_LINK_LIBRARIES(otbIOTests6 OTBIO  )

ADD_EXECUTABLE(otbIOTests7 otbIOTests7.cxx ${BasicIO_SRCS7})
TARGET_LINK_LIBRARIES(otbIOTests7 OTBIO  )

ADD_EXECUTABLE(otbIOTests8 otbIOTests8.cxx ${BasicIO_SRCS8})
TARGET_LINK_LIBRARIES(otbIOTests8 OTBIO  )

ADD_EXECUTABLE(otbIOTests9 otbIOTests9.cxx ${BasicIO_SRCS9})
TARGET_LINK_LIBRARIES(otbIOTests9 OTBIO  )

ADD_EXECUTABLE(otbIOTests10 otbIOTests10.cxx ${BasicIO_SRCS10})
TARGET_LINK_LIBRARIES(otbIOTests10 OTBIO  )

ADD_EXECUTABLE(otbIOTests11 otbIOTests11.cxx ${BasicIO_SRCS11})
TARGET_LINK_LIBRARIES(otbIOTests11 OTBIO  )

ADD_EXECUTABLE(otbIOTests12 otbIOTests12.cxx ${BasicIO_SRCS12})
TARGET_LINK_LIBRARIES(otbIOTests12 OTBIO  )

# Case for releae 2.2.1
IF(OTB_COMPILE_JPEG2000)
   IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DOPJ_STATIC)
   ENDIF(NOT BUILD_SHARED_LIBS)

   ADD_EXECUTABLE(otbIOTests13 otbIOTests13.cxx ${BasicIO_SRCS13})
   TARGET_LINK_LIBRARIES(otbIOTests13 OTBIO  )
ENDIF(OTB_COMPILE_JPEG2000)

ADD_EXECUTABLE(otbIOTests14 otbIOTests14.cxx ${BasicIO_SRCS14})
TARGET_LINK_LIBRARIES(otbIOTests14 OTBIO  )

ADD_EXECUTABLE(otbIOTests15 otbIOTests15.cxx ${BasicIO_SRCS15})
TARGET_LINK_LIBRARIES(otbIOTests15 OTBIO  )

ADD_EXECUTABLE(otbIOTests16 otbIOTests16.cxx ${BasicIO_SRCS16})
TARGET_LINK_LIBRARIES(otbIOTests16 OTBIO  )

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

