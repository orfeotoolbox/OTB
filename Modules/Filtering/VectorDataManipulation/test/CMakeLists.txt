otb_module_test()

set(OTBVectorDataManipulationTests
otbVectorDataManipulationTestDriver.cxx
otbDBOverlapDataNodeFeatureFunction.cxx
otbVectorDataExtractROI.cxx
otbVectorDataToSpecificDescriptionFilterBase.cxx
otbVectorDataToVectorDataFilterNew.cxx
otbPolygonCompacityFunctor.cxx
otbVectorDataToRandomLineGenerator.cxx
otbConcatenateVectorDataFilter.cxx
otbVectorDataExtractROINew.cxx
otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction.cxx
)

add_executable(otbVectorDataManipulationTestDriver ${OTBVectorDataManipulationTests})
target_link_libraries(otbVectorDataManipulationTestDriver ${OTBVectorDataManipulation-Test_LIBRARIES})
otb_module_target_label(otbVectorDataManipulationTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvDBOverlapDataNodeFeatureFunction COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
  ${TEMP}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
  otbDBOverlapDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${TEMP}/bfTvDBOverlapDataNodeFeatureFunctionOutput.shp
  0
  )

otb_add_test(NAME bfTuDBOverlapDataNodeFeatureFunctionNew COMMAND otbVectorDataManipulationTestDriver
  otbDBOverlapDataNodeFeatureFunctionNew
  )

otb_add_test(NAME coTvVectorDataExtractROI COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/coVectorDataExtractROIOutput.shp
  ${TEMP}/coVectorDataExtractROIOutput.shp
  otbVectorDataExtractROI
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/coVectorDataExtractROIOutput.shp
  374369.48850211215904 4853951.786124914  # Origin of the RemoteSensingRegion
  1000.25 -25000.2                               # Size of the Cartoregion
  )

otb_add_test(NAME coTvVectorDataExtractROI2 COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/coVectorDataExtractROIOutput2.shp
  ${TEMP}/coVectorDataExtractROIOutput2.shp
  otbVectorDataExtractROI
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${TEMP}/coVectorDataExtractROIOutput2.shp
  374369.48850211215904 4828951.58612491376698  # Origin of the RemoteSensingRegion
  1000.25 25000.2                               # Size of the Cartoregion
  )

otb_add_test(NAME fzTuVectorDataToSpecificDescriptionFilterBaseNew COMMAND otbVectorDataManipulationTestDriver
  otbVectorDataToSpecificDescriptionFilterBaseNew)

otb_add_test(NAME coTuVectorDataToVectorDataFilterNew COMMAND otbVectorDataManipulationTestDriver
  otbVectorDataToVectorDataFilterNew)

otb_add_test(NAME bfTvPolygonCompacityFunctor COMMAND otbVectorDataManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfPolygonCompacityFunctorTest.txt
  ${TEMP}/bfPolygonCompacityFunctorTest.txt
  otbPolygonCompacityFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfPolygonCompacityFunctorTest.txt
  )

otb_add_test(NAME bfTuVectorDataToRandomLineGenerator COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  otbVectorDataToRandomLineGenerator
  ${INPUTDATA}/ObjectReco/Boats/maur_B010202_01Polygons.shp
  ${TEMP}/bfTvVectorDataToRandomLineGeneratorOutput.shp
  )

otb_add_test(NAME bfTuVectorDataToRandomLineGeneratorNew COMMAND otbVectorDataManipulationTestDriver
  otbVectorDataToRandomLineGeneratorNew
  )

otb_add_test(NAME bfTuConcatenateVectorDataFilterNew COMMAND otbVectorDataManipulationTestDriver
  otbConcatenateVectorDataFilterNew
  )

otb_add_test(NAME bfTvConcatenateVectorDataFilter COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/otbConcatenateVectorDataFilter_Output.shp
  ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
  otbConcatenateVectorDataFilter
  -in  ${INPUTDATA}/france_coastline.shp
  ${INPUTDATA}/waterways.shp
  -out ${TEMP}/otbConcatenateVectorDataFilter_Output.shp
  )

otb_add_test(NAME coTuVectorDataExtractROINew COMMAND otbVectorDataManipulationTestDriver
  otbVectorDataExtractROINew)

otb_add_test(NAME bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Polygon COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Polygon.shp
  0 )
otb_add_test(NAME bfTuRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew COMMAND otbVectorDataManipulationTestDriver
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionNew
  )
otb_add_test(NAME bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction_Line COMMAND otbVectorDataManipulationTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  otbRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvRadiometryHomogenousWithNeighborhoodDataNodeFeatureFunctionOutput_Line.shp
  0 )
