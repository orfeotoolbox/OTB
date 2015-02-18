otb_module_test()

set(OTBPointSetTests
otbPointSetTestDriver.cxx
otbPointSetSourceTest.cxx
otbPointSetExtractROITest.cxx
otbSimplePointCountStrategyTest.cxx
otbThresholdImageToPointSetFilterNew.cxx
otbRandomPointSetSourceTest.cxx
otbImageToPointSetFilterTest.cxx
otbTransformPointSetFilterTest.cxx
otbThresholdImageToPointSetTest.cxx
)

add_executable(otbPointSetTestDriver ${OTBPointSetTests})
target_link_libraries(otbPointSetTestDriver ${OTBPointSet-Test_LIBRARIES})
otb_module_target_label(otbPointSetTestDriver)

# Tests Declaration

otb_add_test(NAME coTuPointSetSource COMMAND otbPointSetTestDriver
  otbPointSetSourceTest)

otb_add_test(NAME bfTvPointSetExtractROI COMMAND otbPointSetTestDriver
  otbPointSetExtractROITest)

otb_add_test(NAME feTvSimplePointCountStrategyTest COMMAND otbPointSetTestDriver
  otbSimplePointCountStrategyTest)

otb_add_test(NAME feTuThresholdImageToPointSetFilterNew COMMAND otbPointSetTestDriver
  otbThresholdImageToPointSetFilterNew
  )

otb_add_test(NAME bfTvRandomPointSetSource COMMAND otbPointSetTestDriver
  otbRandomPointSetSourceTest)

otb_add_test(NAME bfTuImageToPointSetFilterTest COMMAND otbPointSetTestDriver
  otbImageToPointSetFilterTest)

otb_add_test(NAME bfTvTransformPointSetFilter COMMAND otbPointSetTestDriver
  otbTransformPointSetFilterTest)

otb_add_test(NAME feTuThresholdImageToPointSet1 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To255.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To255.txt
  0 255 )

otb_add_test(NAME feTuThresholdImageToPointSet2 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_0To0.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_0To0.txt
  0 0 )

otb_add_test(NAME feTuThresholdImageToPointSet3 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_255To255.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_255To255.txt
  255 255 )

otb_add_test(NAME feTuThresholdImageToPointSet4 COMMAND otbPointSetTestDriver
  --compare-ascii ${EPSILON_3} ${BASELINE_FILES}/feThresholdImageToPointSet_Threshold_1To200.txt
  ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
  otbThresholdImageToPointSetTest
  ${INPUTDATA}/small_points.hdr
  ${TEMP}/feThresholdImageToPointSet_Threshold_1To200.txt
  1 200 )

