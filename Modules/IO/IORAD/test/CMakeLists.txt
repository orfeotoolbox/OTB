otb_module_test()

set(OTBIORADTests
otbIORADTestDriver.cxx
otbRADImageIOTestCanRead.cxx
)

add_executable(otbIORADTestDriver ${OTBIORADTests})
target_link_libraries(otbIORADTestDriver ${OTBIORAD-Test_LIBRARIES})
otb_module_target_label(otbIORADTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuRADImageIOCanRead_RADCR4sngl COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  ${INPUTDATA}/RADCR4_image.rad )

otb_add_test(NAME ioTuRADImageIOCanRead_RADCR4mult COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  ${INPUTDATA}/RADCR4_multimage.rad )

otb_add_test(NAME ioTuRADImageIOCanRead_RADCI2 COMMAND otbIORADTestDriver
  otbRADImageIOTestCanRead
  LARGEINPUT{RAD_CI2/3017_HH.rad})

