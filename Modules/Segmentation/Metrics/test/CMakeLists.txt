otb_module_test()

set(OTBMetricsTests
otbMetricsTestDriver.cxx
otbHooverMatrixFilterNew.cxx
otbHooverInstanceFilterNew.cxx
otbHooverInstanceFilterToAttributeImage.cxx
otbHooverMatrixFilter.cxx
)

add_executable(otbMetricsTestDriver ${OTBMetricsTests})
target_link_libraries(otbMetricsTestDriver ${OTBMetrics-Test_LIBRARIES})
otb_module_target_label(otbMetricsTestDriver)

# Tests Declaration

otb_add_test(NAME obTuHooverMatrixFilterNew COMMAND otbMetricsTestDriver
  otbHooverMatrixFilterNew)

otb_add_test(NAME obTuHooverInstanceFilterNew COMMAND otbMetricsTestDriver
  otbHooverInstanceFilterNew)

otb_add_test(NAME obTvHooverInstanceFilterToAttributeImage COMMAND otbMetricsTestDriver
  --compare-image ${EPSILON_9}
  ${BASELINE}/obTvHooverInstanceFilterToAttributeImage.tif
  ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
  otbHooverInstanceFilterToAttributeImage
  ${INPUTDATA}/maur_GT.tif
  ${INPUTDATA}/maur_labelled.tif
  ${TEMP}/obTvHooverInstanceFilterToAttributeImage.tif
  )

otb_add_test(NAME obTvHooverMatrixFilter COMMAND otbMetricsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/obTvHooverMatrixFilter.txt
  ${TEMP}/obTvHooverMatrixFilter.txt
  otbHooverMatrixFilter
  ${INPUTDATA}/Seg1InputForRCC8Graph.tif
  ${INPUTDATA}/Seg2InputForRCC8Graph.tif
  ${TEMP}/obTvHooverMatrixFilter.txt
  )

