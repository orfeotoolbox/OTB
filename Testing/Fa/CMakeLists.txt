# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Fa)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_DIR} )

#Tolerance sur diff pixel image
SET(TOL 0.0)

# !!!!!  Pointe sur un exe généré dans le repertoire Common
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)
SET(IO_TESTS     ${CXX_TEST_PATH}/otbIOTests)


# -------       FAs traitees -----------------------------------


# ---  FA 00005 ENVI (GDAL) image multi-canal ---
ADD_TEST(FA-00005-io_EnviMultiBande ${COMMON_TESTS} 
   --compare-binary  ${BASELINE}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.c1
                     ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.c1
         otbMultiChannelExtractROI
         -double ${INPUTDATA}/GomaBestScale_w15_50.raw
         -double ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hd
         -startX 100 -startY 150 -sizeX 150 -sizeY 200
         -channels 1 2 3)

ADD_TEST(FA-00005-io_EnviMultiBande_RW ${IO_TESTS} 
         otbVectorImageFileReaderWriterTest
         ${INPUTDATA}/GomaBestScale_w15_50.raw
         ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15.hd
         )



# ---  FA 00006 ENVI (GDAL) Conversion Type de pixel ---
ADD_TEST(FA-00006-io_Poupees_OCT_UChar2Double ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
#                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         -uchar ${INPUTDATA}/poupees_1canal.hd 
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hd )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2Double ${COMMON_TESTS} 
   --compare-binary  ${TEMP}/FA-00006-io_Poupees_1canal_r8.c1
                     ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.c1
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/poupees_1canal.hd 
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.hd )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2UChar ${COMMON_TESTS} 
   --compare-binary  ${INPUTDATA}/poupees_1canal.c1
                     ${TEMP}/FA-00006-io_Poupees_1canal_oct.c1
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/poupees_1canal.hd 
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct.hd )

ADD_TEST(FA-00006-io_Poupees_R8_Double2UCharBis ${COMMON_TESTS} 
   --compare-binary  ${INPUTDATA}/poupees_1canal.c1
                     ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.c1
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hd
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.hd )

# ------   RADASAT   ------------
ADD_TEST(FA-00006-io_Radarsat_UI2_UShort2UShort ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -ushort ${IMAGEDATA}/RADARSAT1/SCENE01/
         -ushort ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_ui2.hd
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Int2Int ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -int ${IMAGEDATA}/RADARSAT1/SCENE01/
         -int ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hd
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Double2Double_ ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -double ${IMAGEDATA}/RADARSAT1/SCENE01/
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hd
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )


ADD_TEST(FA-00006-io_Radarsat_I4_Int2Double ${COMMON_TESTS} 
   --compare-binary  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.c1
                     ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.c1
         otbMultiToMonoChannelExtractROI
         -int    ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hd
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hd
         )

ADD_TEST(FA-00006-io_Radarsat_I4_Double2Double ${COMMON_TESTS} 
   --compare-binary  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.c1
                     ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.c1
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hd
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hd
         )

ADD_TEST(FA-00006-io_Radarsat_R8_Double2Double ${COMMON_TESTS} 
   --compare-binary  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.c1
                     ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.c1
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hd
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hd
         )

ADD_TEST(FA-00012-co_AlignementsQB ${CXX_TEST_PATH}/AlignementsQB
        ${INPUTDATA}/poupees.hd 
        ${TEMP}/FA-00012-co_AlignementsQB.png 
        )


# ---  FA 00013 : impossible d'écrire une image TIFF(CAI)  ---
ADD_TEST(FA-00013-io_WriteImageTIFF ${IO_TESTS} 
        otbImageFileWriterTest
        ${INPUTDATA}/sbuv_NB_c1.png 
        ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1.tif )


ADD_TEST(FA-00013-io_WriteImageTIFF_bis ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/sbuv_NB_c1.png
         ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1_extrait.tif
         -startX 0 -startY 0 -sizeX 60 -sizeY 488 )






# -------       Fin des FAs traitees -----------------------------------


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ADD_EXECUTABLE(AlignementsQB  AlignementsQB.cxx)
TARGET_LINK_LIBRARIES(AlignementsQB OTBFeatureExtraction OTBIO OTBCommon ${CAI_LIBRARY}  ${GDAL_LIBRARY} ITKIO ITKCommon)




ENDIF( NOT OTB_DISABLE_CXX_TESTING )
