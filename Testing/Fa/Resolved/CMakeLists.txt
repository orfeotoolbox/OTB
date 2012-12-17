SET(TOL 0.0)

# ---  FA 00005 ENVI (GDAL) image multi-canal ---
ADD_TEST(FA-00005-io_EnviMultiBande_RW ${CXX_TEST_PATH}/otbIOTests9
         otbVectorImageFileReaderWriterTest
         ${OTB_DATA_ROOT}/Input/GomaBestScale_w15_50.raw.hdr
         ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15.hdr
         )

ADD_TEST(FA-00005-io_EnviMultiBande ${CXX_TEST_PATH}/otbCommonTests2
#   --compare-image ${TOL}  ${OTB_DATA_ROOT}/Baseline/OTB/Fa/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
#                     ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
         otbMultiChannelExtractROI
         -double ${OTB_DATA_ROOT}/Input/GomaBestScale_w15_50.raw.hdr
         -double ${TEMP}/FA-00005-io_EnviMultiBande_GomaBestScale_w15_50_100_120_150_200.hdr
         -startX 100 -startY 150 -sizeX 150 -sizeY 200
         -channels 1 2 3)

# ---  FA 00006 ENVI (GDAL) Conversion Type de pixel ---

ADD_TEST(FA-00006-io_Amst_ers1_Double2UChar ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}  ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
#                       ${TEMP}/FA-00006-io_amst_ers1_double2uchar.hdr
         otbMultiToMonoChannelExtractROI
         -double ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
         -uchar ${TEMP}/FA-00006-io_amst_ers1_double2uchar.hdr )
ADD_TEST(FA-00006-io_Amst_ers1_UChar2UChar ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}   ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
#                       ${TEMP}/FA-00006-io_amst_ers1_uchar2uchar.hdr
         otbMultiToMonoChannelExtractROI
         -uchar ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
         -uchar ${TEMP}/FA-00006-io_amst_ers1_uchar2uchar.hdr )

ADD_TEST(FA-00006-io_Amst_ers1_Double2Double ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         -double ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
         -double ${TEMP}/FA-00006-io_amst_ers1_double2double.hdr )

ADD_TEST(FA-00006-io_Amst_ers1_UChar2Double ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         -uchar ${OTB_DATA_ROOT}/Input/amst_ers1.ima.hdr
         -double ${TEMP}/FA-00006-io_amst_ers1_uchar2double.hdr )


ADD_TEST(FA-00006-io_Poupees_OCT_UChar2Double ${CXX_TEST_PATH}/otbCommonTests3
#   --compare-image ${TOL}   ${OTB_DATA_ROOT}/Baseline/OTB/Fa/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
#                     ${TEMP}/ioMultiMonoChannelROI_ENVI2PNG_poupees_1canal_302_2_134_330.png
         otbMultiToMonoChannelExtractROI
         -uchar ${OTB_DATA_ROOT}/Input/poupees_1canal.c1.hdr
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2Double ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.hdr
         otbMultiToMonoChannelExtractROI
         -double ${OTB_DATA_ROOT}/Input/poupees_1canal.c1.hdr
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8_bis.hdr )

ADD_TEST(FA-00006-io_Poupees_OCT_Double2UChar ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL} ${OTB_DATA_ROOT}/Input/poupees_1canal.c1.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_oct.hdr
         otbMultiToMonoChannelExtractROI
         -double ${OTB_DATA_ROOT}/Input/poupees_1canal.c1.hdr
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct.hdr )

ADD_TEST(FA-00006-io_Poupees_R8_Double2UCharBis ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}  ${OTB_DATA_ROOT}/Input/poupees_1canal.c1.hdr
#                       ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Poupees_1canal_r8.hdr
         -uchar ${TEMP}/FA-00006-io_Poupees_1canal_oct_bis.hdr )
SET_TESTS_PROPERTIES(FA-00006-io_Poupees_R8_Double2UCharBis PROPERTIES DEPENDS FA-00006-io_Poupees_OCT_UChar2Double)


# ------   RADARSAT   ------------
IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(FA-00006-io_Radarsat_UI2_UShort2UShort ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         -ushort ${OTB_DATA_LARGEINPUT_ROOT}/RADARSAT1/GOMA/SCENE01/
         -ushort ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_ui2.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Int2Int ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         -int ${OTB_DATA_LARGEINPUT_ROOT}/RADARSAT1/GOMA/SCENE01/
         -int ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_UI2_Double2Double ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         -double ${OTB_DATA_LARGEINPUT_ROOT}/RADARSAT1/GOMA/SCENE01/
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -startX 2559 -startY 3591 -sizeX 337 -sizeY 280 )

ADD_TEST(FA-00006-io_Radarsat_I4_Int2Double ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         otbMultiToMonoChannelExtractROI
         -int    ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_bis.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_I4_Int2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Int2Int)

ADD_TEST(FA-00006-io_Radarsat_I4_Double2Double ${CXX_TEST_PATH}/otbCommonTests3
#     --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_i4.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_ter.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_I4_Double2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Int2Int)

ADD_TEST(FA-00006-io_Radarsat_R8_Double2Double ${CXX_TEST_PATH}/otbCommonTests3
#    --compare-image ${TOL}  ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
#                       ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         otbMultiToMonoChannelExtractROI
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8.hdr
         -double ${TEMP}/FA-00006-io_Radarsat_2559_3591_337_280_r8_4.hdr
         )
SET_TESTS_PROPERTIES(FA-00006-io_Radarsat_R8_Double2Double PROPERTIES DEPENDS FA-00006-io_Radarsat_UI2_Double2Double)

ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(FA-00012-co_AlignementsQB ${CXX_TEST_PATH}/AlignementsQB
        ${OTB_DATA_ROOT}/Input/poupees.hdr
        ${TEMP}/FA-00012-co_AlignementsQB.png
        )


# ---  FA 00013 : impossible to write a TIFF(CAI) -> GDAL  ---
ADD_TEST(FA-00013-io_WriteImageTIFF ${CXX_TEST_PATH}/otbIOTests7
        otbImageFileWriterTest
        ${OTB_DATA_ROOT}/Input/sbuv_NB_c1.png
        ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1.tif )


ADD_TEST(FA-00013-io_WriteImageTIFF_bis ${CXX_TEST_PATH}/otbCommonTests3
         otbMultiToMonoChannelExtractROI
         ${OTB_DATA_ROOT}/Input/sbuv_NB_c1.png
         ${TEMP}/FA-00013-io_WriteImageTIFF_sbuv_NB_c1_extrait.tif
         -startX 0 -startY 0 -sizeX 60 -sizeY 488 )

IF (OTB_USE_DEPRECATED)
# ---  FA 00052 : Floating point exception lecture ROI_QB_MUL_1.png  ---
ADD_TEST(FA-00052-vi_Floating_Point_Exception ${CXX_TEST_PATH}/otbVisuTests1
        otbImageViewer
        ${EXAMPLES_DATA}/ROI_QB_MUL_1.png
        3
         )
ENDIF (OTB_USE_DEPRECATED)

# ---  FA 00060 : Pb Map activation  ---
ADD_TEST(FA-00060-le_Map_Activation ${CXX_TEST_PATH}/MapActivation
        ${OTB_DATA_ROOT}/Input/list.tif
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
        ${OTB_DATA_ROOT}/Input/qb_RoadExtract2sub200x200.tif
        20 50 500 10
        )

# --- FA 0000428 : Pb with the otb::StreamingImageFileWriter when the buffered region is bigger than the requested region
ADD_TEST(FA-0000428-CastImageFilterStreaming ${CXX_TEST_PATH}/0000428-CastImageFilterStreaming
#  --compare-image ${TOL} ${OTB_DATA_ROOT}/Input/scene_8.tif
#                        ${TEMP}/FA-0000428-scene_8.tif
#  Execute 0000428-CastImageFilterStreaming                
    ${OTB_DATA_ROOT}/Input/scene_8.tif
    ${TEMP}/FA-0000428-scene_8.tif)

# -------       End of fixed FAs -----------------------------------

ADD_TEST(FA-00070-vi_Streaming_Stat ${CXX_TEST_PATH}/StreamingStat
         ${OTB_DATA_ROOT}/Examples/qb_RoadExtract.tif
         )

ADD_TEST(FA-00070-vi_Streaming_Stat_bis ${CXX_TEST_PATH}/StreamingStat
         ${OTB_DATA_ROOT}/Examples/amst.png
         )


# -------       From Mantis tracker   -----------------------------------
ADD_TEST(FA-0000041-mean_shift1 ${CXX_TEST_PATH}/0000041-mean_shift
        ${OTB_DATA_ROOT}/Input/QB_Suburb.png
        12
        7
        100
        ${TEMP}/filtered_image1.tif
        ${TEMP}/segmented_image1.tif
        ${TEMP}/labelled_image1.tif
        ${TEMP}/boundary_of_labelled_image1.tif
        )

ADD_TEST(FA-0000041-mean_shift2 ${CXX_TEST_PATH}/0000041-mean_shift
        ${OTB_DATA_ROOT}/Input/QB_Suburb.png
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
        ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
        ${TEMP}/extracted_filter_134.tif
        5
        500 # extract start index
        300 # extract size
        )

ADD_TEST(FA-Spot5SensorModel ${CXX_TEST_PATH}/SensorModelBorder
  ${OTB_DATA_LARGEINPUT_ROOT}/SPOT5/TEHERAN/IMAGERY.TIF
  ${TEMP}/FA-Spot5SensorModel.txt
  )
ENDIF(OTB_DATA_USE_LARGEINPUT)

IF(OTB_USE_VISU_GUI)
ADD_TEST(FA-0000132-jpg ${CXX_TEST_PATH}/0000132-jpg
        ${OTB_DATA_ROOT}/Input/toulouse_auat.jpg
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
  ${OTB_DATA_ROOT}/Input/scene_8.tif
  ${OTB_DATA_ROOT}/Input/scene_16.tif
  ${TEMP}/FA-0000433-LSD_8b.shp
  ${TEMP}/FA-0000433-LSD_16b.shp)

IF(BUILD_APPLICATIONS)
# ------- FA 0000436 : WrapperInputImage::GetImage -------------------------
ADD_TEST(FA-0000436-WrapperInputImage_GetImage ${CXX_TEST_PATH}/0000436-WrapperInputImage_GetImage
0000436-WrapperInputImage_GetImage
  ${OTB_DATA_ROOT}/Input/scene_8.tif
)
ENDIF(BUILD_APPLICATIONS)
# ------- FA 0000437-WriteImageCentOS -------------------------
ADD_TEST(FA-0000437-WriteImageCentOS ${CXX_TEST_PATH}/0000437-WriteImageCentOS
  ${OTB_DATA_ROOT}/Baseline/OTB/Fa/FA-0000437-WriteImageCentOSInput.tif
  ${TEMP}/FA-0000437-WriteImageCentOSOutput.tif)
  
IF(BUILD_APPLICATIONS)
ADD_TEST(NAME FA-000478-UncaughtException
         COMMAND  ${CXX_TEST_PATH}/0000478-UncaughtException
                  $<TARGET_FILE_DIR:otbapp_StereoSensorModelToElevationMap>
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_left.tif 
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_right.tif 
                  ${TEMP}/FA-0000478-output.tif)
ENDIF(BUILD_APPLICATIONS)

ADD_TEST(NAME FA-0000495-StreamingImageFileWriterProgressReporting
         COMMAND  ${CXX_TEST_PATH}/0000495-StreamingImageFileWriterProgressReporting
                  ${OTB_DATA_ROOT}/Examples/sensor_stereo_left.tif
                  ${TEMP}/FA-0000495-output1.tif
                  ${TEMP}/FA-0000495-output2.tif)

ADD_TEST(NAME otbVectorDataToLabelImageFilterWithoutReader
  COMMAND ${CXX_TEST_PATH}/otbVectorDataToLabelImageFilterWithoutReader
  ${TEMP}/rasterizedVectorData.tif)
  
