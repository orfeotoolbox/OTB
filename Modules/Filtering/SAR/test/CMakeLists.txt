otb_module_test()

set(OTBSARTests
  otbSARTestDriver.cxx
  otbSarDeburstFilterTest.cxx
  )

add_executable(otbSARTestDriver ${OTBSARTests})
target_link_libraries(otbSARTestDriver ${OTBSAR-Test_LIBRARIES})
otb_module_target_label(otbSARTestDriver)

otb_add_test(NAME saTvSarDeburstImageFilterTest COMMAND otbSARTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/saTvSarDeburstImageFilterTestOutput.tif
  ${TEMP}/saTvSarDeburstImageFilterTestOutput.tif
  otbSarDeburstFilterTest
  ${INPUTDATA}/s1a-iw1-slc-vh-amp_xt.tif
  ${TEMP}/saTvSarDeburstImageFilterTestOutput.tif)
