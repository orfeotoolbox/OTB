# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)




SET(IO_TESTS ${CXX_TEST_PATH}/otbIOTests)

# -------            otb::GDALImageIO   ------------------------------
ADD_TEST(tuIoGDALImageIOTest ${IO_TESTS} otbGDALImageIOTest
        ${INPUTDATA}/cthead1.png )


# -------            otb::CAIImageIO   ------------------------------
# CanRead()
ADD_TEST(tuIoCAIImageIOCanReadJPG ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupees.jpg )

ADD_TEST(tuIoCAIImageIOCanReadBSQ ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupees.hd )

ADD_TEST(tuIoCAIImageIOCanReadSPOT4 ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/SPOT4_SCENE01 )

ADD_TEST(tuIoCAIImageIOCanReadSPOT5TIF ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesTIF )

ADD_TEST(tuIoCAIImageIOCanReadSPOT5BIL ${IO_TESTS} otbCAIImageIOTestCanRead
        ${INPUTDATA}/poupeesBIL )

# CanWrite()
ADD_TEST(tuIoCAIImageIOCanWriteJPG ${IO_TESTS} otbCAIImageIOTestCanWrite ${TEMP}/IoCAIImageIOCanWriteJPG.jpg )
ADD_TEST(tuIoCAIImageIOCanWriteBSQ ${IO_TESTS} otbCAIImageIOTestCanWrite ${TEMP}/IoCAIImageIOCanWriteBSQ.hd )



# -------            otb::ImageFileReader pour CAI  ------------------------------
# Niveau de Gris
ADD_TEST(tvIoImageFileReaderPNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderPNG2PNG_cthead1.png
                    ${TEMP}/IoImageFileReaderPNG2PNG_cthead1.png
        otbImageFileReaderTest
        ${INPUTDATA}/cthead1.png
        ${TEMP}/IoImageFileReaderPNG2PNG_cthead1.png )
# RGB
ADD_TEST(tvIoImageFileReaderRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_PNG2PNG_sbuv.png
                    ${TEMP}/IoImageFileReaderRGB_PNG2PNG_sbuv.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileReaderRGB_PNG2PNG_sbuv.png )
ADD_TEST(tvIoImageFileReaderRGB_PNG2TIFF ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff
                    ${TEMP}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileReaderRGB_PNG2TIFF_sbuv.tiff )
ADD_TEST(tvIoImageFileReaderRGB_BSQ2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_BSQ2PNG_poupees.png
                    ${TEMP}/IoImageFileReaderRGB_BSQ2PNG_poupees.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/IoImageFileReaderRGB_BSQ2PNG_poupees.png )
ADD_TEST(tvIoImageFileReaderRGB_BSQ2TIFF ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff
                    ${TEMP}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/IoImageFileReaderRGB_BSQ2TIFF_poupees.tiff )

ADD_TEST(tvIoImageFileReaderRGB_SPOT5TIF2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
                    ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesTIF 
        ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesTIF.png )
ADD_TEST(tvIoImageFileReaderRGB_SPOT5BIL2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
                    ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png
        otbImageFileReaderRGBTest
        ${INPUTDATA}/poupeesBIL
        ${TEMP}/IoImageFileReaderRGB_SPOT5TIF2PNG_poupeesBIL.png )

# TRES LONG
#ADD_TEST(tvIoImageFileReaderRGB_SPOT42PNG ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoImageFileReaderRGB_SPOT42PNG_SPOT4_SCENE01.png
#                    ${TEMP}/IoImageFileReaderRGB_SPOT42PNG_SPOT4_SCENE01.png
#        otbImageFileReaderRGBTest
#        ${INPUTDATA}/SPOT4_SCENE01
#        ${TEMP}/IoImageFileReaderRGB_SPOT42PNG_SPOT4_SCENE01.png )


# -------            otb::ExtractROI   ------------------------------
# -------            otb::VectorToRGBImageFilter   ------------------------------
ADD_TEST(tvIoExtractROI_VectorToRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_1_2_3.png
         700 60 77 489
         1 2 3 )
ADD_TEST(tvIoExtractROI_VectorToRGB_PNG2PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
                    ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/sbuv.png 
         ${TEMP}/IoExtractROI_VectorToRGB_PNG2PNG_sbuv_700_60_77_489_channels_3_2_1.png
         700 60 77 489
         3 2 1 )


ADD_TEST(tvIoExtractROI_RGB_SPOT42PNG ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
                    ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_1_2_3.png
         700 60 77 489
         1 2 3 )

ADD_TEST(tvIoExtractROI_RGB_SPOT42PNGBis ${IO_TESTS} 
  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
                    ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_RGB_SPOT42PNG_700_60_77_489_channels_3_2_1.png
         700 60 77 489
         3 2 1 )

ADD_TEST(tvIoExtractROI_VectorToRGB_BSQ2PNG ${IO_TESTS} 
#  --compare-image   ${BASELINE}/CoExtractROI_VectorToRGB_sbuv_700_60_77_489_channels_1_2_3.png
#                    ${TEMP}/CoExtractROI_VectorToRGB_sbuv_700_60_77_489_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/poupees.hd
         ${TEMP}/IoExtractROI_VectorToRGB_BSQ2PNG_poupees_100_100_77_300_channels_1_2_3.png
         100 100 77 300
         1 2 3 )

ADD_TEST(tvIoExtractROI_Scalar_SPOT42PNG ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_1.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_1.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_1.png
         700 60 77 489
         1 )
ADD_TEST(tvIoExtractROI_Scalar_SPOT42PNGBis ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_2.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_2.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_2.png
         700 60 77 489
         2 )
ADD_TEST(tvIoExtractROI_Scalar_SPOT42PNGTer ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_3.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_3.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT4_SCENE01
         ${TEMP}/IoExtractROI_Scalar_SPOT42PNG_700_60_77_489_channels_3.png
         700 60 77 489
         3 )

# Test sur l'image SPOT 5 4 canaux
ADD_TEST(tvIoExtractROI_RGB_SPOT52PNG ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
#                    ${TEMP}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
         otbExtractROI_VectorToRGB
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_RGB_SPOT52PNG_700_700_1000_1000_channels_1_2_3.png
         700 700 1000 1000
         1 2 3 )

ADD_TEST(tvIoExtractROI_Scalar_SPOT52PNG1 ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_1.png
         700 700 1000 1000
         1 )
ADD_TEST(tvIoExtractROI_Scalar_SPOT52PNG2 ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_2.png
         700 700 1000 1000
         2 )
ADD_TEST(tvIoExtractROI_Scalar_SPOT52PNG3 ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_3.png
         700 700 1000 1000
         3 )
ADD_TEST(tvIoExtractROI_Scalar_SPOT52PNG4 ${IO_TESTS} 
#  --compare-image   ${BASELINE}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
#                    ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
         otbExtractROI_VectorToScalar
         ${INPUTDATA}/SPOT5
         ${TEMP}/IoExtractROI_Scalar_SPOT52PNG_700_700_1000_1000_channel_4.png
         700 700 1000 1000
         4 )



# -------            otb::ImageFileWriter pour CAI  ------------------------------
# Niveau de Gris

ADD_TEST(tvIoImageFileWriterPNG2PNG ${IO_TESTS} 
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/IoImageFileWriterPNG2PNG_cthead1.png )

ADD_TEST(tvIoImageFileWriterPNG2BSQ ${IO_TESTS} 
        otbImageFileWriterTest
        ${INPUTDATA}/cthead1.png 
        ${TEMP}/IoImageFileWriterPNG2BSQ_cthead1.hd )

# RGB
ADD_TEST(tvIoImageFileWriterRGB_PNG2PNG ${IO_TESTS} 
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileWriterRGB_PNG2PNG_sbuv.png )

ADD_TEST(tvIoImageFileWriterRGB_PNG2BSQ ${IO_TESTS} 
        otbImageFileWriterRGBTest
        ${INPUTDATA}/sbuv.png 
        ${TEMP}/IoImageFileWriterRGB_PNG2BSQ_sbuv.hd )




#ADD_TEST(tvIoImageFileWriterPNG2PNG ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.png )

#ADD_TEST(tvIoImageFileWriterPNG2BSQ ${IO_TESTS} otbImageFileWriterTest
#        ${INPUTDATA}/cthead1.png 
#        ${TEMP}/otb_writer_cthead1.hd )


#ADD_TEST(tvIoImageFileWriterRGBPNG2BSQ ${IO_TESTS} otbImageFileWriterRGBTest
#        ${INPUTDATA}/sbuv.png 
#        ${TEMP}/otb_writer_RGB_poupees.hd )

SET(BasicIO_SRCS
otbCAIImageIOTestCanRead.cxx
otbCAIImageIOTestCanWrite.cxx
otbGDALImageIOTest.cxx
otbImageFileReaderTest.cxx
otbImageFileWriterTest.cxx
#otbImageIOFactoryTest.cxx
otbImageFileReaderRGBTest.cxx
otbImageFileWriterRGBTest.cxx
otbExtractROI_VectorToRGB.cxx
otbExtractROI_VectorToScalar.cxx
)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")
#LINK_DIRECTORIES( $ENV{CAI_DIR_LIB} )


ADD_EXECUTABLE(otbIOTests otbIOTests.cxx ${BasicIO_SRCS})
# CAI ${TIFF_LIBRARY} ${CAI_LIBRARY} ${GDAL_LIBRARY}
TARGET_LINK_LIBRARIES(otbIOTests OTBIO OTBCommon ${CAI_LIBRARY} ${GDAL_LIBRARY} ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




