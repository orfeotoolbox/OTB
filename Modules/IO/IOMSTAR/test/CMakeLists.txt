otb_module_test()

set(OTBIOMSTARTests
otbIOMSTARTestDriver.cxx
otbMSTARImageIOTestCanRead.cxx
)

add_executable(otbIOMSTARTestDriver ${OTBIOMSTARTests})
target_link_libraries(otbIOMSTARTestDriver ${OTBIOMSTAR-Test_LIBRARIES})
otb_module_target_label(otbIOMSTARTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuMSTARImageIOCanRead2 COMMAND otbIOMSTARTestDriver otbMSTARImageIOTestCanRead
  ${INPUTDATA}/MSTAR/HB20010.000)

otb_add_test(NAME ioTuMSTARImageIOCanRead1 COMMAND otbIOMSTARTestDriver otbMSTARImageIOTestCanRead
  ${INPUTDATA}/MSTAR/HB19896.016)

