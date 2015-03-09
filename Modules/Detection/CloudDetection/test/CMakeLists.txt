otb_module_test()

set(OTBCloudDetectionTests
otbCloudDetectionTestDriver.cxx
otbCloudEstimatorDefaultFilter.cxx
otbCloudDetectionFilter.cxx
otbCloudEstimatorFilter.cxx
otbCloudEstimatorFilterNew.cxx
otbCloudDetectionFilterNew.cxx
)

add_executable(otbCloudDetectionTestDriver ${OTBCloudDetectionTests})
target_link_libraries(otbCloudDetectionTestDriver ${OTBCloudDetection-Test_LIBRARIES})
otb_module_target_label(otbCloudDetectionTestDriver)

# Tests Declaration

otb_add_test(NAME feTvCloudEstimatorDefaultFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvSpectralAngleOutput.tif
  ${TEMP}/feTvSpectralAngleOutput.tif
  otbCloudEstimatorDefaultFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvSpectralAngleOutput.tif
  500
  731
  500
  632
  )

otb_add_test(NAME feTvCloudDetectionFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvCloudDetectionOutput.tif
  ${TEMP}/feTvCloudDetectionOutput.tif
  otbCloudDetectionFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvCloudDetectionOutput.tif
  500
  731
  500
  632
  0.25   # variance
  0.95   # minthreshold
  1.0    # maxthreshold
  )

otb_add_test(NAME feTvCloudEstimatorFilter COMMAND otbCloudDetectionTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvCloudEstimatorOutput.tif
  ${TEMP}/feTvCloudEstimatorOutput.tif
  otbCloudEstimatorFilter
  ${INPUTDATA}/ExtrZoneNuageuse.tif
  ${TEMP}/feTvCloudEstimatorOutput.tif
  500
  731
  500
  632
  0.25    # variance
  )

otb_add_test(NAME feTuCloudEstimatorFilterNew COMMAND otbCloudDetectionTestDriver
  otbCloudEstimatorFilterNew)

otb_add_test(NAME feTuCloudDetectionFilterNew COMMAND otbCloudDetectionTestDriver
  otbCloudDetectionFilterNew)

