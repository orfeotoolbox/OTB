otb_module_test()

set(OTBOGRProcessingTests
otbOGRProcessingTestDriver.cxx
otbOGRLayerStreamStitchingFilter.cxx
)

if(OTB_USE_EDISON)
  list(APPEND OTBOGRProcessingTests otbStreamingImageToOGRLayerSegmentationFilter.cxx)
endif()

add_executable(otbOGRProcessingTestDriver ${OTBOGRProcessingTests})
target_link_libraries(otbOGRProcessingTestDriver ${OTBOGRProcessing-Test_LIBRARIES})
otb_module_target_label(otbOGRProcessingTestDriver)

# Tests Declaration
if(OTB_USE_EDISON)
otb_add_test(NAME obTvStreamingImageToOGRLayerSegmentationFilter COMMAND otbOGRProcessingTestDriver
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

otb_add_test(NAME obTuStreamingImageToOGRLayerSegmentationFilterNew COMMAND otbOGRProcessingTestDriver
  otbStreamingImageToOGRLayerSegmentationFilterNew)

otb_add_test(NAME obTvStreamingImageToOGRLayerSegmentationFilterHalfSpacing COMMAND otbOGRProcessingTestDriver
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

otb_add_test(NAME obTvStreamingImageToOGRLayerSegmentationFilterQBOrthoWGS84shp COMMAND otbOGRProcessingTestDriver
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
endif()

otb_add_test(NAME obTuOGRLayerStreamStitchingFilter COMMAND otbOGRProcessingTestDriver
  --compare-ogr  ${EPSILON_8}
  ${BASELINE_FILES}/obTvFusionOGRTile.shp
  ${TEMP}/obTvFusionOGRTile.shp
  otbOGRLayerStreamStitchingFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/QB_Toulouse_Ortho_withTiles.shp
  ${TEMP}/obTvFusionOGRTile.shp
  112
  )

