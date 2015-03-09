otb_module_test()

set(OTBMeanShiftTests
otbMeanShiftTestDriver.cxx
otbMeanShiftConnectedComponentSegmentationFilterTest.cxx
otbMeanShiftSegmentationFilter.cxx
)

add_executable(otbMeanShiftTestDriver ${OTBMeanShiftTests})
target_link_libraries(otbMeanShiftTestDriver ${OTBMeanShift-Test_LIBRARIES})
otb_module_target_label(otbMeanShiftTestDriver)

# Tests Declaration

otb_add_test(NAME obTuMeanShiftConnectedComponentSegmentationFilter COMMAND otbMeanShiftTestDriver
  otbMeanShiftConnectedComponentSegmentationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftConnectedComponentSegmentationImage.png
  9
  9
  0.01
  "distance<0.2"
  50)

otb_add_test(NAME obTuMeanShiftSegmentationFilterPruning COMMAND otbMeanShiftTestDriver
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

otb_add_test(NAME obTuMeanShiftSegmentationFilter COMMAND otbMeanShiftTestDriver
  otbMeanShiftSegmentationFilter
  ${INPUTDATA}/ROI_QB_MUL_4.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterLabelImage.tif
  ${TEMP}/obTuMeanShiftSegmentationFilterClusteredImage.tif
  4
  25
  100
  0.1
  )

