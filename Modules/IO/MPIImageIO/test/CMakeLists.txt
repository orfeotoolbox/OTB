otb_module_test()

set(OTBMPIImageIOTests
otbMPIImageIOTestDriver.cxx
otbSPTWStripsWriteTest.cxx
)

add_executable(otbMPIImageIOTestDriver ${OTBMPIImageIOTests})

# This does seems right : those libs are in TEST DEPENDS
# but the build fail because of undefined references :(
target_link_libraries(otbMPIImageIOTestDriver
  ${OTBImageIO_LIBRARIES}
  ${OTBTestKernel_LIBRARIES}
)

#
otb_module_target_label(otbMPIImageIOTestDriver)

# Tests Declaration

otb_add_test(NAME cdTvSPTWWriteStrips COMMAND otbMPIImageIOTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  otbSPTWStripsWriteTest
  ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
  ${TEMP}/cdSPTWWritedInStripsImage.tif
  )

