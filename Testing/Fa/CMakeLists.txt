# $Id$

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Fa)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)
SET(EXAMPLES_DATA ${OTB_SOURCE_DIR}/Examples/Data)
#Tolerance sur diff pixel image
SET(TOL 0.0)

# !!!!!  Exe generated in the Common dir
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests3)
SET(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
SET(IO_TESTS     ${CXX_TEST_PATH}/otbIOTests9)
SET(IO_TESTS2     ${CXX_TEST_PATH}/otbIOTests8)
SET(VISU_TESTS   ${CXX_TEST_PATH}/otbVisuTests1)

# -------       FAs traitees -----------------------------------

# ---  FA 00005 ENVI (GDAL) image multi-canal ---
ADD_TEST(FA-00005-io_EnviMultiBande_RW ${IO_TESTS} 
         otbVectorImageFileReaderWriterTest
         ${INPUTDATA}/GomaBestScale_w15_50.raw.hdr
         ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15.hdr
         )

ADD_TEST(FA-00005-io_EnviMultiBande ${COMMON_TESTS2} 
#   --compare-image ${TOL}  ${BASELINE}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
#                     ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
         otbMultiChannelExtractROI
         -double ${INPUTDATA}/GomaBestScale_w15_50.raw.hdr
         -double ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
         -startX 100 -startY 150 -sizeX 150 -sizeY 200
         -channels 1 2 3)

# ---  FA 00006 ENVI (GDAL) Conversion Type de pixel ---

ADD_TEST(FA-00006-io_Amst_ers1_Double2UChar ${COMMON_TESTS} 
#     --compare-image ${TOL}  ${INPUTDATA}/amst_ers1.ima.hdr
#                       ${TEMP}/FA-00006-io_amst_ers1_double2uchar.hdr
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/amst_ers1.ima.hdr 
         -uchar ${TEMP}/FA-00006-io_amst_ers1_double2uchar.hdr )
ADD_TEST(FA-00006-io_Amst_ers1_UChar2UChar ${COMMON_TESTS} 
#     --compare-image ${TOL}   ${INPUTDATA}/amst_ers1.ima.hdr
#                       ${TEMP}/FA-00006-io_amst_ers1_uchar2uchar.hdr
         otbMultiToMonoChannelExtractROI
         -uchar ${INPUTDATA}/amst_ers1.ima.hdr 
         -uchar ${TEMP}/FA-00006-io_amst_ers1_uchar2uchar.hdr )

ADD_TEST(FA-00006-io_Amst_ers1_Double2Double ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/amst_ers1.ima.hdr
         -double ${TEMP}/FA-00006-io_amst_ers1_double2double.hdr )

ADD_TEST(FA-00006-io_Amst_ers1_UChar2Double ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -uchar ${INPUTDATA}/amst_ers1.ima.hdr
         -double ${TEMP}/FA-00006-io_amst_ers1_uchar2double.hdr )


ADD_TEST(FA-00006-io_Poupees_OCT_UChar2Double ${COMMON_TESTS} 
#   --compare-image ${TOL}   ${BASELINE}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
#                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         -uchar ${INPUTDATA}/poupees_1canal.c1.hdr
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2Double ${COMMON_TESTS} 
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.hdr
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/poupees_1canal.c1.hdr 
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.hdr )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2UChar ${COMMON_TESTS} 
#     --compare-image ${TOL} ${INPUTDATA}/poupees_1canal.c1.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_oct.hdr
         otbMultiToMonoChannelExtractROI
         -double ${INPUTDATA}/poupees_1canal.c1.hdr 
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct.hdr )

ADD_TEST(FA-00006-io_Poupees_R8_Double2UCharBis ${COMMON_TESTS} 
#     --compare-image ${TOL}  ${INPUTDATA}/poupees_1canal.c1.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.hdr )

# ------   RADARSAT   ------------
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(FA-00006-io_Radarsat_UI2_UShort2UShort ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -ushort ${IMAGEDATA}/RADARSAT1/SCENE01/
         -ushort ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_ui2.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Int2Int ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -int ${IMAGEDATA}/RADARSAT1/SCENE01/
         -int ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Double2Double_ ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         -double ${IMAGEDATA}/RADARSAT1/SCENE01/
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_I4_Int2Double ${COMMON_TESTS} 
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         otbMultiToMonoChannelExtractROI
         -int    ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         )

ADD_TEST(FA-00006-io_Radarsat_I4_Double2Double ${COMMON_TESTS} 
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         )

ADD_TEST(FA-00006-io_Radarsat_R8_Double2Double ${COMMON_TESTS} 
#    --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         )

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(FA-00012-co_AlignementsQB ${CXX_TEST_PATH}/AlignementsQB
        ${INPUTDATA}/poupees.hdr 
        ${TEMP}/FA-00012-co_AlignementsQB.png 
        )


# ---  FA 00013 : impossible to write a TIFF(CAI) -> GDAL  ---
ADD_TEST(FA-00013-io_WriteImageTIFF ${IO_TESTS2} 
        otbImageFileWriterTest
        ${INPUTDATA}/sbuv_NB_c1.png 
        ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1.tif )


ADD_TEST(FA-00013-io_WriteImageTIFF_bis ${COMMON_TESTS} 
         otbMultiToMonoChannelExtractROI
         ${INPUTDATA}/sbuv_NB_c1.png
         ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1_extrait.tif
         -startX 0 -startY 0 -sizeX 60 -sizeY 488 )

# ---  FA 00052 : Floating point exception lecture ROI_QB_MUL_1.png  ---
ADD_TEST(FA-00052-vi_Floating_Point_Exception ${VISU_TESTS}
	otbImageViewer
	${EXAMPLES_DATA}/ROI_QB_MUL_1.png
	3
         )

# ---  FA 00060 : Pb Map activation  ---
ADD_TEST(FA-00060-le_Map_Activation ${CXX_TEST_PATH}/MapActivation
	${INPUTDATA}/list.mhd
        4
        4
        20
        1.0
        0.1
        128
        1
        ${TEMP}/FA-00013-le-som.hdr
        ${TEMP}/FA-00013-le-actMap.png
        ${TEMP}/FA-00013-le-som.hdr
	)

# -------       Fin des FAs traitees -----------------------------------

ADD_TEST(FA-00070-vi_Streaming_Stat ${CXX_TEST_PATH}/StreamingStat
	${EXAMPLES_DATA}/qb_RoadExtract.tif
         )

ADD_TEST(FA-00070-vi_Streaming_Stat_bis ${CXX_TEST_PATH}/StreamingStat
	${EXAMPLES_DATA}/amst.png
         )

ADD_EXECUTABLE(AlignementsQB  AlignementsQB.cxx)
TARGET_LINK_LIBRARIES(AlignementsQB OTBFeatureExtraction OTBIO OTBCommon gdal ITKIO ITKCommon)

ADD_EXECUTABLE(MapActivation  MapActivation.cxx)
TARGET_LINK_LIBRARIES(MapActivation OTBFeatureExtraction OTBIO OTBCommon gdal ITKIO ITKCommon)

ADD_EXECUTABLE(StreamingStat  StreamingStat.cxx)
TARGET_LINK_LIBRARIES(StreamingStat OTBFeatureExtraction OTBIO OTBCommon gdal ITKIO ITKCommon)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
