
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(EXAMPLEDATA ${OTB_DATA_ROOT}/Examples)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance on  pixel image diff
SET(NOTOL 0.0)
SET(EPSILON_9 0.000000001)

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

IF(OTB_USE_LIBLAS)
  SET(IO_TESTS17    ${CXX_TEST_PATH}/otbIOTests17)
ENDIF(OTB_USE_LIBLAS)

SET(IO_TESTS18    ${CXX_TEST_PATH}/otbIOTests18)

IF(OTB_USE_CURL)
  SET(IO_TESTS19    ${CXX_TEST_PATH}/otbIOTests19)
ENDIF(OTB_USE_CURL)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ COMMON_TESTS3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

IF(OTB_DATA_USE_LARGEINPUT)

# ---  RADARSAT (GDAL)  ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT2ENVI ${COMMON_TESTS3}
         otbMultiToMonoChannelExtractROI
         -ushort ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  Nom r�pertoire ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI ${COMMON_TESTS3}
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${EPSILON_9} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
#                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         -startX 0 -startY 0 -sizeX 337 -sizeY 280 )
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
ADD_TEST(ioTvImageFileReaderERS ${IO_TESTS1}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderERS_SLCI.tif
                            ${TEMP}/ioImageFileReaderERS_SLCI.tif
         otbImageFileReaderERS
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioImageFileReaderERS_SLCI.tif)

ADD_TEST(ioTvImageFileReaderRADARSAT ${IO_TESTS1}
   --compare-image ${EPSILON_9} ${BASELINE}/ioImageFileReaderRADARSAT.tif
                           ${TEMP}/ioImageFileReaderRADARSAT.tif
         otbImageFileReaderERS
         ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/
         ${TEMP}/ioImageFileReaderRADARSAT.tif)

ADD_TEST(ioTvImageFileReaderPALSAR ${IO_TESTS1}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderPALSAR_l1.1.tif
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
ADD_TEST(ioTuMWImageIOCanWrite ${IO_TESTS1} otbMWImageIOTestCanWrite
        ${TEMP}/QB_Toulouse_Ortho_PAN.mw)

ADD_TEST(ioTvImageFileReaderTIF2MW ${IO_TESTS1}
  --compare-image ${EPSILON_9}        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                ${TEMP}/ioImageFileReaderTIF2MW.mw
        otbImageFileReaderTestFloat
        ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
        ${TEMP}/ioImageFileReaderTIF2MW.mw )


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
#        ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/ )
        ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/ )
        ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/IKONOS/PARIS/po_79039_pan_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_GRN ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/IKONOS/PARIS/po_79039_grn_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_BLU ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/IKONOS/PARIS/po_79039_blu_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_RED ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/IKONOS/PARIS/po_79039_red_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS5} otbGDALImageIOTestCanRead
        ${LARGEDATA}/IKONOS/PARIS/po_79039_nir_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT4 ${IO_TESTS5} otbGDALImageIOTestCanRead
# Filename identification is now set in the ImageFileReader class (Fiche 212)
#        ${LARGEDATA}/SPOT4/RIO_DE_JANEIRO/ )
        ${LARGEDATA}/SPOT4/RIO_DE_JANEIRO/IMAG_01.DAT )

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
  --compare-image ${EPSILON_9}  ${BASELINE}/ioImageFileReaderONERA.hdr
                          ${TEMP}/ioImageFileReaderONERA.hdr
        otbImageFileReaderONERATest
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERA.hdr )

ADD_TEST(ioTvONERAImageFileReaderComplex ${IO_TESTS6}
  --compare-n-images ${EPSILON_9} 2
			  ${BASELINE}/ioImageFileReaderONERAComplexReal.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
			  ${BASELINE}/ioImageFileReaderONERAComplexImag.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexImag.hdr
        otbImageFileReaderONERAComplex
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
	${TEMP}/ioImageFileReaderONERAComplexImag.hdr )

ADD_TEST(ioTvONERAImageFileWriterComplex ${IO_TESTS6}
  --compare-image ${EPSILON_9}   ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
                           ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
        otbImageFileWriterONERAComplex
        ${LARGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

ADD_TEST(ioTvONERAImageStreamWriter ${IO_TESTS6}
  --compare-image ${EPSILON_9}   ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
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
         ${LARGEDATA}/SPOT4/RIO_DE_JANEIRO/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT4/RIO_DE_JANEIRO/
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
         ${LARGEDATA}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/SPOT5/TEHERAN/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
ADD_TEST(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
ADD_TEST(ioTvMultiChannelROI_IkonosRED2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
                            ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_red_0000000.tif
         ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosGRN2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_grn_0000000.tif
         ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosBLU2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_blu_0000000.tif
         ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosNIR2PNG ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  LUM ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2LUM ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  BSQ ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2BSQ ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                                ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         otbMultiChannelExtractROI
         ${LARGEDATA}/IKONOS/PARIS/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird PAN  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdPAN2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )

# ---  Quickbird XS  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdXS2TIFF ${COMMON_TESTS2}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
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
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
ADD_TEST(ioTvOtbImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbImageTestRadarsat.txt
        otbImageTest
       	${LARGEDATA}/RADARSAT1/GOMA/SCENE01/
        ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTvOtbVectorImageTestRadarsat ${IO_TESTS9}
   --compare-ascii ${EPS}  ${TEMP}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
        otbVectorImageTest
        ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/
        ${TEMP}/ioOtbVectorImageTestRadarsat.txt)


# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTvOtbVectorImageTestSpot5 ${IO_TESTS9}
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageTest
        ${LARGEDATA}/SPOT5/TEHERAN/
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ---  otb::VectorImageReadWriteTest  ---
ADD_TEST(ioTvVectorImageFileReaderWriterTest ${IO_TESTS9}
   --compare-image ${EPSILON_9}
		   ${INPUTDATA}/qb_RoadExtract.img.hdr
                   ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
        otbVectorImageFileReaderWriterTest
        ${INPUTDATA}/qb_RoadExtract.img.hdr
        ${TEMP}/ioTvVectorImageFileReaderWriterTest.hdr
)

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

# ---  STREAMING  IMAGE   FILE    WRITER  ---
# ---  TESTS DIFFERENT ACTIVATE STREAMING PARAMETER  ---
# 1. DEFAULT
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 1.bis WITH IMAGEFILEWRITER : DEFAULT
ADD_TEST(ioTvStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbStreamingWithImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingWithImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2. AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
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
# 3. NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         NUMBER_OF_STREAM_DIVISIONS
         10
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



# 4. BUFFER_MEMORY_SIZE
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         BUFFER_MEMORY_SIZE
         8*2000*10
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
# 5. BUFFER_NUMBER_OF_LINES
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                          ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         BUFFER_NUMBER_OF_LINES
         1000
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
# 6. TILING_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
			    ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_STREAM_DIVISIONS.tif
         TILING_STREAM_DIVISIONS
         )
# 7. TILING_STREAM_DIVISIONS with number of divisions
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
			    ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_TILING_NUMBER_OF_STREAM_DIVISIONS.tif
         TILING_NUMBER_OF_STREAM_DIVISIONS
				 100
         )

ADD_TEST(ioTvStreamingIFWriterWithFilter ${IO_TESTS10}
   --compare-image ${EPSILON_9}   ${BASELINE}/ioStreamingImageFileWriterWithFilter_1_100.hdr
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
   --compare-metadata ${EPSILON_9}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

ADD_TEST(ioTvImageMetadataWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
		      ${INPUTDATA}/spot5SubWithGcps.tif
		      ${TEMP}/temp.tif
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
	${TEMP}/temp.tif
        )


# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#		      ${INPUTDATA}/HFAGeoreferenced.img
#		      ${TEMP}/temp.tif
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#	${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#		      ${INPUTDATA}/spot5SubWithGcps.tif
#		      ${TEMP}/temp.img
#        otbImageMetadataFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#	${TEMP}/temp.img
#        )


ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIF ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
		      ${INPUTDATA}/spot5SubWithGcps.tif
		      ${TEMP}/temp.tif
        otbImageMetadataStreamingFileWriterTest
        ${INPUTDATA}/spot5SubWithGcps.tif
	${TEMP}/temp.tif
        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_HFA2TIFF ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#		      ${INPUTDATA}/HFAGeoreferenced.img
#		      ${TEMP}/temp.tif
#        otbImageMetadataStreamingFileWriterTest
#        ${INPUTDATA}/HFAGeoreferenced.img
#	${TEMP}/temp.tif
#        )

# Commented because gdal does not seems to write the metadata the same way in both formats
#ADD_TEST(ioTvImageMetadataStreamingWriterTest_TIFF2HFA ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
#		      ${INPUTDATA}/spot5SubWithGcps.tif
#		      ${TEMP}/temp.img
#        otbImageMetadataStreamingFileWriterTest
#        ${INPUTDATA}/spot5SubWithGcps.tif
#	${TEMP}/temp.img
#        )

ADD_TEST(ioTvPipelineMetadataHandlingTest ${IO_TESTS11}
   --compare-metadata ${EPSILON_9}
		      ${INPUTDATA}/HFAGeoreferenced.img
		      ${TEMP}/temp.img
        otbPipelineMetadataHandlingTest
        ${INPUTDATA}/HFAGeoreferenced.img
	${TEMP}/temp.img
        )

# Commented out, since this test fails beceause of an ITK bug (added to our bugtracker for tracking)
#ADD_TEST(ioTvPipelineMetadataHandlingWithUFFilterTest ${IO_TESTS11}
#   --compare-metadata ${EPSILON_9}
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
--compare-image ${EPSILON_9}
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

# ---  otb::DEMHandler ---
ADD_TEST(ioTuDEMHandlerNew ${IO_TESTS12} otbDEMHandlerNew )

ADD_TEST(ioTvDEMHandler ${IO_TESTS12}
--compare-ascii ${EPSILON_9} 	${BASELINE_FILES}/ioDEMGetHeightAboveMSL.txt
                        ${TEMP}/ioDEMGetHeightAboveMSL.txt
        otbDEMHandlerTest
        ${INPUTDATA}/DEM/srtm_directory
        ${INPUTDATA}/DEM/egm96.grd
        ${TEMP}/ioDEMGetHeightAboveMSL.txt
        3.6999 44.08
        )

# ---  otb::DEMToImageGenerator ---
ADD_TEST(ioTuDEMToImageGeneratorNew ${IO_TESTS12} otbDEMToImageGeneratorNew )

ADD_TEST(ioTvDEMToImageGeneratorTest ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvDEMToImageGenerator.tif
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

#Big extend
ADD_TEST(ioTvDEMToImageGeneratorTest2 ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvDEMToImageGenerator2.tif
                        ${TEMP}/ioTvDEMToImageGenerator2.tif
        otbDEMToImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToImageGenerator2.tif
         -1.8 52 232 422  0.02 -0.018
#          576378.080 5763132.837 232 422 2000 -2000 30 N
# 3.765220920574784 44.224685913433
)


ADD_TEST(ioTvossimElevManagerTest ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvossimElevManagerTest.tif
                        ${TEMP}/ioTvossimElevManagerTest.tif
        otbOssimElevManagerTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvossimElevManagerTest.tif
        6.5
        45.5
        0.002
        -0.002
        500
        500
        )

ADD_TEST(ioTvossimElevManagerTest3 ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvossimElevManagerTest3.tif
                        ${TEMP}/ioTvossimElevManagerTest3.tif
        otbOssimElevManagerTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvossimElevManagerTest3.tif
         -1.8 52   0.02 -0.018 232 422
#         -1.8870830 52.0135758  0.024363378967 -0.018457085039 232 422
        )

#This test investigate the problem with the ossimElevManager
# - differences between plaforms
# - file descriptor leak
ADD_TEST(ioTvossimElevManagerTest4 ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvossimElevManagerTest3.tif
                        ${TEMP}/ioTvossimElevManagerTest4.hdr
        otbOssimElevManagerTest4
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvossimElevManagerTest4
         -1.8 52   0.02 -0.018 232 422
        )


ADD_TEST(ioTvossimElevManagerTest2 ${IO_TESTS12}
--compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvossimElevManagerTest2.txt
                        ${TEMP}/ioTvossimElevManagerTest2.txt
        otbOssimElevManagerTest2
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvossimElevManagerTest2.txt
	6.5
	44.5
	0.002
	500
         )


# ---  otb::DEMToOrthoImageGenerator ---
ADD_TEST(ioTuDEMToOrthoImageGeneratorNew ${IO_TESTS12}
      otbDEMToOrthoImageGeneratorNew )

ADD_TEST(ioTvDEMToOrthoImageGeneratorTest ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvDEMToOrthoImageGenerator.tif
                        ${TEMP}/ioTvDEMToOrthoImageGenerator.tif
        otbDEMToOrthoImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToOrthoImageGenerator.tif
         778245.3501 #6.5
         4933369.633 #44.5
         500
         500
         30 #resolution in m
         -30 #resolution in m
	 31 # utm zone
         N # hemisphere
)

#Big extend
ADD_TEST(ioTvDEMToOrthoImageGeneratorTest2 ${IO_TESTS12}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvDEMToOrthoImageGenerator2.tif
                        ${TEMP}/ioTvDEMToOrthoImageGenerator2.tif
        otbDEMToOrthoImageGeneratorTest
         ${INPUTDATA}/DEM/srtm_directory
         ${TEMP}/ioTvDEMToOrthoImageGenerator2.tif
         576378.080 5763132.837 232 422 2000 -2000 30 N
)

# ---  otb::PrepareSRTMDirectory ---
ADD_TEST(ioTuPrepareSRTMDirectoryNew ${IO_TESTS12}
      otbPrepareSRTMDirectoryNew )

ADD_TEST(ioTvPrepareSRTMDirectoryTest ${IO_TESTS12}
  --compare-n-ascii ${EPSILON_9} 3
		    ${INPUTDATA}/DEM/srtm_directory/N44E007.hgt
		    ${TEMP}/ioTvPrepareSRTMDirectoryTest/N44E007.hgt
		    ${INPUTDATA}/DEM/srtm_directory/N46E007.hgt
		    ${TEMP}/ioTvPrepareSRTMDirectoryTest/N46E007.hgt
		    ${INPUTDATA}/DEM/srtm_directory/N45E007.hgt
		    ${TEMP}/ioTvPrepareSRTMDirectoryTest/N45E007.hgt
  otbPrepareSRTMDirectoryTest
      ${INPUTDATA}/DEM/srtm_directory
      ${TEMP}/ioTvPrepareSRTMDirectoryTest
      7.2
      45.2
      6.1
      44.6
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
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
                        ${TEMP}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${INPUTDATA}/bretagne.j2k
	 ${TEMP}/ioTvVectorImageFileReaderWriterJ2K2TIFOutput.tif
         )

ADD_TEST(ioTvImageFileReaderWriterJ2K2TIF ${IO_TESTS2}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
         otbDoubleImageIOTest
         ${INPUTDATA}/bretagne.j2k
	 ${TEMP}/ioTvImageFileReaderWriterJ2K2TIFOutput.tif
        )

#NOTE: we are comparing with the kakadu output in raw format.
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorImageFileReaderWriterJP22TIF ${IO_TESTS9}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvVectorImageFileReaderWriterJP22TIFOutput
                        ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         otbVectorImageFileReaderWriterTest
         ${LARGEDATA}/JPEG2000/QB_multicomp_small.jpx
	 ${TEMP}/ioTvVectorImageFileReaderWriterJP22TIFOutput.tif
         )


ADD_TEST(ioTvImageFileReaderWriterJP22TIF ${IO_TESTS2}
--compare-image ${EPSILON_9}  ${BASELINE}/ioTvImageFileReaderWriterJP22TIFOutput.tif
                        ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         otbDoubleImageIOTest
         ${LARGEDATA}/JPEG2000/QB_multicomp_small.jpx
	 ${TEMP}/ioTvImageFileReaderWriterJP22TIFOutput.tif
         )
ENDIF(OTB_DATA_USE_LARGEINPUT)



# ---  JPEG2000 can write ---
# FIXME Desactivated until http://bugs.orfeo-toolbox.org/view.php?id=104
# is solved
# ADD_TEST(ioTuJPEG2000ImageIOCanWrite ${IO_TESTS13}
#          otbJPEG2000ImageIOTestCanWrite
#          ${TEMP}/toto.j2k
#          )

ENDIF(OTB_COMPILE_JPEG2000)

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
		${LARGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
		1000 1000
)

ADD_TEST(ioTvWritingComplexDataWithComplexImage ${IO_TESTS14}
--compare-image ${EPSILON_9}
		${BASELINE}/ioWritingComplexDataWithComplexImageOutput.tif
		${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
		otbWritingComplexDataWithComplexImageTest
		${LARGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
		${TEMP}/ioWritingComplexDataWithComplexImageOutput.tif
		1000 1000
		500  500
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS15 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
	${LARGEDATA}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
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
ADD_TEST(ioTlSHPVectorDataFileReader3 ${IO_TESTS15}
        --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader3.txt
                        ${TEMP}/ioSHPVectorDataFileReader3.txt
        otbVectorDataFileReader
	${LARGEDATA}/VECTOR/MidiPyrenees/roads.shp
	${TEMP}/ioSHPVectorDataFileReader3.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)


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


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvVectorDataFileReaderWriter ${IO_TESTS15}
        --compare-ogr  ${EPSILON_9}
            ${BASELINE_FILES}/ioTvVectorDataFileReaderWriterOutput.shp
            ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
        otbVectorDataFileReaderWriter
            ${LARGEDATA}/QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp
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

# CanRead OGR GML()
ADD_TEST(ioTuOGRVectorDataIOTestCanRead ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/adressPoint.gml)

# CanRead OGR MapInfo()
ADD_TEST(ioTuOGRVectorDataIOTestCanRead ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

# CanRead OGR PostGIS()
IF(OTB_USE_PQXX)
ADD_TEST(ioTuOGRVectorDataIOTestCanRead ${IO_TESTS15}
        otbOGRVectorDataIOTestCanRead
        PG:"dbname='orfeotoolbox_test' host='localhost' port='5432' user='orfeotoolbox_test_user' password='Bidfeud0'")
ENDIF(OTB_USE_PQXX)

# CanWrite OGR GML()
ADD_TEST(ioTuOGRVectorDataIOCanWrite ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/adressPoint.gml)

# CanWrite OGR MapInfo()
ADD_TEST(ioTuOGRVectorDataIOCanWrite ${IO_TESTS15}
        otbOGRVectorDataIOCanWrite
        ${INPUTDATA}/LOCALITY_POLYGON.tab)

# Canwrite OGR PostGIS()
IF(OTB_USE_PQXX)
ADD_TEST(ioTuOGRVectorDataIOCanWrite ${IO_TESTS15}
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
        ${LARGEDATA}/RAD_CI2/3017_HH.rad)

# Convert a ci2.RAD file into ENVI format
ADD_TEST(ioTvImageFileReaderRADCI22ENVI ${IO_TESTS16}
--compare-image ${EPSILON_9}
	${BASELINE}/ioImageFileReaderRADCI22ENVI.hdr
	${TEMP}/ioImageFileReaderRADCI22ENVI.hdr
        otbImageFileReaderRADComplexFloatExtract
        ${LARGEDATA}/RAD_CI2/3017_HH.rad
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

ADD_TEST(ioTuIkonosImageMetadataInterfaceNew ${IO_TESTS18}
        otbIkonosImageMetadataInterfaceNew
)

ADD_TEST(ioTuQuickBirdImageMetadataInterfaceNew ${IO_TESTS18}
        otbQuickBirdImageMetadataInterfaceNew
)

ADD_TEST(ioTuTerraSarImageMetadataInterfaceNew ${IO_TESTS18}
        otbTerraSarImageMetadataInterfaceNew
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvImageMetadataInterfaceQBTest ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceQB.txt
                         ${TEMP}/ioTvImageMetadataInterfaceQB.txt
        otbImageMetadataInterfaceTest
        ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceQB.txt
)
ADD_TEST(ioTvImageMetadataInterfaceQBCevennesTest ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceQBCevennes.txt
                         ${TEMP}/ioTvImageMetadataInterfaceQBCevennes.txt
        otbImageMetadataInterfaceTest
        ${LARGEDATA}/QUICKBIRD/CEVENNES/06FEB12104912-P1BS-005533998070_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceQBCevennes.txt
)
ADD_TEST(ioTvImageMetadataInterfaceQBMULTest ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceQBMUL.txt
                         ${TEMP}/ioTvImageMetadataInterfaceQBMUL.txt
        otbImageMetadataInterfaceTest
        ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/ioTvImageMetadataInterfaceQBMUL.txt
)
ADD_TEST(ioTvImageMetadataInterfaceIKONOSTest ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceIKONOS.txt
                         ${TEMP}/ioTvImageMetadataInterfaceIKONOS.txt
        otbImageMetadataInterfaceTest
        ${LARGEDATA}/IKONOS/BLOSSEVILLE/po_2619900_blu_0000000.tif
        ${TEMP}/ioTvImageMetadataInterfaceIKONOS.txt
)
ADD_TEST(ioTvImageMetadataInterfaceSPOTTest ${IO_TESTS18}
         --compare-ascii ${NOTOL} ${BASELINE_FILES}/ioTvImageMetadataInterfaceSPOT.txt
                         ${TEMP}/ioTvImageMetadataInterfaceSPOT.txt
        otbImageMetadataInterfaceTest
        ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
        ${TEMP}/ioTvImageMetadataInterfaceSPOT.txt
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(ioTvImageMetadataInterfaceTest ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvImageMetadataInterface.txt
                         ${TEMP}/ioTvImageMetadataInterface.txt
        otbImageMetadataInterfaceTest2
        ${EXAMPLEDATA}/qb_RoadExtract.tif
        ${TEMP}/ioTvImageMetadataInterface.txt
)


IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvTerraSarImageMetadataInterface_dims ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTerraSarImageMetadataInterface_dims.txt
                         ${TEMP}/ioTvTerraSarImageMetadataInterface_dims.txt
        otbTerraSarImageMetadataInterface
        ${LARGEDATA}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_strip_011.cos
        ${TEMP}/ioTvTerraSarImageMetadataInterface_dims.txt
)
ADD_TEST(ioTvTerraSarImageMetadataInterface_TORONTO ${IO_TESTS18}
         --compare-ascii ${EPSILON_9} ${BASELINE_FILES}/ioTvTerraSarImageMetadataInterface_TORONTO.txt
                         ${TEMP}/ioTvTerraSarImageMetadataInterface_TORONTO.txt
        otbTerraSarImageMetadataInterface
        ${LARGEDATA}/TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos
        ${TEMP}/ioTvTerraSarImageMetadataInterface_TORONTO.txt
)

ADD_TEST(ioTvImageKeywordlistSpot5 ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5_saveState_loadState.txt
         --ignore-lines-with 3 support_data.metadata_file meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
         ${TEMP}/ioTvImageKeywordlistSpot5.txt
         ${TEMP}/ioTvImageKeywordlistSpot5_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistQuickbird ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird_saveState_loadState.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         ${TEMP}/ioTvImageKeywordlistQuickbird.txt
         ${TEMP}/ioTvImageKeywordlistQuickbird_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistIkonos ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos_saveState_loadState.txt
         --ignore-lines-with 2 meters_per_pixel_x meters_per_pixel_y
    otbImageKeywordlist
         ${LARGEDATA}/IKONOS/BLOSSEVILLE/po_2619900_pan_0000000.tif
         ${TEMP}/ioTvImageKeywordlistIkonos.txt
         ${TEMP}/ioTvImageKeywordlistIkonos_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistRapidEye ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEDATA}/RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf
         ${TEMP}/ioTvImageKeywordlistRapidEye.txt
         ${TEMP}/ioTvImageKeywordlistRapidEye_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistEnvisatAsar ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEDATA}/ENVISAT_ASAR_SCENE01/ASA_APS_1PNDPA20030821_7713.N1
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar.txt
         ${TEMP}/ioTvImageKeywordlistEnvisatAsar_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistAlosPalsar ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEDATA}/PALSAR/200801280007/l1data/VOL-ALPSRP037120700-H1.1__A
         ${TEMP}/ioTvImageKeywordlistAlosPalsar.txt
         ${TEMP}/ioTvImageKeywordlistAlosPalsar_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistRadarSat1 ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat1.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001
         ${TEMP}/ioTvImageKeywordlistRadarSat1.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat1_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistRadarSat2 ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2_saveState_loadState.txt
	  --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEDATA}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
         ${TEMP}/ioTvImageKeywordlistRadarSat2.txt
         ${TEMP}/ioTvImageKeywordlistRadarSat2_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistTerraSarX ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_saveState_loadState.txt
	  --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEDATA}/TERRASARX/dims/TSX-1.SAR.L1B/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistTerraSarX_Upsala ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala_saveState_loadState.txt
	  --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEDATA}/TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/TSX1_SAR__MGD.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Upsala_saveState_loadState.txt
)
ADD_TEST(ioTvImageKeywordlistTerraSarX_Toronto ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto_saveState_loadState.txt
	  --ignore-lines-with 1 product_xml_filename
    otbImageKeywordlist
         ${LARGEDATA}/TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto.txt
         ${TEMP}/ioTvImageKeywordlistTerraSarX_Toronto_saveState_loadState.txt
)

ADD_TEST(ioTvImageKeywordlistErs ${IO_TESTS18}
  --compare-n-ascii ${NOTOL} 2
         ${BASELINE_FILES}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs.txt
         ${BASELINE_FILES}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs_saveState_loadState.txt
    otbImageKeywordlist
         ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
         ${TEMP}/ioTvImageKeywordlistErs.txt
         ${TEMP}/ioTvImageKeywordlistErs_saveState_loadState.txt
)

ENDIF(OTB_DATA_USE_LARGEINPUT)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ otbIOTESTS19 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(ioTvImageFileReaderServerNameHTTPTest ${IO_TESTS19}
	otbImageFileReaderServerName
	http://tile.openstreetmap.org/)

ADD_TEST(ioTvImageFileReaderServerNameOTBTest ${IO_TESTS19}
	otbImageFileReaderServerName
	${OTB_DATA_ROOT}/Examples/osmfile.otb)

IF(OTB_USE_CURL)

ADD_TEST(ioTuTileMapImageSourceNew ${IO_TESTS19}
        otbTileMapImageSourceNew )

#ADD_TEST(ioTvTileMapImageSource ${IO_TESTS19}
#        --compare-image ${NOTOL}
#                ${BASELINE}/ioTileMapImageSourceToulouse.png
#                ${TEMP}/ioTileMapImageSourceToulouse.png
#        otbTileMapImageSource
#                43.6044
#                1.44295
#                http://tile.openstreetmap.org
#		${TEMP}/ioTileMapImageSourceToulouse.png
#		256)

ADD_TEST(ioTuTileMapFetcherNew ${IO_TESTS19}
        otbTileMapFetcherNew )

ADD_TEST(ioTvTileMapFetcherTest ${IO_TESTS19}
	otbTileMapFetcherTest
		132122
		95719
		18
		http://tile.openstreetmap.org/
                #http://andy.sandbox.cloudmade.com/tiles/cycle/
		${TEMP}/
		2
		${TEMP}/)

# We do not perform regression testing here because images on server might change
ADD_TEST(ioTvTileMapImageIOFile ${IO_TESTS19}
otbTileMapImageIOTest
    ${OTB_DATA_ROOT}/Examples/osmfile.otb
    ${TEMP}/ioTvTileMapImageIOFileOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)

ADD_TEST(ioTvTileMapImageIOWeb ${IO_TESTS19}
otbTileMapImageIOTest
    http://tile.openstreetmap.org/
    ${TEMP}/ioTvTileMapImageIOWebOutput.png
    ${TEMP}
    1.4835345 43.55968261 13
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(ioTvTileMapWriter ${IO_TESTS19}
    otbTileMapWriter
       ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
#       ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
       ${TEMP}
)
ENDIF(OTB_DATA_USE_LARGEINPUT)

ENDIF(OTB_USE_CURL)

#----------------------------------------------------------------------------------
SET(BasicIO_SRCS1
otbIOTests1.cxx
otbImageFileReaderERS.cxx
otbLUMImageIOTestCanRead.cxx
otbLUMImageIOTestCanWrite.cxx
otbMWImageIOTestCanRead.cxx
otbMWImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileReaderTestFloat.cxx
otbShortImageIOTest.cxx
otbImageIOFactoryNew.cxx
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

#SET(BasicIO_SRCS4
#)
#CONTAINS THE BasicIO_SRCS2 include
SET(BasicIO_SRCS5
otbIOTests5.cxx
otbGDALImageIOTestCanRead.cxx
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
otbDXFToSpatialObjectGroupFilterNew.cxx
otbSpatialObjectDXFReaderNew.cxx
otbSpatialObjectDXFReader.cxx
)
SET(BasicIO_SRCS12
otbIOTests12.cxx
otbDEMHandlerNew.cxx
otbDEMHandlerTest.cxx
otbDEMToImageGeneratorNew.cxx
otbDEMToImageGeneratorTest.cxx
otbOssimElevManagerTest.cxx
otbOssimElevManagerTest2.cxx
otbOssimElevManagerTest4.cxx
otbDEMToOrthoImageGeneratorNew.cxx
otbDEMToOrthoImageGeneratorTest.cxx
otbPrepareSRTMDirectoryNew.cxx
otbPrepareSRTMDirectoryTest.cxx
)


IF(OTB_COMPILE_JPEG2000)
SET(BasicIO_SRCS13
otbIOTests13.cxx
otbJPEG2000ImageIOTestCanRead.cxx
otbJPEG2000ImageIOTestCanWrite.cxx
)
ENDIF(OTB_COMPILE_JPEG2000)


SET(BasicIO_SRCS14
otbIOTests14.cxx
otbGDALDriverDoubleWritingTest.cxx
otbReadingComplexDataIntoComplexImageTest.cxx
otbWritingComplexDataWithComplexImageTest.cxx
)



SET(BasicIO_SRCS15
otbIOTests15.cxx
otbVectorDataSourceNew.cxx
otbVectorDataFileReaderNew.cxx
otbVectorDataFileReader.cxx
otbVectorDataFileWriterNew.cxx
otbVectorDataFileWriter.cxx
otbVectorDataFileWriterPolygons.cxx
otbVectorDataFileReaderWriter.cxx
otbVectorDataFileGeoReaderWriter.cxx
otbOGRVectorDataIONew.cxx
otbOGRVectorDataIOCanRead.cxx
otbOGRVectorDataIOCanWrite.cxx
otbKMLVectorDataIONew.cxx
otbKMLVectorDataIOTestCanRead.cxx
otbKMLVectorDataIOTestCanWrite.cxx
otbKMLVectorDataIOTestFileReader.cxx
otbVectorDataKeywordlist.cxx
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
otbImageKeywordlist.cxx
otbTerraSarImageMetadataInterface.cxx
otbSpotImageMetadataInterfaceNew.cxx
otbQuickBirdImageMetadataInterfaceNew.cxx
otbIkonosImageMetadataInterfaceNew.cxx
otbTerraSarImageMetadataInterfaceNew.cxx
otbImageMetadataInterfaceTest.cxx
otbImageMetadataInterfaceTest2.cxx
)

IF(OTB_USE_CURL)
SET(BasicIO_SRCS19
otbIOTests19.cxx
otbImageFileReaderServerName.cxx
otbTileMapImageSourceNew.cxx
otbTileMapImageSource.cxx
otbTileMapFetcherNew.cxx
otbTileMapFetcherTest.cxx
otbTileMapImageIOTest.cxx
otbTileMapWriter.cxx
)
ENDIF(OTB_USE_CURL)

OTB_ADD_EXECUTABLE(otbIOTests1 "${BasicIO_SRCS1}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests2 "${BasicIO_SRCS2}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests3 "${BasicIO_SRCS3}" "OTBIO;OTBTesting")
#OTB_ADD_EXECUTABLE(otbIOTests4 "${BasicIO_SRCS4}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests5 "${BasicIO_SRCS5}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests6 "${BasicIO_SRCS6}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests7 "${BasicIO_SRCS7}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests8 "${BasicIO_SRCS8}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests9 "${BasicIO_SRCS9}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests10 "${BasicIO_SRCS10}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests11 "${BasicIO_SRCS11}" "OTBIO;OTBTesting")
OTB_ADD_EXECUTABLE(otbIOTests12 "${BasicIO_SRCS12}" "OTBIO;OTBTesting")



# Case for releae 2.2.1
IF(OTB_COMPILE_JPEG2000)
   IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DOPJ_STATIC)
   ENDIF(NOT BUILD_SHARED_LIBS)

   OTB_ADD_EXECUTABLE(otbIOTests13 "${BasicIO_SRCS13}" "OTBIO;OTBTesting")
ENDIF(OTB_COMPILE_JPEG2000)

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

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

