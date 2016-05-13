otb_module_test()

set(OTBMPITiffWriterTests
otbMPITiffWriterTestDriver.cxx
otbSPTWStripsWriteTest.cxx
)

add_executable(otbMPITiffWriterTestDriver ${OTBMPITiffWriterTests})

# target link libraries
target_link_libraries(otbMPITiffWriterTestDriver ${OTBMPITiffWriter-Test_LIBRARIES})


otb_module_target_label(otbMPITiffWriterTestDriver)

# Tests Declaration

otb_add_test(NAME cdTvSPTWWriteStrips COMMAND otbMPITiffWriterTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  otbSPTWStripsWriteTest
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  )

