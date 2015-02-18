otb_module_test()

set(OTBMathParserTests
otbMathParserTestDriver.cxx
otbParserXTest.cxx
otbMaskMuParserFilterNew.cxx
otbMaskMuParserFilterTest.cxx
otbParserConditionDataNodeFeatureFunction.cxx
otbParserTest.cxx
otbImageListToSingleImageFilterTest.cxx
otbBandMathXImageFilter.cxx
otbBandMathImageFilter.cxx
)

add_executable(otbMathParserTestDriver ${OTBMathParserTests})
target_link_libraries(otbMathParserTestDriver ${OTBMathParser-Test_LIBRARIES})
otb_module_target_label(otbMathParserTestDriver)

# Tests Declaration

otb_add_test(NAME coTvParserX COMMAND otbMathParserTestDriver
  otbParserXTest
  )

otb_add_test(NAME coTuParserX COMMAND otbMathParserTestDriver
  otbParserXTestNew
  )

otb_add_test(NAME bfTuMaskMuParserFilterNew COMMAND otbMathParserTestDriver
  otbMaskMuParserFilterNew
  )

otb_add_test(NAME bfTvMaskMuParserFilterTest COMMAND otbMathParserTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvMaskMuParserFilterTest.tif
  ${TEMP}/bfTvMaskMuParserFilterTest.tif
  otbMaskMuParserFilterTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/bfTvMaskMuParserFilterTest.tif
  "(b1>100)*(b2>120)"
  )

otb_add_test(NAME bfTvParserConditionDataNodeFeatureFunction_Line_NDVI COMMAND otbMathParserTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Line_NDVI.shp
  otbParserConditionDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Line_NDVI.shp
  "ndvi(b3,b4) > 0.047"
  0
  )

otb_add_test(NAME bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI COMMAND otbMathParserTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI.shp
  otbParserConditionDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvParserConditionDataNodeFeatureFunction_Polygon_NDVI.shp
  "ndvi(b3,b4) > 0.047"
  0
  )

otb_add_test(NAME coTvParser COMMAND otbMathParserTestDriver
  otbParserTest
  )

otb_add_test(NAME coTuParser COMMAND otbMathParserTestDriver
  otbParserTestNew
  )

otb_add_test(NAME bfTuImageListToSingleImageFilterNew COMMAND otbMathParserTestDriver
  otbImageListToSingleImageFilterNew)

otb_add_test(NAME bfTvImageListToSingleImageFilter COMMAND otbMathParserTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvImageListToSingleImageFilter.tif
  ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
  otbImageListToSingleImageFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvImageListToSingleImageFilterOut.tif
  )

otb_add_test(NAME bfTuBandMathXImageFilterNew COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterNew)

otb_add_test(NAME bfTvBandMathXImageFilterConv COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterConv
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )

otb_add_test(NAME bfTvBandMathXImageFilter COMMAND otbMathParserTestDriver
  otbBandMathXImageFilter)

otb_add_test(NAME bfTvBandMathXImageFilterWithIdx COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
  )

otb_add_test(NAME bfTvBandMathXImageFilterTxt COMMAND otbMathParserTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  otbBandMathXImageFilterTxt
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )

otb_add_test(NAME bfTvBandMathImageFilterWithIdx COMMAND otbMathParserTestDriver
  otbBandMathImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
  )

otb_add_test(NAME bfTuBandMathImageFilter COMMAND otbMathParserTestDriver
  otbBandMathImageFilterNew)

otb_add_test(NAME bfTvBandMathImageFilter COMMAND otbMathParserTestDriver
  otbBandMathImageFilter)

