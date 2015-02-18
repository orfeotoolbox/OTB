otb_module_test()

set(OTBCurlAdaptersTests
otbCurlAdaptersTestDriver.cxx
otbIsNightlyRevision.cxx
)

add_executable(otbCurlAdaptersTestDriver ${OTBCurlAdaptersTests})
target_link_libraries(otbCurlAdaptersTestDriver ${OTBCurlAdapters-Test_LIBRARIES})
otb_module_target_label(otbCurlAdaptersTestDriver)

# Tests Declaration

otb_add_test(NAME tsTvDataIsNightlyRevision COMMAND otbCurlAdaptersTestDriver
  otbIsNightlyRevision
  ${OTB_DATA_WC_REVISION}
  http://hg.orfeo-toolbox.org/nightly/dataNightlyNumber )

otb_add_test(NAME tsTvIsNightlyRevision COMMAND otbCurlAdaptersTestDriver
  otbIsNightlyRevision
  ${OTB_WC_REVISION}
  http://hg.orfeo-toolbox.org/nightly/libNightlyNumber )

