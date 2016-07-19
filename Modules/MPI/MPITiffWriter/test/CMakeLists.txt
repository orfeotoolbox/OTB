otb_module_test()

set(OTBMPITiffWriterTests
otbMPITiffWriterTestDriver.cxx
otbMPISPTWReadWriteTest.cxx
)

add_executable(otbMPITiffWriterTestDriver ${OTBMPITiffWriterTests})

# target link libraries
target_link_libraries(otbMPITiffWriterTestDriver ${OTBMPITiffWriter-Test_LIBRARIES})


otb_module_target_label(otbMPITiffWriterTestDriver)

# Tests Declaration

otb_add_test_mpi(NAME otbMPISPTWReadWriteTest
  NBPROCS 2
  COMMAND otbMPITiffWriterTestDriver
  --compare-image ${NOTOL}
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/otbMPITiffWriterTestOutput.tif
  otbMPISPTWReadWriteTest
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/otbMPITiffWriterTestOutput.tif
  )

