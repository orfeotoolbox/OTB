otb_module_test()

set(OTBWatershedsTests
otbWatershedsTestDriver.cxx
otbWatershedSegmentationFilter.cxx
)

add_executable(otbWatershedsTestDriver ${OTBWatershedsTests})
target_link_libraries(otbWatershedsTestDriver ${OTBWatersheds-Test_LIBRARIES})
otb_module_target_label(otbWatershedsTestDriver)

# Tests Declaration

otb_add_test(NAME obTuWatershedSegmentationFilter COMMAND otbWatershedsTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/obTuWatershedSegmentationFilterLabelImage.tif
  ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
  otbWatershedSegmentationFilter
  ${EXAMPLEDATA}/ROI_QB_PAN_1.tif
  ${TEMP}/obTuWatershedSegmentationFilterLabelImage.tif
  0.01
  0.2
  )

