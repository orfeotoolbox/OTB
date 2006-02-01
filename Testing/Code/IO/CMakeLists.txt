# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)




SET(IO_TESTS ${CXX_TEST_PATH}/otbIOTests)

# Test creation d'une image BSQ de 3 canaux RGB
ADD_TEST(ioTvCreationRGB_2_BSQ ${IO_TESTS} 
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/sbuv.hd
         0 0 800 600
         1 2 3 )

ADD_TEST(ioTvCreationBSQ_2_RGB ${IO_TESTS} 
  --compare-image   ${INPUTDATA}/sbuv.png
                    ${TEMP}/sbuv_2.png
         otbExtractROI_VectorToRGB
         ${TEMP}/sbuv.hd 
         ${TEMP}/sbuv_2.png
         0 0 800 600
         1 2 3 )




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
ADD_TEST(ioTuCAIImageIOCanWriteBSQ ${IO_TESTS} otbCAIImageIOTestCanWrite ${TEMP}/IoCAIImageIOCanWriteBSQ.hd )

# -------            otb::GDALImageIO   ------------------------------
# CanRead()
#ADD_TEST(ioTuGDALImageIOCanRead ${IO_TESTS} otbGDALImageIOTestCanRead
#        ${INPUTDATA}/poupees.jpg )




# -------            otb::ImageFileReader pour CAI  ------------------------------
# Niveau de Gris
ADD_TEST(ioTvImageFileReaderPNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/IoImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/IoImageFileReaderPNG2PNG_cthead1.png )
# RGB
ADD_TEST(ioTvImageFileReaderRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_PNG2PNG_sbuv.png
                    ${TEMP}/IoImageFileReaderRGB_PNG2PNG_sbuv.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileReaderRGB_PNG2PNG_sbuv.png )
ADD_TEST(ioTvImageFileReaderRGB_PNG2TIFF ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff
                    ${TEMP}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_BSQ2PNG_poupees.png
                    ${TEMP}/IoImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/IoImageFileReaderRGB_BSQ2PNG_poupees.png )
ADD_TEST(ioTvImageFileReaderRGB_BSQ2TIFF ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff
                    ${TEMP}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(ioTvImageFileReaderRGB_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF 
        ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png )
ADD_TEST(ioTvImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
                    ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesBIL
        ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )

# -------            otb::ExtractROI   ------------------------------
# -------            otb::VectorToRGBImageFilter   ------------------------------

ADD_TEST(ioTvExtractROI_VectorToRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
         700 60 77 489
         1 2 3 )

ADD_TEST(ioTvExtractROI_VectorToRGB_PNG2PNGBis ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
                    ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
         700 60 77 489
         3 2 1 )

ADD_TEST(ioTvExtractROI_RGB_SPOT42PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
         700 60 77 489
         1 2 3 )

ADD_TEST(ioTvExtractROI_RGB_SPOT42PNGBis ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
                    ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
         700 60 77 489
         3 2 1 )

ADD_TEST(ioTvExtractROI_VectorToRGB_BSQ2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_VectorToRGB_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_VectorToRGB_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/poupees.hd
         ${TEMP}/IoExtractROI_VectorToRGB_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         100 100 77 300
         1 2 3 )

ADD_TEST(ioTvExtractROI_Scalar_SPOT42PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_1.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_1.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_1.png
         700 60 77 489
         1 )
ADD_TEST(ioTvExtractROI_Scalar_SPOT42PNGBis ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_2.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_2.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_2.png
         700 60 77 489
         2 )
ADD_TEST(ioTvExtractROI_Scalar_SPOT42PNGTer ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_3.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_3.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channel_3.png
         700 60 77 489
         3 )

# Test sur l'image SPOT 5 4 canaux
ADD_TEST(ioTvExtractROI_RGB_SPOT52PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
         700 700 1000 1000
         1 2 3 )

ADD_TEST(ioTvExtractROI_Scalar_SPOT52PNG1 ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
         700 700 1000 1000
         1 )
ADD_TEST(ioTvExtractROI_Scalar_SPOT52PNG2 ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
         700 700 1000 1000
         2 )
ADD_TEST(ioTvExtractROI_Scalar_SPOT52PNG3 ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
         700 700 1000 1000
         3 )
ADD_TEST(ioTvExtractROI_Scalar_SPOT52PNG4 ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
         700 700 1000 1000
         4 )





# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileWriterPNG2PNG_cthead1.png
                    ${TEMP}/IoImageFileWriterPNG2PNG_cthead1.png
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/IoImageFileWriterPNG2PNG_cthead1.png )

ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS} 
  --compare-binary   ${BASELINE}/IoImageFileWriterPNG2BSQ_cthead1.c1
                    ${TEMP}/IoImageFileWriterPNG2BSQ_cthead1.c1
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/IoImageFileWriterPNG2BSQ_cthead1.hd )

# RGB
ADD_TEST(ioTvImageFileWriterRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileWriterRGB_PNG2PNG_sbuv.png
                    ${TEMP}/IoImageFileWriterRGB_PNG2PNG_sbuv.png
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileWriterRGB_PNG2PNG_sbuv.png )

ADD_TEST(ioTvImageFileWriterRGB_PNG2BSQ ${IO_TESTS} 
  --compare-binary   ${BASELINE}/IoImageFileWriterRGB_PNG2BSQ_sbuv.c1
                    ${TEMP}/IoImageFileWriterRGB_PNG2BSQ_sbuv.c1
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileWriterRGB_PNG2BSQ_sbuv.hd )




#ADD_TEST(ioTvImageFileWriterPNG2PNG ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.png )

#ADD_TEST(ioTvImageFileWriterPNG2BSQ ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.hd )


#ADD_TEST(ioTvImageFileWriterRGBPNG2BSQ ${IO_TESTS} otbImageFileWriterRGBTest
#        ${INPUTDATA}/sbuv.png 
#        ${TEMP}/otb_writer_RGB_poupees.hd )

SET(BasicIO_SRCS
otbExtractROI_VectorToRGB.cxx
otbExtractROI_VectorToScalar.cxx
otbCAIImageIOTestCanRead.cxx
otbCAIImageIOTestCanWrite.cxx
#otbGDALImageIOTestCanRead.cxx
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




