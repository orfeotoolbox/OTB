otb_module_test()

set(OTBLabellingTests
otbLabellingTestDriver.cxx
otbLabelizeConfidenceConnectedImageFilter.cxx
otbLabelizeConnectedThresholdImageFilter.cxx
otbLabelizeConnectedThresholdImageFilterNew.cxx
otbLabelizeNeighborhoodConnectedImageFilterNew.cxx
otbLabelizeNeighborhoodConnectedImageFilter.cxx
otbLabelizeConfidenceConnectedImageFilterNew.cxx
)

add_executable(otbLabellingTestDriver ${OTBLabellingTests})
target_link_libraries(otbLabellingTestDriver ${OTBLabelling-Test_LIBRARIES})
otb_module_target_label(otbLabellingTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvLabelizeConfidenceConnectedImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  otbLabelizeConfidenceConnectedImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeConfidenceConnectedImageFilterOutput.png
  127 127 2 3 1
  )

otb_add_test(NAME bfTvLabelizeConnectedThresholdImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  otbLabelizeConnectedThresholdImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeConnectedThresholdImageFilterOutput.png
  170 170
  80 80
  )

otb_add_test(NAME bfTuLabelizeConnectedThresholdImageFilterNew COMMAND otbLabellingTestDriver
  otbLabelizeConnectedThresholdImageFilterNew)

otb_add_test(NAME bfTuLabelizeNeighborhoodConnectedImageFilterNew COMMAND otbLabellingTestDriver
  otbLabelizeNeighborhoodConnectedImageFilterNew)

otb_add_test(NAME bfTvLabelizeNeighborhoodConnectedImageFilter COMMAND otbLabellingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  otbLabelizeNeighborhoodConnectedImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfTvLabelizeNeighborhoodConnectedImageFilterOutput.png
  170 170
  80 80
  )

otb_add_test(NAME bfTuLabelizeConfidenceConnectedImageFilterNew COMMAND otbLabellingTestDriver
  otbLabelizeConfidenceConnectedImageFilterNew)

