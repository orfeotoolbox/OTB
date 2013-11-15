if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

# Common generic tests
set(SEGMENTATION_TESTS1 ${CXX_TEST_PATH}/otbSegmentationTests1)

endif( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# -------  otb::HooverMatrix &  otb::HooverInstance -------------

add_test(obTuHooverMatrixFilterNew ${SEGMENTATION_TESTS1}
    otbHooverMatrixFilterNew)

add_test(obTvHooverMatrixFilter ${SEGMENTATION_TESTS1}
    --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
    ${TEMP}/obTvHooverMatrixFilter.txt
    otbHooverMatrixFilter
    ${INPUTDATA}/Seg1InputForRCC8Graph.tif
    ${INPUTDATA}/Seg2InputForRCC8Graph.tif
    ${TEMP}/obTvHooverMatrixFilter.txt
    )

add_test(obTuHooverInstanceFilterNew ${SEGMENTATION_TESTS1}
    otbHooverInstanceFilterNew)

add_test(obTvHooverInstanceFilterToAttributeImage ${SEGMENTATION_TESTS1}
    --compare-image ${EPSILON_9}
    ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    otbHooverInstanceFilterToAttributeImage
    ${INPUTDATA}/maur_GT.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    )

add_test(obTuWatershedSegmentationFilter ${SEGMENTATION_TESTS1}
      --compare-image ${NOTOL}
      ${BASELINE}/obTuWatershedSegmentationFilterLabelImage.tif
      ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
      otbWatershedSegmentationFilter
      ${EXAMPLEDATA}/ROI_QB_PAN_1.tif
      ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
      0.01
      0.2
      )

add_test(obTuMeanShiftSegmentationFilter ${SEGMENTATION_TESTS1}
      otbMeanShiftSegmentationFilter
      ${INPUTDATA}/ROI_QB_MUL_4.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterLabelImage.tif
      ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImage.tif
      4
      25
      100
      0.1
      )

add_test(obTvMorphologicalProfilesSegmentationFilter ${SEGMENTATION_TESTS1}
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


add_test(obTuMeanShiftSegmentationFilterPruning ${SEGMENTATION_TESTS1}
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
add_test(obTuStreamingImageToOGRLayerSegmentationFilterNew ${SEGMENTATION_TESTS1}
    otbStreamingImageToOGRLayerSegmentationFilterNew)

add_test(obTvStreamingImageToOGRLayerSegmentationFilter ${SEGMENTATION_TESTS1}
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

add_test(obTvStreamingImageToOGRLayerSegmentationFilterQBOrthoWGS84shp ${SEGMENTATION_TESTS1}
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


add_test(obTvStreamingImageToOGRLayerSegmentationFilterHalfSpacing ${SEGMENTATION_TESTS1}
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
add_test(obTuOGRLayerStreamStitchingFilter ${SEGMENTATION_TESTS1}
    --compare-ogr  ${EPSILON_8}
    ${BASELINE_FILES}/obTvFusionOGRTile.shp
    ${TEMP}/obTvFusionOGRTile.shp
    otbOGRLayerStreamStitchingFilter
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${INPUTDATA}/QB_Toulouse_Ortho_withTiles.shp
    ${TEMP}/obTvFusionOGRTile.shp
    112
    )


set(Segmentation_SRCS1
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

add_executable(otbSegmentationTests1 otbSegmentationTests1.cxx ${Segmentation_SRCS1})
target_link_libraries(otbSegmentationTests1 OTBIO OTBSegmentation  OTBTesting OTBOGRAdapters)

