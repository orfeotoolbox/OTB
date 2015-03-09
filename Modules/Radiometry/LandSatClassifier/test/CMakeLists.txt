otb_module_test()
set(OTBLandSatClassifierTests
  otbLandsatTMSpectralRuleBasedClassifierTest.cxx
  otbLandSatClassifierTestDriver.cxx  )

add_executable(otbLandSatClassifierTestDriver ${OTBLandSatClassifierTests})
target_link_libraries(otbLandSatClassifierTestDriver ${OTBLandSatClassifier-Test_LIBRARIES})
otb_module_target_label(otbLandSatClassifierTestDriver)
otb_add_test(NAME raTvLandsatTMSpectralRuleBasedClassifierTest COMMAND otbLandSatClassifierTestDriver
  otbLandsatTMSpectralRuleBasedClassifierTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMSpectralRuleBasedClassifierTest_Image.tif
  )
