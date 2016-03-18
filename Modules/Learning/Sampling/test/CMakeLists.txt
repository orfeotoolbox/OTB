otb_module_test()

set(OTBSamplingTests
otbSamplingTestDriver.cxx
otbOGRDataToClassStatisticsFilterTest.cxx
otbImageSampleExtractorFilterTest.cxx
)

add_executable(otbSamplingTestDriver ${OTBSamplingTests})
target_link_libraries(otbSamplingTestDriver ${OTBSampling-Test_LIBRARIES})
otb_module_target_label(otbSamplingTestDriver)

# Tests Declaration

otb_add_test(NAME leTuOGRDataToClassStatisticsFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToClassStatisticsFilterNew )

otb_add_test(NAME leTvOGRDataToClassStatisticsFilter COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvOGRDataToClassStatisticsFilterOutput.txt
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt
  otbOGRDataToClassStatisticsFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt)

otb_add_test(NAME leTuImageSampleExtractorFilterNew COMMAND otbSamplingTestDriver
  otbImageSampleExtractorFilterNew )
