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
SET(EXAMPLES_DATA ${OTB_DATA_ROOT}/Examples)
#Tolerance sur diff pixel image
SET(TOL 0.0)

# !!!!!  Exe generated in the Common dir
SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests3)
SET(COMMON_TESTS2 ${CXX_TEST_PATH}/otbCommonTests2)
SET(IO_TESTS     ${CXX_TEST_PATH}/otbIOTests9)
SET(IO_TESTS2     ${CXX_TEST_PATH}/otbIOTests7)
SET(VISU_TESTS   ${CXX_TEST_PATH}/otbVisuTests1)

# -------       FAs fixed -----------------------------------

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
SET_TESTS_PROPERTIES(FA-00006-io_Poupees_R8_Double2UCharBis PROPERTIES DEPENDS FA-00006-io_Poupees_OCT_UChar2Double)


# ------   RADARSAT   ------------
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(FA-00006-io_Radarsat_UI2_UShort2UShort ${COMMON_TESTS}
         otbMultiToMonoChannelExtractROI
         -ushort ${IMAGEDATA}/RADARSAT1/GOMA/SCENE01/
         -ushort ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_ui2.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Int2Int ${COMMON_TESTS}
         otbMultiToMonoChannelExtractROI
         -int ${IMAGEDATA}/RADARSAT1/GOMA/SCENE01/
         -int ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Double2Double ${COMMON_TESTS}
         otbMultiToMonoChannelExtractROI
         -double ${IMAGEDATA}/RADARSAT1/GOMA/SCENE01/
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_I4_Int2Double ${COMMON_TESTS}
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         otbMultiToMonoChannelExtractROI
         -int    ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_I4_Int2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Int2Int)

ADD_TEST(FA-00006-io_Radarsat_I4_Double2Double ${COMMON_TESTS}
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_I4_Double2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Int2Int)

ADD_TEST(FA-00006-io_Radarsat_R8_Double2Double ${COMMON_TESTS}
#    --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_R8_Double2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Double2Double)

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

IF (OTB_USE_DEPRECATED)
# ---  FA 00052 : Floating point exception lecture ROI_QB_MUL_1.png  ---
ADD_TEST(FA-00052-vi_Floating_Point_Exception ${VISU_TESTS}
        otbImageViewer
        ${EXAMPLES_DATA}/ROI_QB_MUL_1.png
        3
         )
ENDIF (OTB_USE_DEPRECATED)

# ---  FA 00060 : Pb Map activation  ---
ADD_TEST(FA-00060-le_Map_Activation ${CXX_TEST_PATH}/MapActivation
        ${INPUTDATA}/list.tif
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

# ---  FA 000295 : Pb Mean Shift clustering module  ---
ADD_TEST(000295-MeanShiftVectorImageFilterWithStreamingShrink ${CXX_TEST_PATH}/000295-MeanShiftVectorImageFilterWithStreamingShrink
        ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
        20 50 500 10
        )

# --- FA 0000428 : Pb with the otb::StreamingImageFileWriter when the buffered region is bigger than the requested region
ADD_TEST(FA-0000428-CastImageFilterStreaming ${CXX_TEST_PATH}/0000428-CastImageFilterStreaming
#  --compare-image ${TOL} ${INPUTDATA}/scene_8.tif
#                        ${TEMP}/FA-0000428-scene_8.tif
#  Execute 0000428-CastImageFilterStreaming                
    ${INPUTDATA}/scene_8.tif
    ${TEMP}/FA-0000428-scene_8.tif)

# -------       End of fixed FAs -----------------------------------

ADD_TEST(FA-00070-vi_Streaming_Stat ${CXX_TEST_PATH}/StreamingStat
        ${EXAMPLES_DATA}/qb_RoadExtract.tif
         )

ADD_TEST(FA-00070-vi_Streaming_Stat_bis ${CXX_TEST_PATH}/StreamingStat
        ${EXAMPLES_DATA}/amst.png
         )


# -------       From Mantis tracker   -----------------------------------
ADD_TEST(FA-0000041-mean_shift1 ${CXX_TEST_PATH}/0000041-mean_shift
        ${INPUTDATA}/QB_Suburb.png
        12
        7
        100
        ${TEMP}/filtered_image1.tif
        ${TEMP}/segmented_image1.tif
        ${TEMP}/labelled_image1.tif
        ${TEMP}/boundary_of_labelled_image1.tif
        )

ADD_TEST(FA-0000041-mean_shift2 ${CXX_TEST_PATH}/0000041-mean_shift
        ${INPUTDATA}/QB_Suburb.png
        20
        7
        100
        ${TEMP}/filtered_image2.tif
        ${TEMP}/segmented_image2.tif
        ${TEMP}/labelled_image2.tif
        ${TEMP}/boundary_of_labelled_image2.tif
        )
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(FA-00134-feature_extraction ${CXX_TEST_PATH}/00134-feature_extraction
        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/extracted_filter_134.tif
        5
        500 # extract start index
        300 # extract size
        )

ADD_TEST(FA-Spot5SensorModel ${CXX_TEST_PATH}/SensorModelBorder
  ${IMAGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
  ${TEMP}/FA-Spot5SensorModel.txt
  )
ENDIF(OTB_DATA_USE_LARGEINPUT)

IF(OTB_USE_VISU_GUI)
ADD_TEST(FA-0000132-jpg ${CXX_TEST_PATH}/0000132-jpg
        ${INPUTDATA}/toulouse_auat.jpg
        )
ENDIF(OTB_USE_VISU_GUI)

IF(USE_FFTWD OR USE_FFTWF)
ADD_TEST(FA-0000169-fftcomplextocomplex ${CXX_TEST_PATH}/0000169-fftcomplextocomplex
  )
ENDIF(USE_FFTWD OR USE_FFTWF)

ADD_TEST(faTvWriteUnsignedLong ${CXX_TEST_PATH}/WriteUnsignedLong
${TEMP}/WriteUnsignedLong.tif
)

# ------- FA 0000209 : SVM Validation issue -------------------------
ADD_TEST(FA-000209-SVMValidationLinearlySeparableWithoutProbEstimate_OK ${CXX_TEST_PATH}/0000209-SVMValidationLinearlySeparableProbEstimation
0000209-SVMValidationLinearlySeparableProbEstimation 500 500 0.0025 0.0075 0.0075 0.0025 0. 0.0025 0. 0.0025 0 0)

# ------- FA 0000433 : LSD 8bots vs 16bits -------------------------
ADD_TEST(FA-0000433-LineSegmentDetector_8b_16b_compare ${CXX_TEST_PATH}/0000433-LineSegmentDetector_8b_16b_compare
#--compare-ogr  ${EPSILON_8}
#                 ${TEMP}/FA-0000433-LSD_8b.shp
#                 ${TEMP}/FA-0000433-LSD_16b.shp
0000433-LineSegmentDetector_8b_16b_compare 
  ${INPUTDATA}/scene_8.tif
  ${INPUTDATA}/scene_16.tif
  ${TEMP}/FA-0000433-LSD_8b.shp
  ${TEMP}/FA-0000433-LSD_16b.shp)

IF(BUILD_APPLICATIONS)
# ------- FA 0000436 : WrapperInputImage::GetImage -------------------------
ADD_TEST(FA-0000436-WrapperInputImage_GetImage ${CXX_TEST_PATH}/0000436-WrapperInputImage_GetImage
0000436-WrapperInputImage_GetImage
  ${INPUTDATA}/scene_8.tif
)
ENDIF(BUILD_APPLICATIONS)
# ------- FA 0000437-WriteImageCentOS -------------------------
ADD_TEST(FA-0000437-WriteImageCentOS ${CXX_TEST_PATH}/0000437-WriteImageCentOS
  ${BASELINE}/FA-0000437-WriteImageCentOSInput.tif
  ${TEMP}/FA-0000437-WriteImageCentOSOutput.tif)

# -------       Vectorization issue   -----------------------------------
# FIXME Desactivated until http://bugs.orfeo-toolbox.org/view.php?id=94
# has somebody working on it
# ADD_TEST(FA-0000094-fe-PolygonsVectorization ${CXX_TEST_PATH}/PolygonsVectorization
#         -in ${INPUTDATA}/objtest2.tif
#         -ot ${TEMP}/output.txt
#         -out ${TEMP}/objtest2-result.tif
#         )

ADD_EXECUTABLE(AlignementsQB  AlignementsQB.cxx)
TARGET_LINK_LIBRARIES(AlignementsQB OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(MapActivation  MapActivation.cxx)
TARGET_LINK_LIBRARIES(MapActivation OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(StreamingStat  StreamingStat.cxx)
TARGET_LINK_LIBRARIES(StreamingStat OTBFeatureExtraction OTBIO OTBCommon)

ADD_EXECUTABLE(0000041-mean_shift  0000041-mean_shift.cxx)
TARGET_LINK_LIBRARIES(0000041-mean_shift OTBIO OTBCommon OTBBasicFilters)

ADD_EXECUTABLE(00134-feature_extraction  00134-feature_extraction.cxx)
TARGET_LINK_LIBRARIES(00134-feature_extraction OTBIO OTBCommon OTBBasicFilters OTBFeatureExtraction)

IF(OTB_USE_VISU_GUI)
ADD_EXECUTABLE(0000132-jpg 0000132-jpg.cxx )
TARGET_LINK_LIBRARIES(0000132-jpg OTBIO OTBVisualization)
ENDIF(OTB_USE_VISU_GUI)

ADD_EXECUTABLE(PolygonsVectorization PolygonsVectorization.cxx)
TARGET_LINK_LIBRARIES(PolygonsVectorization OTBIO OTBCommon)

ADD_EXECUTABLE(0000169-fftcomplextocomplex  0000169-fftcomplextocomplex.cxx)
TARGET_LINK_LIBRARIES(0000169-fftcomplextocomplex OTBIO OTBCommon)

ADD_EXECUTABLE(000295-MeanShiftVectorImageFilterWithStreamingShrink 000295-MeanShiftVectorImageFilterWithStreamingShrink.cxx)
TARGET_LINK_LIBRARIES(000295-MeanShiftVectorImageFilterWithStreamingShrink OTBIO OTBCommon)

ADD_EXECUTABLE(SensorModelBorder SensorModelBorder.cxx)
TARGET_LINK_LIBRARIES(SensorModelBorder OTBIO OTBCommon OTBBasicFilters OTBProjections)

ADD_EXECUTABLE(WriteUnsignedLong WriteUnsignedLong.cxx)
TARGET_LINK_LIBRARIES(WriteUnsignedLong OTBIO OTBCommon)

ADD_EXECUTABLE(0000209-SVMValidationLinearlySeparableProbEstimation 0000209-SVMValidationLinearlySeparableProbEstimation.cxx)
TARGET_LINK_LIBRARIES(0000209-SVMValidationLinearlySeparableProbEstimation OTBIO OTBLearning)

ADD_EXECUTABLE(0000428-CastImageFilterStreaming  0000428-CastImageFilterStreaming.cxx)
TARGET_LINK_LIBRARIES(0000428-CastImageFilterStreaming OTBIO OTBCommon OTBBasicFilters)

ADD_EXECUTABLE(0000433-LineSegmentDetector_8b_16b_compare 0000433-LineSegmentDetector_8b_16b_compare.cxx)
TARGET_LINK_LIBRARIES(0000433-LineSegmentDetector_8b_16b_compare OTBIO OTBFeatureExtraction)

IF(BUILD_APPLICATIONS)
  ADD_EXECUTABLE(0000436-WrapperInputImage_GetImage 0000436-WrapperInputImage_GetImage.cxx)
  TARGET_LINK_LIBRARIES(0000436-WrapperInputImage_GetImage OTBApplicationEngine)
ENDIF(BUILD_APPLICATIONS)

ADD_EXECUTABLE(0000437-WriteImageCentOS 0000437-WriteImageCentOS.cxx)
TARGET_LINK_LIBRARIES(0000437-WriteImageCentOS OTBIO OTBCommon)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
