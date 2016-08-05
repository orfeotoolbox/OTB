otb_module_test()

set(OTBCurlAdaptersTests
otbCurlAdaptersTestDriver.cxx
otbIsNightlyRevision.cxx
)

add_executable(otbCurlAdaptersTestDriver ${OTBCurlAdaptersTests})
target_link_libraries(otbCurlAdaptersTestDriver ${OTBCurlAdapters-Test_LIBRARIES})
otb_module_target_label(otbCurlAdaptersTestDriver)

# Is*Revision tests are removed. see JIRA OTB-1052
#the building of testdriver otbCurlAdaptersTestDriver is kept just
#for the sake of linking libcurl with OTB to ensure curl (superbuild/system)
#is fine for usage with OTB
