otb_module_test()

set(OTBIOBSQTests
otbIOBSQTestDriver.cxx
otbBSQImageIOTestCanRead.cxx
otbBSQImageIOTestCanWrite.cxx
)

add_executable(otbIOBSQTestDriver ${OTBIOBSQTests})
target_link_libraries(otbIOBSQTestDriver ${OTBIOBSQ-Test_LIBRARIES})
otb_module_target_label(otbIOBSQTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuBSQImageIOCanRead COMMAND otbIOBSQTestDriver otbBSQImageIOTestCanRead
  ${INPUTDATA}/poupees.hd)

otb_add_test(NAME ioTuBSQImageIOCanWrite COMMAND otbIOBSQTestDriver otbBSQImageIOTestCanWrite
  ${TEMP}/poupees.hd)

