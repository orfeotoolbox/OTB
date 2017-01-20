otb_module_test()

set(OTBSARTests
  otbSARTestDriver.cxx
  otbSarDeburstFilterTest.cxx
  )

add_executable(otbSARTestDriver ${OTBSARTests})
target_link_libraries(otbSARTestDriver ${OTBSAR-Test_LIBRARIES})
otb_module_target_label(otbSARTestDriver)
