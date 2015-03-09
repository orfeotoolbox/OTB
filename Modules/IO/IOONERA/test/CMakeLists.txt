otb_module_test()

set(OTBIOONERATests
otbIOONERATestDriver.cxx
otbONERAImageIOTestCanRead.cxx
)

add_executable(otbIOONERATestDriver ${OTBIOONERATests})
target_link_libraries(otbIOONERATestDriver ${OTBIOONERA-Test_LIBRARIES})
otb_module_target_label(otbIOONERATestDriver)

# Tests Declaration

otb_add_test(NAME ioTuONERAImageIOCanRead COMMAND otbIOONERATestDriver otbONERAImageIOTestCanRead
  LARGEINPUT{ONERA/spa3_0215_rad.ent})

