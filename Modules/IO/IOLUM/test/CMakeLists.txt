otb_module_test()

set(OTBIOLUMTests
otbIOLUMTestDriver.cxx
otbLUMImageIOTestCanWrite.cxx
otbLUMImageIOTestCanRead.cxx
)

add_executable(otbIOLUMTestDriver ${OTBIOLUMTests})
target_link_libraries(otbIOLUMTestDriver ${OTBIOLUM-Test_LIBRARIES})
otb_module_target_label(otbIOLUMTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuLUMImageIOCanWrite COMMAND otbIOLUMTestDriver otbLUMImageIOTestCanWrite
  ${TEMP}/poupees.lum)

otb_add_test(NAME ioTuLUMImageIOCanRead COMMAND otbIOLUMTestDriver otbLUMImageIOTestCanRead
  ${INPUTDATA}/poupees.lum)

