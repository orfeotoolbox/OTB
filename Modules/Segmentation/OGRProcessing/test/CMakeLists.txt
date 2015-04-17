otb_module_test()

set(OTBOGRProcessingTests
otbOGRProcessingTestDriver.cxx
otbOGRLayerStreamStitchingFilter.cxx
)

add_executable(otbOGRProcessingTestDriver ${OTBOGRProcessingTests})
target_link_libraries(otbOGRProcessingTestDriver ${OTBOGRProcessing-Test_LIBRARIES})
otb_module_target_label(otbOGRProcessingTestDriver)

# Tests Declaration

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

