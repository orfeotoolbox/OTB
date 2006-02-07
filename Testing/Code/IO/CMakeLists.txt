# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)
#Tolerance sur diff pixel image
SET(TOL 0.0)

# !!!!!  Pointe sur un exe généré dan sle repertoire Common
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)



ADD_EXECUTABLE(otbGDALImageIOTestCanRead otbGDALImageIOTestCanRead.cxx )
TARGET_LINK_LIBRARIES(otbGDALImageIOTestCanRead OTBIO OTBCommon ${CAI_LIBRARY} ${GDAL_LIBRARY} ITKIO ITKCommon)
ADD_TEST(ioTuGDALImageIOCanReadPNG ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/sbuv.png )

ADD_TEST(ioTuGDALImageIOCanRead_JPG ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/poupees.jpg )

ADD_TEST(ioTuGDALImageIOCanRead_ENVI ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/poupees_1canal.c1 )

ADD_TEST(ioTuGDALImageIOCanRead_RADARSAT ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/RADARSAT1/SCENE01/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_SAR_ERS2 ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

ADD_TEST(ioTuGDALImageIOCanRead_PALSAR ${CXX_TEST_PATH}/otbGDALImageIOTestCanRead 
        ${INPUTDATA}/PALSAR_L41/PASL4100212160127170308070002/PASL4100212160127170308070002_ortho.dat )






# ------- TEST IO COMMON   ------------------------------

SET(IO_TESTS ${CXX_TEST_PATH}/otbIOTests)


# -------            otb::CAIImageIO   ------------------------------
# CanRead()
ADD_TEST(ioTuCAIImageIOCanReadJPG ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupees.jpg )

ADD_TEST(ioTuCAIImageIOCanReadBSQ ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupees.hd )

ADD_TEST(ioTuCAIImageIOCanReadSPOT4 ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/SPOT4_SCENE01 )

ADD_TEST(ioTuCAIImageIOCanReadSPOT5TIF ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesTIF )

ADD_TEST(ioTuCAIImageIOCanReadSPOT5BIL ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesBIL )

# CanWrite()
ADD_TEST(ioTuCAIImageIOCanWriteBSQ ${IO_TESTS} otbCAIImageIOTestCanWrite ${TEMP}/ioCAIImageIOCanWriteBSQ.hd )




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

ADD_TEST(ioTvImageFileReaderRGB_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image ${TOL}   ${BASELINE}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF 
        ${TEMP}/ioImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png )
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




#ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.png )

#ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.hd )


#ADD_TEST(ioTvImageFileWriterRGBPNG2BSQ ${IO_TESTS} otbImageFileWriterRGBTest
#        ${INPUTDATA}/sbuv.png 
#        ${TEMP}/otb_writer_RGB_poupees.hd )



# -------            otb::MultiChannelExtractROI   ------------------------------
# ---  SPOT4 (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_1_2_3.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 1 2 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT42PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
                     ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/ioExtractROI_SPOT42PNG_spot4_700_60_77_489_channels_3_2_1.png
         -startX 700 -startY 60 -sizeX 77 -sizeY 489
         -channels 3 2 1 )

# ---  BSQ (CAI)  ---
ADD_TEST(ioTvMultiChannelROI_BSQ2PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
                     ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/poupees.hd
         ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
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
         ${INPUTDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channels_1_2_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 2 3 )

ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_2 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_1.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 1 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_3 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_2.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 2 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_4 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_3.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 3 )
ADD_TEST(ioTvMultiChannelROI_SPOT52PNG_5 ${COMMON_TESTS} 
   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
                     ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         otbMultiChannelExtractROI
         ${INPUTDATA}/SPOT5_SCENE01
         ${TEMP}/ioExtractROI_SPOT52PNG_spot5_700_700_1000_1000_channel_4.png
         -startX 700 -startY 700 -sizeX 1000 -sizeY 1000
         -channels 4 )

# ---  BSQ (CAI)  ---
#ADD_TEST(ioTvMultiChannelROI_BSQ2PNG_2 ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#                     ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         otbMultiChannelExtractROI
#         ${INPUTDATA}/poupees.hd
#         ${TEMP}/ioExtractROI_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
#         -startX 100 -startY 100 -sizeX 77 -sizeY 300
#         -channels 1 2 3 )




SET(BasicIO_SRCS
otbCAIImageIOTestCanRead.cxx
otbCAIImageIOTestCanWrite.cxx
otbImageFileReaderTest.cxx
otbImageFileWriterTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
)




INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")
#LINK_DIRECTORIES( $ENV{CAI_DIR_LIB} )


ADD_EXECUTABLE(otbIOTests otbIOTests.cxx ${BasicIO_SRCS})
# CAI ${TIFF_LIBRARY} ${CAI_LIBRARY} ${GDAL_LIBRARY}
TARGET_LINK_LIBRARIES(otbIOTests OTBIO OTBCommon ${CAI_LIBRARY} ${GDAL_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




