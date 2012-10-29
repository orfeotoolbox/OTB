IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

# Common generic tests
SET(SEGMENTATION_TESTS1 ${CXX_TEST_PATH}/otbSegmentationTests1)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# -------  otb::HooverMatrix &  otb::HooverInstance -------------

ADD_TEST(obTuHooverMatrixFilterNew ${SEGMENTATION_TESTS1}
    otbHooverMatrixFilterNew)

ADD_TEST(obTvHooverMatrixFilter ${SEGMENTATION_TESTS1}
    --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
    ${TEMP}/obTvHooverMatrixFilter.txt
    otbHooverMatrixFilter
    ${INPUTDATA}/Seg1InputForRCC8Graph.tif
    ${INPUTDATA}/Seg2InputForRCC8Graph.tif
    ${TEMP}/obTvHooverMatrixFilter.txt
    )

ADD_TEST(obTuHooverInstanceFilterNew ${SEGMENTATION_TESTS1}
    otbHooverInstanceFilterNew)

ADD_TEST(obTvHooverInstanceFilterToAttributeImage ${SEGMENTATION_TESTS1}
    --compare-image ${EPSILON_9}
    ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    otbHooverInstanceFilterToAttributeImage
    ${INPUTDATA}/maur_GT.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    )

ADD_TEST(obTuWatershedSegmentationFilter ${SEGMENTATION_TESTS1}
      --compare-image ${NOTOL}
      ${BASELINE}/obTuWatershedSegmentationFilterLabelImage.tif
      ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
      otbWatershedSegmentationFilter
      ${EXAMPLEDATA}/ROI_QB_PAN_1.tif
      ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
      0.01
      0.2
      )

ADD_TEST(obTuMeanShiftSegmentationFilter ${SEGMENTATION_TESTS1}
      otbMeanShiftSegmentationFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterLabelImage.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImage.tif
      4
      25
      100
      0.1
      )

ADD_TEST(obTvMorphologicalProfilesSegmentationFilter ${SEGMENTATION_TESTS1}
      --compare-image ${NOTOL}
      ${BASELINE}/obTvMorphologicalProfilesSegmentationFilter.tif
      ${TEMP}/obTvMorphologicalProfilesSegmentationFilter.tif
      otbMorphologicalProfilesSegmentationFilter
      ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
      ${TEMP}/obTvMorphologicalProfilesSegmentationFilter.tif
      1
      1
      5
      )


ADD_TEST(obTuMeanShiftSegmentationFilterPruning ${SEGMENTATION_TESTS1}
      otbMeanShiftSegmentationFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterLabelImagePruning.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImagePruning.tif
      4
      25
      100
      0.1
      20
      )

# -------            otb::StreamingVectorizedSegmentation   -------------
ADD_TEST(obTuStreamingImageToOGRLayerSegmentationFilterNew ${SEGMENTATION_TESTS1}
    otbStreamingImageToOGRLayerSegmentationFilterNew)

ADD_TEST(obTvStreamingImageToOGRLayerSegmentationFilter ${SEGMENTATION_TESTS1}
     --compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/obTvStreamingVectorizedSegmentationOutput.sqlite
     ${TEMP}/obTvStreamingVectorizedSegmentationOutput.sqlite
     otbStreamingImageToOGRLayerSegmentationFilter
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
     ${TEMP}/obTvStreamingVectorizedSegmentationOutput.sqlite
     NewLayer
     100
     5
     15.
     100
     1 #filter small object
     200 #minimum size of object
     0 #Simplify Flag
     0.2 #Simplification tolerance
     )

ADD_TEST(obTvStreamingImageToOGRLayerSegmentationFilterQBOrthoWGS84shp ${SEGMENTATION_TESTS1}
     --compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/obTvStreamingVectorizedSegmentationOutputQBOrthoWGS84.shp
     ${TEMP}/obTvStreamingVectorizedSegmentationOutputQBOrthoWGS84.shp
     otbStreamingImageToOGRLayerSegmentationFilter
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN_WGS84.tif
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask_WGS84.tif
     ${TEMP}/obTvStreamingVectorizedSegmentationOutputQBOrthoWGS84.shp
     NewLayer
     100
     5
     15.
     100
     1 #filter small object
     200 #minimum size of object
     0 #Simplify Flag
     0.2 #Simplification tolerance
     )


ADD_TEST(obTvStreamingImageToOGRLayerSegmentationFilterHalfSpacing ${SEGMENTATION_TESTS1}
     --compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/obTvStreamingVectorizedSegmentationHalfSpacingOutput.sqlite
     ${TEMP}/obTvStreamingVectorizedSegmentationHalfSpacingOutput.sqlite
     otbStreamingImageToOGRLayerSegmentationFilter
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN_HalfSpacing.tif
     ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask_HalfSpacing.tif
     ${TEMP}/obTvStreamingVectorizedSegmentationHalfSpacingOutput.sqlite
     NewLayer
     100
     5
     15.
     100
     1 #filter small object
     200 #minimum size of object
     0 #Simplify Flag
     0.2 #Simplification tolerance
     )



# -------            otb::OGRLayerStreamStitchingFilter   -------------
ADD_TEST(obTuOGRLayerStreamStitchingFilter ${SEGMENTATION_TESTS1}
    --compare-ogr  ${EPSILON_8}
    ${BASELINE_FILES}/obTvFusionOGRTile.shp
    ${TEMP}/obTvFusionOGRTile.shp
    otbOGRLayerStreamStitchingFilter
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${INPUTDATA}/QB_Toulouse_Ortho_withTiles.shp
    ${TEMP}/obTvFusionOGRTile.shp
    112
    )


SET(Segmentation_SRCS1
otbHooverMatrixFilterNew.cxx
otbHooverMatrixFilter.cxx
otbHooverInstanceFilterNew.cxx
otbHooverInstanceFilterToAttributeImage.cxx
otbWatershedSegmentationFilter.cxx
otbMeanShiftSegmentationFilter.cxx
otbMorphologicalProfilesSegmentationFilter.cxx
otbStreamingImageToOGRLayerSegmentationFilter.cxx
otbOGRLayerStreamStitchingFilter.cxx
)

ADD_EXECUTABLE(otbSegmentationTests1 otbSegmentationTests1.cxx ${Segmentation_SRCS1})
TARGET_LINK_LIBRARIES(otbSegmentationTests1 OTBIO OTBSegmentation  OTBTesting OTBOGRAdapters)

