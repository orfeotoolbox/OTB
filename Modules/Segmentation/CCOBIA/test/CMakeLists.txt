otb_module_test()

set(OTBCCOBIATests
otbCCOBIATestDriver.cxx
otbStreamingConnectedComponentOBIATest.cxx
otbConnectedComponentMuParserFunctorTest.cxx
otbMeanShiftStreamingConnectedComponentOBIATest.cxx
otbLabelObjectOpeningMuParserFilterNew.cxx
otbLabelObjectOpeningMuParserFilterTest.cxx
)

add_executable(otbCCOBIATestDriver ${OTBCCOBIATests})
target_link_libraries(otbCCOBIATestDriver ${OTBCCOBIA-Test_LIBRARIES})
otb_module_target_label(otbCCOBIATestDriver)

# Tests Declaration

otb_add_test(NAME obTuStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew COMMAND otbCCOBIATestDriver
  otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilterNew)

otb_add_test(NAME obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter COMMAND otbCCOBIATestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
  ${TEMP}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
  otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTvStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
  "((b1>80) * intensity>95)"
  "distance<40"
  15
  "SHAPE_Elongation>8"
  5 )

otb_add_test(NAME bfTvConnectedComponentMuParserFunctorTest COMMAND otbCCOBIATestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_1.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
  otbConnectedComponentMuParserFunctorTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_1.tif
  "distance<40"
  )

otb_add_test(NAME bfTvConnectedComponentMuParserFunctorTestMask COMMAND otbCCOBIATestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvConnectedComponentMuParserFunctorTest_2.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
  otbConnectedComponentMuParserFunctorTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/bfTvConnectedComponentMuParserFunctorTest_2.tif
  "distance<40"
  ${INPUTDATA}/ROI_QB_MUL_4_Mask.tif
  )

otb_add_test(NAME obTuMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter COMMAND otbCCOBIATestDriver
  otbMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.shp
  9
  10
  0.001
  "distance<10"
  20
  "SHAPE_Elongation>10"
  5)

otb_add_test(NAME bfTuLabelObjectOpeningMuParserFilterNew COMMAND otbCCOBIATestDriver
  otbLabelObjectOpeningMuParserFilterNew
  )

otb_add_test(NAME bfTvLabelObjectOpeningMuParserFilterTest COMMAND otbCCOBIATestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/bfTvLabelObjectOpeningMuParserFilterTest.tif
  ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
  otbLabelObjectOpeningMuParserFilterTest
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${INPUTDATA}/ROI_QB_MUL_4_Label.tif
  ${TEMP}/bfTvLabelObjectOpeningMuParserFilterTest.tif
  "SHAPE_Elongation>8"
  )

