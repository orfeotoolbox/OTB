otb_module_test()

set(OTBIOMWTests
otbIOMWTestDriver.cxx
otbMWImageIOTestCanRead.cxx
otbMWImageIOTestCanWrite.cxx
)

add_executable(otbIOMWTestDriver ${OTBIOMWTests})
target_link_libraries(otbIOMWTestDriver ${OTBIOMW-Test_LIBRARIES})
otb_module_target_label(otbIOMWTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuMWImageIOCanReadIMG COMMAND otbIOMWTestDriver otbMWImageIOTestCanRead
  ${BASELINE}/QB_Toulouse_Ortho_PAN.img)

otb_add_test(NAME ioTuMWImageIOCanReadMW COMMAND otbIOMWTestDriver otbMWImageIOTestCanRead
  ${BASELINE}/QB_Toulouse_Ortho_PAN.mw)

otb_add_test(NAME ioTuMWImageIOCanWriteIMG COMMAND otbIOMWTestDriver otbMWImageIOTestCanWrite
  ${TEMP}/QB_Toulouse_Ortho_PAN.img)
set_property(TEST ioTuMWImageIOCanWriteIMG PROPERTY WILL_FAIL true)

otb_add_test(NAME ioTuMWImageIOCanWriteMW COMMAND otbIOMWTestDriver otbMWImageIOTestCanWrite
  ${TEMP}/QB_Toulouse_Ortho_PAN.mw)

