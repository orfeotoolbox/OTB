otb_module_test()

set(OTBMPIImageIOTests
otbMPIImageIOTestDriver.cxx
otbSPTWStripsWriteTest.cxx
)

add_executable(otbMPIImageIOTestDriver ${OTBMPIImageIOTests})
target_link_libraries(otbMPIImageIOTestDriver ${OTBMPIImageIO-Test_LIBRARIES})
otb_module_target_label(otbMPIImageIOTestDriver)

# Tests Declaration

otb_add_test(NAME cdTvSPTWWriteStrips COMMAND otbMPIImageIOTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  otbSPTWStripsWriteTest
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  )

