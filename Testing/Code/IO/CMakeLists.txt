# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_DIR} )

#Tolerance sur diff pixel image
SET(TOL 0.0)

# !!!!!  Pointe sur un exe généré dans le repertoire Common
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)
SET(IO_TESTS     ${CXX_TEST_PATH}/otbIOTests)

# ---  STREAMING   ---
ADD_TEST(ioTvStreamingImageFilterTest ${IO_TESTS} 
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestRef.png
#                      ${TEMP}/ioStreamingImageFilterTest.png
         otbStreamingImageFilterTest
         ${INPUTDATA}/poupees_1canal.hd
         ${TEMP}/ioStreamingImageFilterTest.png
         )
# ---  STREAMING   ---
ADD_TEST(ioTvStreamingImageFileWriterTest ${IO_TESTS} 
#   --compare-image ${TOL} ${TEMP}/ioStreamingImageFilterTestRef.png
#                      ${TEMP}/ioStreamingImageFilterTest.png
         otbStreamingImageFileWriterTest
         ${INPUTDATA}/poupees_1canal.hd
         ${TEMP}/ioStreamingImageFileWriterTest.png
         )

# ---  RADARSAT (GDAL)  ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT2BSQ ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -ushort ${IMAGEDATA}/RADARSAT1/SCENE01/
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2PNG_2559_3591_337_280.hd
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

# ---  RADARSAT (GDAL)  Nom répertoire ---
ADD_TEST(ioTvMultiMonoChannelROI_RADARSAT_BSQ2BSQ2 ${COMMON_TESTS} 
   --compare-image ${TOL} ${TEMP}/ioMultiMonoChannelROI_RADARSAT2PNG_2559_3591_337_280.hd
                      ${TEMP}/ioMultiMonoChannelROI_RADARSAT2PNG_2559_3591_337_280_2.hd
         otbMultiToMonoChannelExtractROI
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2PNG_2559_3591_337_280.hd
         -ushort ${TEMP}/ioMultiMonoChannelROI_RADARSAT2PNG_2559_3591_337_280_2.hd
         -startX 0 -startY 0 -sizeX 337 -sizeY 280 )

# ---  SAR_ERS2_SLCI (GDAL)  ---

#ADD_TEST(ioTvMultiMonoChannelROI_SAR_ERS2_SLCI2BSQ ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiToMonoChannelExtractROI
#         -ushort ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 
#         -ushort ${TEMP}/ioMultiMonoChannelROI_SAR_ERS2_SLCI2PNG_channel_1.hd
#         -startX 200 -startY 100 -sizeX 1000 -sizeY 1000
#         -channel 1)

ADD_TEST(ioTvImageFileReaderERS ${IO_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbImageFileReaderERS
         ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 
         ${TEMP}/ioImageFileReaderERS_SLCI2PNG_channel_1.hd)

# ---  ENVI (GDAL) ---
ADD_TEST(ioTvMultiMonoChannelROI_ENVI2PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/poupees_1canal.c1
         ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         -startX 302 -startY 2 -sizeX 134 -sizeY 330
         -channel 1)





# ------- TEST IO COMMON   ------------------------------



# -------            otb::CAIImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuCAIImageIOCanReadBSQ ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupees.hd )

ADD_TEST(ioTuCAIImageIOCanReadSPOT4 ${IO_TESTS} otbCAIImageIOTestCanRead
        ${IMAGEDATA}/SPOT4_SCENE01 )

ADD_TEST(ioTuCAIImageIOCanReadSPOT5TIF ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesTIF )

ADD_TEST(ioTuCAIImageIOCanReadSPOT5BIL ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesBIL )

ADD_TEST(ioTuCAIImageIOCanRead_IKONOS_PAN ${IO_TESTS} otbCAIImageIOTestCanRead 
        ${IMAGEDATA}/PARIS_HR/Ikonos/po_79039_pan_0000000.tif)

# -------            otb::GDALImageIO   ------------------------------

ADD_TEST(ioTuGDALImageIOCanRead_ENVI ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${INPUTDATA}/poupees_1canal.c1 )

ADD_TEST(ioTuGDALImageIOCanRead_RADARSAT ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/RADARSAT1/SCENE01/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_PAN ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS_HR/QuickBird/000000183008_01_P001_PAN/02APR06105612-P1BS-000000183008_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_QUICK_XS ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS_HR/QuickBird/000000183008_01_P001_MUL/02APR06105612-M1BS-000000183008_01_P001.TIF )

ADD_TEST(ioTuCAIImageIOCanRead_QUICK_XS ${IO_TESTS} otbCAIImageIOTestCanRead 
        ${IMAGEDATA}/PARIS_HR/QuickBird/000000183008_01_P001_MUL/02APR06105612-M1BS-000000183008_01_P001.TIF )

ADD_TEST(ioTuGDALImageIOCanRead_IKONOS_PAN ${IO_TESTS} otbGDALImageIOTestCanRead 
        ${IMAGEDATA}/PARIS_HR/Ikonos/po_79039_pan_0000000.tif)


# CanWrite()
ADD_TEST(ioTuCAIImageIOCanWriteBSQ ${IO_TESTS} otbCAIImageIOTestCanWrite 
        ${TEMP}/ioCAIImageIOCanWriteBSQ.hd )




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
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
                    ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/ioImageFileReaderRGB_BSQ2PNG_poupees.png )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
                    ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/ioImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(ioTvImageFileReaderRGB_Cai_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF 
        ${TEMP}/ioImageFileReaderRGB_Cai_SPOT5TIF2PNG_poupeesTIF.png )
ADD_TEST(ioTvImageFileReaderRGB_Gdal_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF/IMAGERY.TIF 
        ${TEMP}/ioImageFileReaderRGB_Gdal_SPOT5TIF2PNG_poupeesTIF.png )

ADD_TEST(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
                    ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesBIL
        ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )


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
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hd )
ADD_TEST(ioTvImageFileReaderBSQ2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/cthead1.png
                    ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1_2.png
        otbImageFileReaderTest
        ${TEMP}/ioImageFileWriterPNG2BSQ_cthead1.hd 
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
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hd )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG_2 ${IO_TESTS} 
  --compare-image ${TOL}   ${INPUTDATA}/sbuv.png
                    ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png
        otbImageFileReaderRGBTest
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv.hd
        ${TEMP}/ioImageFileWriterRGB_PNG2BSQ_sbuv_2.png )


# -------            otb::MultiChannelExtractROI   ------------------------------
# ---  SPOT4 (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT4_SCENE01
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT4_SCENE01
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )

# ---  BSQ (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_BSQ2PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         -uchar ${INPUTDATA}/poupees.hd
         -uchar ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 BIL (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT5BIL2PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/poupeesBIL
         ${TEMP}/ioExtractROI_SPOT5BIL2PNG_poupees_100_100_77_300_channels_1_2_3.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3 )

# ---  SPOT5 (CAI)  ---
# ---  Test sur l'image SPOT 5 4 canaux  ---
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  Ikonos PAN  ---
ADD_TEST(ioTvMultiChannelROI_IkonosPAN2PNG ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
#                     ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS_HR/Ikonos/po_79039_pan_0000000.tif
         ${TEMP}/ioExtractROI_IkonosPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Ikonos XS  ---
ADD_TEST(ioTvMultiChannelROI_IkonosXS2PNG ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_IkonosXS2PNG_100_100_77_300.png
#                     ${TEMP}/ioExtractROI_IkonosXS2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS_HR/Ikonos/???
         ${TEMP}/ioExtractROI_IkonosXS2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3)

# ---  Quickbird PAN  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdPAN2PNG ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdPAN2PNG_100_100_77_300.png
#                     ${TEMP}/ioExtractROI_QuickbirdPAN2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS_HR/QuickBird/000000183008_01_P001_PAN/02APR06105612-P1BS-000000183008_01_P001.TIF
         ${TEMP}/ioExtractROI_QuickbirdPAN2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1)

# ---  Quickbird XS  ---
ADD_TEST(ioTvMultiChannelROI_QuickbirdXS2PNG ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_QuickbirdXS2PNG_100_100_77_300.png
#                     ${TEMP}/ioExtractROI_QuickbirdXS2PNG_100_100_77_300.png
         otbMultiChannelExtractROI
         ${IMAGEDATA}/PARIS_HR/QuickBird/000000183008_01_P001_MUL/02APR06105612-M1BS-000000183008_01_P001.TIF
         ${TEMP}/ioExtractROI_QuickbirdXS2PNG_100_100_77_300.png
         -startX 100 -startY 100 -sizeX 77 -sizeY 300
         -channels 1 2 3)

ADD_TEST(ioTuOtbImageTestSAR_ERS ${IO_TESTS} 
   --compare-ascii   ${BASELINE_FILES}/ioOtbImageTestSAR_ERS.txt
                     ${TEMP}/ioOtbImageTestSAR_ERS.txt
         otbImageTest
         ${IMAGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
         ${TEMP}/ioOtbImageTestSAR_ERS.txt)

# ---  Otb::Image Test  ---
ADD_TEST(ioTuOtbImageTestRadarsat ${IO_TESTS} 
   --compare-ascii   ${BASELINE_FILES}/ioOtbImageTestRadarsat.txt
                     ${TEMP}/ioOtbImageTestRadarsat.txt
        otbImageTest
       	${IMAGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbImageTestRadarsat.txt)

# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestRadarsat ${IO_TESTS} 
   --compare-ascii   ${BASELINE_FILES}/ioOtbVectorImageTestRadarsat.txt
                     ${TEMP}/ioOtbVectorImageTestRadarsat.txt
        otbVectorImageTest
        ${IMAGEDATA}/RADARSAT1/SCENE01/
        ${TEMP}/ioOtbVectorImageTestRadarsat.txt)


# ---  Otb::VectorImage Test  ---
ADD_TEST(ioTuOtbVectorImageTestSpot5 ${IO_TESTS} 
   --compare-ascii   ${BASELINE_FILES}/ioOtbVectorImageTestSpot5.txt
                     ${TEMP}/ioOtbVectorImageTestSpot5.txt
        otbVectorImageTest
        ${IMAGEDATA}/SPOT5_SCENE01/IMAGERY.TIF
        ${TEMP}/ioOtbVectorImageTestSpot5.txt)
        
        
SET(BasicIO_SRCS
otbGDALImageIOTestCanRead.cxx
otbCAIImageIOTestCanRead.cxx
otbCAIImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileWriterTest.cxx
otbVectorImageFileReaderWriterTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
otbMultiToMonoChannelExtractROISAR.cxx
otbImageFileReaderERS.cxx
otbStreamingImageFilterTest.cxx
otbStreamingImageFileWriterTest.cxx
otbImageTest.cxx
otbVectorImageTest.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(otbIOTests otbIOTests.cxx ${BasicIO_SRCS})
TARGET_LINK_LIBRARIES(otbIOTests OTBIO OTBCommon  cai ${GDAL_LIBRARY} ITKIO ITKCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING )




