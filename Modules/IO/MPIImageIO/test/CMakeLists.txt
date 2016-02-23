otb_module_test()

set(OTBMPIMappersTests
otbMPIMappersTestDriver.cxx
otbSPTWStripsWriteTest.cxx
)

add_executable(otbMPIMappersTestDriver ${OTBMPIMappersTests})
target_link_libraries(otbMPIMappersTestDriver ${OTBMPIMappers-Test_LIBRARIES})
otb_module_target_label(otbMPIMappersTestDriver)

# Tests Declaration

otb_add_test(NAME cdTvSPTWWriteStrips COMMAND otbMPIMappersTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  otbSPTWStripsWriteTest
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  )

