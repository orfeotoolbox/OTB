IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

# Common generic tests
SET(SEGMENTATION_TESTS1 ${CXX_TEST_PATH}/otbSegmentationTests1)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


# -------  otb::HooverMatrix &  otb::HooverInstance -------------

ADD_TEST(obTuHooverMatrixFilterNew ${OBIA_TESTS1}
    otbHooverMatrixFilterNew)

ADD_TEST(obTvHooverMatrixFilter ${OBIA_TESTS1}
    --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
    ${TEMP}/obTvHooverMatrixFilter.txt
    otbHooverMatrixFilter
    ${INPUTDATA}/Seg1InputForRCC8Graph.tif
    ${INPUTDATA}/Seg2InputForRCC8Graph.tif
    ${TEMP}/obTvHooverMatrixFilter.txt
    )

ADD_TEST(obTuHooverInstanceFilterNew ${OBIA_TESTS1}
    otbHooverInstanceFilterNew)

ADD_TEST(obTvHooverInstanceFilterToAttributeImage ${OBIA_TESTS1}
    --compare-image ${EPSILON_9}
    ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    otbHooverInstanceFilterToAttributeImage
    ${INPUTDATA}/maur_GT.tif
    ${INPUTDATA}/maur_labelled.tif
    ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
    )

# -------            otb::StreamingVectorizedSegmentation   -------------
ADD_TEST(obTuStreamingImageToOGRDataSourceSegmentationFilterNew ${SEGMENTATION_TESTS1}
    otbStreamingImageToOGRDataSourceSegmentationFilterNew)

ADD_TEST(obTvStreamingImageToOGRDataSourceSegmentationFilter ${SEGMENTATION_TESTS1}
     --compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/obTvStreamingVectorizedSegmentationOutput.sqlite
     ${TEMP}/obTvStreamingVectorizedSegmentationOutput.sqlite
     otbStreamingImageToOGRDataSourceSegmentationFilter
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

ADD_TEST(obTvStreamingImageToOGRDataSourceSegmentationFilterHalfSpacing ${SEGMENTATION_TESTS1}
     --compare-ogr  ${EPSILON_8}
     ${BASELINE_FILES}/obTvStreamingVectorizedSegmentationHalfSpacingOutput.sqlite
     ${TEMP}/obTvStreamingVectorizedSegmentationHalfSpacingOutput.sqlite
     otbStreamingImageToOGRDataSourceSegmentationFilter
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



# -------            otb::OGRDataSourceStreamStitchingFilter   -------------
ADD_TEST(obTuOGRDataSourceStreamStitchingFilter ${SEGMENTATION_TESTS1}
    --compare-ogr  ${EPSILON_8}
    ${BASELINE_FILES}/obTvFusionOGRTile.shp
    ${TEMP}/obTvFusionOGRTile.shp
    otbOGRDataSourceStreamStitchingFilter
    ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
    ${INPUTDATA}/QB_Toulouse_Ortho_withTiles.shp
    ${TEMP}/obTvFusionOGRTile.shp
    layer
    112
    )


SET(Segmentation_SRCS1
otbHooverMatrixFilterNew.cxx
otbHooverMatrixFilter.cxx
otbHooverInstanceFilterNew.cxx
otbHooverInstanceFilterToAttributeImage.cxx
otbStreamingImageToOGRDataSourceSegmentationFilter.cxx
otbOGRDataSourceStreamStitchingFilter.cxx
)

ADD_EXECUTABLE(otbSegmentationTests1 otbSegmentationTests1.cxx ${Segmentation_SRCS1})
TARGET_LINK_LIBRARIES(otbSegmentationTests1 OTBIO OTBSegmentation  OTBTesting OTBOGRAdapters)

