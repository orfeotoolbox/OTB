# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

# !!!!!  Pointe sur un exe généré dans le repertoire Common
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)
SET(IO_TESTS     ${CXX_TEST_PATH}/otbIOTests)





# ---  RADARSAT (GDAL)  ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT2ENVI ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -ushort ${IMAGEDATA}/RADARSAT1/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  Nom r�pertoire ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT_ENVI2ENVI ${COMMON_TESTS} 
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL} ${BASELINE}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
#                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2ENVI_2559_3591_337_280.hdr
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT_ENVI2ENVI_2559_3591_337_280_2.hdr
         -startX 0 -startY 0 -sizeX 337 -sizeY 280 )

# ---  SAR_ERS2_SLCI (GDAL)  ---

ADD_TEST(ioTvImageFileReaderERS ${IO_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderERS_SLCI.png
#                            ${TEMP}/ioImageFileReaderERS_SLCI.png
         otbImageFileReaderERS
         ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioImageFileReaderERS_SLCI.png)

ADD_TEST(ioTvImageFileReaderRADARSAT ${IO_TESTS} 
   --compare-image ${TOL} ${BASELINE}/ioImageFileReaderRADARSAT.png
                           ${TEMP}/ioImageFileReaderRADARSAT.png
         otbImageFileReaderERS
         ${IMAGEDATA}/RADARSAT1/SCENE01/
         ${TEMP}/ioImageFileReaderRADARSAT.png)


# ---  ENVI (GDAL) ---
ADD_TEST(ioTvMultiMonoChannelROI_ENVI2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         -startX 302 -startY 2 -sizeX 134 -sizeY 330
         -channel 1)





# ------- TEST IO COMMON   ------------------------------


# -------            otb::LUMImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuLUMImageIOCanRead ${IO_TESTS} otbLUMImageIOTestCanRead
        ${INPUTDATA}/poupees.lum)

# CanWrite()
ADD_TEST(ioTuLUMImageIOCanWrite ${IO_TESTS} otbLUMImageIOTestCanWrite
        ${TEMP}/poupees.lum)

ADD_TEST(ioTvImageFileReaderHDR2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_1canal.c1.hdr
                                ${TEMP}/ioImageFileReaderHDR2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.c1.hdr 
        ${TEMP}/ioImageFileReaderHDR2LUM.lum )

ADD_TEST(ioTvImageFileReaderPNG2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2LUM.lum
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileReaderPNG2LUM.lum )

ADD_TEST(ioTvImageFileReaderLUM2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioImageFileReaderLUM2LUM.lum
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderPNG2LUM.lum 
        ${TEMP}/ioImageFileReaderLUM2LUM.lum )

ADD_TEST(ioTvImageFileWriterLUM2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderLUM2HDR.hdr
        otbImageFileReaderTest
        ${TEMP}/ioImageFileReaderLUM2LUM.lum 
        ${TEMP}/ioImageFileReaderLUM2HDR.hdr )

# --- Image I2
ADD_TEST(ioTvImageFileReaderShortLUM2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum
        otbShortImageIOTest
        ${TEMP}/ioImageFileReaderI2_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2LUM.lum )

ADD_TEST(ioTvImageFileReaderShortLUM2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.lum
                                ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum
        otbShortImageIOTest
        ${INPUTDATA}/poupees_I2.lum
        ${TEMP}/ioImageFileReaderI2_LUM2LUM.lum )

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntLUM2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.lum
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderIntHDR2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.lum
                                ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2LUM.lum )

# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatLUM2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.lum
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderFloatHDR2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.lum
                                ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2LUM.lum )

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleLUM2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.lum
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr )
ADD_TEST(ioTvImageFileReaderDoubleHDR2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.lum
                                ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_LUM2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2LUM.lum )




ADD_TEST(ioTvImageFileReaderFloatPNG2LUM ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum
        otbFloatImageIOTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileReaderFloatPNG2LUM.lum )


# ---  STREAMING : LUM  ---
ADD_TEST(ioTvStreamingImageFilterLUM2PNG ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFilterTestLUM.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFilterTestLUM.png
         )

# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithoutStreaming ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithoutStreaming.lum
         0 # Streaming
         )
# ---  LUM STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterLUMWithStreaming ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2LUM.lum
                                ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2LUM.lum
         ${TEMP}/ioStreamingImageFileWriterLUM2LUMWithStreaming_10.lum
         1 # Streaming
         10
         )


# -------            otb::BSQImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuBSQImageIOCanRead ${IO_TESTS} otbBSQImageIOTestCanRead
        ${INPUTDATA}/poupees.hd)

# CanWrite()
ADD_TEST(ioTuBSQImageIOCanWrite ${IO_TESTS} otbBSQImageIOTestCanWrite
        ${TEMP}/poupees.hd)

ADD_TEST(ioTvImageFileReaderPNG2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/cthead1.png
                                ${TEMP}/ioImageFileReaderPNG2BSQ.hd
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileReaderPNG2BSQ.hd )

ADD_TEST(ioTvImageFileReaderPNG2ENVI ${IO_TESTS} 
  --compare-image ${TOL}        ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioImageFileReaderPNG2ENVI.hdr
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileReaderPNG2ENVI.hdr )

ADD_TEST(ioTvImageFileReaderBSQ2PNG ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_1canal.hd
                                ${TEMP}/ioImageFileReaderBSQ2PNG.png
        otbImageFileReaderTest
        ${INPUTDATA}/poupees_1canal.hd 
        ${TEMP}/ioImageFileReaderBSQ2PNG.png )

ADD_TEST(ioTvImageFileReaderRGB_PNG2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/sbuv.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileReaderRGB_PNG2BSQ.hd )

ADD_TEST(ioTvImageFileReaderRGB_PNG2ENVI ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/sbuv.png
                                ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileReaderRGB_PNG2ENVI.hdr )

# --- Image I2
ADD_TEST(ioTvImageFileReaderShortRGB_BSQ2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        otbShortRGBImageIOTest
        ${INPUTDATA}/poupees_I2.hd
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr )

ADD_TEST(ioTvImageFileReaderShortHDR2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I2.hd
                                ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd
        otbShortRGBImageIOTest
        ${TEMP}/ioImageFileReaderI2_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI2_HDR2BSQ.hd )

# --- Image I4
ADD_TEST(ioTvImageFileReaderIntBSQ2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        otbIntImageIOTest
        ${INPUTDATA}/poupees_I4.hd
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderIntHDR2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_I4.hd
                                ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd
        otbIntImageIOTest
        ${TEMP}/ioImageFileReaderI4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderI4_HDR2BSQ.hd )

# --- Image FLOAT
ADD_TEST(ioTvImageFileReaderFloatBSQ2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        otbFloatImageIOTest
        ${INPUTDATA}/poupees_R4.hd
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderFloatHDR2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R4.hd
                                ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd
        otbFloatImageIOTest
        ${TEMP}/ioImageFileReaderR4_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR4_HDR2BSQ.hd )

# --- Image DOUBLE
ADD_TEST(ioTvImageFileReaderDoubleBSQ2HDR ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        otbDoubleImageIOTest
        ${INPUTDATA}/poupees_R8.hd
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr )
ADD_TEST(ioTvImageFileReaderDoubleHDR2BSQ ${IO_TESTS} 
  --compare-image ${TOL}        ${INPUTDATA}/poupees_R8.hd
                                ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd
        otbDoubleImageIOTest
        ${TEMP}/ioImageFileReaderR8_BSQ2HDR.hdr
        ${TEMP}/ioImageFileReaderR8_HDR2BSQ.hd )


# ---  STREAMING FILTER : BSQ  ---
ADD_TEST(ioTvStreamingImageFilterBSQ2PNG ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFilterTestBSQ.png
         otbStreamingImageFilterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFilterTestBSQ.png
         )

# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithoutStreaming.hd
         0 # Streaming
         )
# ---  BSQ STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS} 
   --compare-image ${TOL}       ${TEMP}/ioImageFileReaderPNG2BSQ.hd
                                ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         otbStreamingImageFileWriterTest
         ${TEMP}/ioImageFileReaderPNG2BSQ.hd
         ${TEMP}/ioStreamingImageFileWriterBSQ2BSQWithStreaming_10.hd
         1 # Streaming
         10
         )



# -------            otb::GDALImageIO   ------------------------------

# READER

ADD_TEST(ioTuGDALImageIOCanRead_ENVI ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${INPUTDATA}/poupees_1canal.c1.hdr )

ADD_TEST(ioTuGDALImageIOCanRead_RADARSAT ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/RADARSAT1/SCENE01/ )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/ )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_pan_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_GRN ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_grn_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_BLU ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_blu_0000000.tif)
	
ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_RED ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_red_0000000.tif)
	
ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_NIR ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif)

ADD_TEST(ioTuGDALImageIOCanRead_SPOT4 ${IO_TESTS} otbGDALImageIOTestCanRead
        ${IMAGEDATA}/SPOT4_SCENE01/ )

ADD_TEST(ioTuGDALImageIOCanRead_SPOT5TIF ${IO_TESTS} otbGDALImageIOTestCanRead
        ${INPUTDATA}/poupeesTIF/ )

ADD_TEST(ioTuGDALImageIOCanRead_HFA ${IO_TESTS} otbGDALImageIOTestCanRead
	${INPUTDATA}/HFAGeoreferenced.img)

        
# WRITER

ADD_TEST(ioTuGDALImageIOCanWrite_TIFF ${IO_TESTS} otbGDALImageIOTestCanWrite
	${INPUTDATA}/testGDALWriter.tif)
ADD_TEST(ioTuGDALImageIOCanWrite_HFA ${IO_TESTS} otbGDALImageIOTestCanWrite
	${INPUTDATA}/HFAGeoreferenced.img)

# CanWrite()
# ADD_TEST(ioTuCAIImageIOCanWriteBSQ ${IO_TESTS} otbCAIImageIOTestCanWrite 
#        ${TEMP}/ioCAIImageIOCanWriteBSQ.hd )


# -------            otb::ONERAImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuONERAImageIOCanRead ${IO_TESTS} otbONERAImageIOTestCanRead
        ${IMAGEDATA}/ONERA/spa3_0215_rad.ent)
	

ADD_TEST(ioTvONERAImageFileReader ${IO_TESTS} 
  --compare-image ${TOL}  ${BASELINE}/ioImageFileReaderONERA.hdr
                          ${TEMP}/ioImageFileReaderONERA.hdr
        otbImageFileReaderONERATest
        ${IMAGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERA.hdr )

ADD_TEST(ioTvONERAImageFileReaderComplex ${IO_TESTS} 
  --compare-n-images ${TOL} 2  
			  ${BASELINE}/ioImageFileReaderONERAComplexReal.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
			  ${BASELINE}/ioImageFileReaderONERAComplexImag.hdr
			  ${TEMP}/ioImageFileReaderONERAComplexImag.hdr
        otbImageFileReaderONERAComplex
        ${IMAGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexReal.hdr
	${TEMP}/ioImageFileReaderONERAComplexImag.hdr )

ADD_TEST(ioTvONERAImageFileWriterComplex ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderONERAComplexWrite.ent
                           ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
        otbImageFileWriterONERAComplex
        ${IMAGEDATA}/ONERA/spa3_0215_rad.ent
        ${TEMP}/ioImageFileReaderONERAComplexWrite.ent )

ADD_TEST(ioTvONERAImageStreamWriter ${IO_TESTS} 
  --compare-image ${TOL}   ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
  			   ${TEMP}/ioImageFileReaderONERAStreamWrite.ent
        otbImageFileWriterStreamingONERAComplex
        ${TEMP}/ioImageFileReaderONERAComplexWrite.ent
        ${TEMP}/ioImageFileReaderONERAStreamWrite.ent )

# -------            otb::ONERAImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuMSTARImageIOCanRead1 ${IO_TESTS} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB19896.016)

ADD_TEST(ioTuMSTARImageIOCanRead2 ${IO_TESTS} otbMSTARImageIOTestCanRead
        ${INPUTDATA}/MSTAR/HB20010.000)

ADD_TEST(ioTvImageFileReaderMSTAR ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderMSTAR_rescal_chanel1.png
                    ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png
        otbImageFileReaderMSTAR
        ${INPUTDATA}/MSTAR/HB19896.016
        ${TEMP}/ioImageFileReaderMSTAR_rescal_chanel1.png )

# -------            otb::ImageFileReader pour CAI  ------------------------------
# Niveau de Gris
ADD_TEST(ioTvImageFileReaderPNG2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/ioImageFileReaderPNG2PNG_cthead1.png )
# RGB
ADD_TEST(ioTvImageFileReaderRGB_PNG2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_PNG2PNG_sbuv.png
                    ${TEMP}/ioImageFileReaderRGB_PNG2PNG_sbuv.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileReaderRGB_PNG2PNG_sbuv.png )
ADD_TEST(ioTvImageFileReaderRGB_PNG2TIFF ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff
                    ${TEMP}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileReaderRGB_PNG2TIFF_sbuv.tiff )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/poupees.hdr
                    ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr 
        ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )

ADD_TEST(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/poupees.hdr 
                    ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hdr 
        ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/
        ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )

ADD_TEST(ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF 
        ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

# Suppression CAI
# ADD_TEST(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#                     ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
#         otbImageFileReaderRGBTest
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )


# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileWriterPNG2PNG_cthead1.png )

ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS} 
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr )
ADD_TEST(ioTvImageFileReaderENVI2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
        otbImageFileReaderTest
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hdr 
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png )

# RGB

ADD_TEST(ioTvImageFileWriterRGB_PNG2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileWriterRGB_PNG2PNG_sbuv.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2PNG_sbuv.png
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileWriterRGB_PNG2PNG_sbuv.png )

ADD_TEST(ioTvImageFileWriterRGB_PNG2BSQ ${IO_TESTS} 
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hdr )
        
ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG_2 ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/sbuv.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png
        otbImageFileReaderRGBTest
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hdr
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png )
        
# WRITER GDAL        
ADD_TEST(ioTvGDALImageFileWriterTIF2TIF ${IO_TESTS}
	otbImageFileWriterRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF
        ${TEMP}/poupees.tiff )


# -------            otb::ImageFileReader pour GDAL  ------------------------------
# -------            otb::ImageFileWriter pour GDAL  ------------------------------


ADD_TEST(ioTvImageReaderWriterRgbPNG2PNG ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2PNG_poupees.png )

ADD_TEST(ioTvImageReaderWriterRgbPNG2TIF ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2TIF_poupees.tif )
ADD_TEST(ioTvImageReaderWriterRgbPNG2ENVI ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees.png
                     ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2ENVI_poupees.hdr )
ADD_TEST(ioTvImageReaderWriterRgbPNG2JPEG ${IO_TESTS} 
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL} ${INPUTDATA}/poupees.png
#                 ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.png
        ${TEMP}/ioImageReaderWriterRgbPNG2JPEG_poupees.jpg )

ADD_TEST(ioTvImageReaderWriterRgbTIF2PNG ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees.tif
                     ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.tif
        ${TEMP}/ioImageReaderWriterRgbTIF2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbENVI2PNG ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees
                     ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees
        ${TEMP}/ioImageReaderWriterRgbENVI2PNG_poupees.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2PNG ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2PNG_couleurs.png )
ADD_TEST(ioTvImageReaderWriterRgbJPEG2TIF ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/couleurs.jpg
                     ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif
        otbImageFileReaderRGBTest
        ${INPUTDATA}/couleurs.jpg
        ${TEMP}/ioImageReaderWriterRgbJPEG2TIF_couleurs.tif )
        


# -------            otb::MultiChannelExtractROI   ------------------------------


# ---  SPOT4 (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT4_SCENE01/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT4_SCENE01/
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )

# ---  ENVI  ---
ADD_TEST(ioTvMultiChannelROI_ENVI2PNG_2 ${COMMON_TESTS} 
# THOMAS (provisoire) : Images OK mais erreur au DIFF ITK !!!
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/poupees.hdr
         -uchar ${TEMP}/ioExtractROI_ENVI2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---  
 

# ADD_TEST(ioTvMultiChannelROI_SPOT5BIL2PNG_2 ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/poupeesBIL
#         ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         -startX 100 -startY 100 -sizeX 77 -sizeY 300
#         -channels 1 2 3 )

# ---  SPOT5 (CAI)  ---
# ---  Test sur l'image SPOT 5 4 canaux  ---
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01/
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
ADD_TEST(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
ADD_TEST(ioTvMultiChannelROI_IkonosRED2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
                            ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_red_0000000.tif
         ${TEMP}/ioExtractROI_IkonosRED2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosGRN2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_grn_0000000.tif
         ${TEMP}/ioExtractROI_IkonosGRN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosBLU2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_blu_0000000.tif
         ${TEMP}/ioExtractROI_IkonosBLU2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

ADD_TEST(ioTvMultiChannelROI_IkonosNIR2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  LUM ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2LUM ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                     ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2LUM_100_100_77_300.lum
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  BSQ ----
ADD_TEST(ioTvMultiChannelROI_IkonosNIR2BSQ ${COMMON_TESTS} 
   --compare-image ${TOL}       ${BASELINE}/ioExtractROI_IkonosNIR2PNG_100_100_77_300.png
                                ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS/Ikonos/po_79039_nir_0000000.tif
         ${TEMP}/ioExtractROI_IkonosNIR2BSQ_100_100_77_300.hd
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird PAN  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdPAN2TIFF ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdPAN2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )

# ---  Quickbird XS  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdXS2TIFF ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
                     ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         otbMultiChannelExtractROI
         -ushort ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
         -ushort ${TEMP}/ioExtractROI_QuickbirdXS2TIFF_1000_1000_100_150.tif
         -startX 1000 -startY 1000 -sizeX 100 -sizeY 150
         )
#         -channels 1 2 3)

ADD_TEST(ioTuOtbImageTestSAR_ERS ${IO_TESTS} 
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest 
         ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
ADD_TEST(ioTuOtbImageTestRadarsat ${IO_TESTS} 
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbImageTestRadarsat.txt
        otbImageTest 
       	${IMAGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestRadarsat ${IO_TESTS} 
   --compare-ascii ${EPS}  ${TEMP}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
        otbVectorImageTest 
        ${IMAGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbVectorImageTestRadarsat.txt)


# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestSpot5 ${IO_TESTS} 
   --compare-ascii ${EPS}  ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageTest 
        ${IMAGEDATA}/SPOT5_SCENE01/
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)
        


# ---  STREAMING : GDAL  ---
ADD_TEST(ioTvStreamingImageFilterGDAL_ENVI ${IO_TESTS} 
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
   --compare-image ${TOL} ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFilterTestCDAL_ENVI.png
         )

# ---  STREAMING : GDAL  ---
ADD_TEST(ioTvStreamingImageFilterGDAL_TIFF ${IO_TESTS} 
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
#                      ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupeesTIF/IMAGERY.TIF 
         ${TEMP}/ioStreamingImageFilterTestGDAL_PoupeesTif.png
         )


# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithoutStreaming ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithoutStreaming.hdr
         0 # Streaming
         )
# ---  STREAMING  IMAGE   FILE    WRITER  ---
ADD_TEST(ioTvStreamingIFWriterBSQWithStreaming ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/poupees_1canal.c1.hdr
                      ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterBSQWithStreaming_100.hdr
         1 # Streaming
         100
         )


# ---  STREAMING  IMAGE   FILE    WRITER  ---
# ---  TESTS DIFFERENT ACTIVATE STREAMING PARAMETER  ---
# 1. DEFAULT
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_DEFAULT.tif
         DEFAULT
         )
# 2. AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS.tif
         AUTOMATIC_NUMBER_OF_STREAM_DIVISIONS
         )
# 3. NUMBER_OF_STREAM_DIVISIONS
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_NUMBER_OF_STREAM_DIVISIONS_10.tif
         NUMBER_OF_STREAM_DIVISIONS
         10
         )
# 4. BUFFER_MEMORY_SIZE
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_MEMORY_SIZE_8x2000x10.tif
         BUFFER_MEMORY_SIZE
         8*2000*10
         )
# 5. BUFFER_NUMBER_OF_LINES
ADD_TEST(ioTvStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES ${IO_TESTS} 
   --compare-image ${TOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
                      		${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         otbStreamingImageFileWriterTestCalculateNumberOfDivisions
         ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
         ${TEMP}/ioStreamingImageFileWriterCalculateNumberOfDivisions_BUFFER_NUMBER_OF_LINES_1000.tif
         BUFFER_NUMBER_OF_LINES
         1000
         )

ADD_TEST(ioTvStreamingIFWriterWithFilter ${IO_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioStreamingImageFileWriterWithFilter_1_100.hdr
                            ${TEMP}/ioStreamingImageFileWriterWithFilter_1_100.hdr
         otbStreamingImageFileWriterWithFilterTest
         ${INPUTDATA}/poupees_1canal.c1.hdr
         ${TEMP}/ioStreamingImageFileWriterWithFilter_1_100.hdr
         1
	 1 # Streaming
         100
         )


#---------- Metadata writing tests ------------------------

ADD_TEST(ioTvImageMetadataWriterTest_HFA ${IO_TESTS} 
   --compare-ascii ${TOL}   ${BASELINE_FILES}/ioImageMetadataWriterTest_HFA_out.txt
                            ${TEMP}/ioImageMetadataWriterTest_HFA_out.txt
        otbImageMetadataFileWriterTest
        ${INPUTDATA}/HFAGeoreferenced.img
	    ${TEMP}/temp.img
    	${TEMP}/ioImageMetadataWriterTest_HFA_out.txt
        )

# -------            otb::DXFToSpatialObjectGroupFilter -------------------------------------------
ADD_TEST(coTuDXFToSpatialObjectGroupFilterNew ${IO_TESTS} 
         otbDXFToSpatialObjectGroupFilterNew)  


#----------------------------------------------------------------------------------
# Exemples de tests sur les grosses images
# Tests non lanc�s pour raison taille des fichiers produits
#        ADD_TEST(ioTvStreamingIFWriterPARIS_SPOT5THR_WithoutStreaming ${IO_TESTS} 
#                 otbStreamingImageFileWriterTest
#                 ${IMAGEDATA}/PARIS/SPOT5/THR
#                 ${TEMP}/ioStreamingImageFileWriterTestCAI_ParisSpot5THR_WithoutStreaming.hd
#                 0 # Test sans streaming
#                 )
#        ADD_TEST(ioTvStreamingIFWriterPARIS_SPOT5THR_WithStreaming_1000 ${IO_TESTS} 
#                 otbStreamingImageFileWriterTest
#                 ${IMAGEDATA}/PARIS/SPOT5/THR
#                 ${TEMP}/ioStreamingImageFileWriterTestCAI_ParisSpot5THR_WithStreaming_1000.hd
#                 1 # Test sans streaming
#                 1000 # NbDivisionsStreaming
#                 )
#----------------------------------------------------------------------------------


        
#----------------------------------------------------------------------------------
SET(BasicIO_SRCS
otbGDALImageIOTestCanRead.cxx
otbGDALImageIOTestCanWrite.cxx
otbMSTARImageIOTestCanRead.cxx
otbImageFileReaderMSTAR.cxx
otbONERAImageIOTestCanRead.cxx
otbImageFileReaderONERA.cxx
otbImageFileReaderONERAComplex.cxx
otbImageFileWriterONERAComplex.cxx
otbImageFileWriterStreamingONERAComplex.cxx
otbImageFileReaderTest.cxx
otbImageFileWriterTest.cxx
otbVectorImageFileReaderWriterTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
otbMultiToMonoChannelExtractROISAR.cxx
otbImageFileReaderERS.cxx
otbStreamingImageFilterTest.cxx
otbStreamingImageFileWriterTest.cxx
otbStreamingImageFileWriterWithFilterTest.cxx
otbImageTest.cxx
otbVectorImageTest.cxx
otbLUMImageIOTestCanRead.cxx
otbLUMImageIOTestCanWrite.cxx
otbBSQImageIOTestCanRead.cxx
otbBSQImageIOTestCanWrite.cxx
otbFloatImageIOTest.cxx
otbShortImageIOTest.cxx
otbShortRGBImageIOTest.cxx
otbIntImageIOTest.cxx
otbDoubleImageIOTest.cxx
otbImageMetadataFileWriterTest.cxx
otbStreamingImageFileWriterTestCalculateNumberOfDivisions.cxx
otbDXFToSpatialObjectGroupFilterNew.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbIOTests otbIOTests.cxx ${BasicIO_SRCS})
TARGET_LINK_LIBRARIES(otbIOTests OTBIO OTBCommon  gdal ITKIO ITKCommon dxf)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )




