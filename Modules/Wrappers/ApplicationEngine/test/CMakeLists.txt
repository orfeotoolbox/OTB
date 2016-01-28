otb_module_test()

set(OTBApplicationEngineTests
otbApplicationEngineTestDriver.cxx
otbWrapperApplicationTest.cxx
otbWrapperParameterTest.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperStringParameterTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
otbWrapperStringListParameterTest.cxx
otbWrapperRAMParameterTest.cxx
otbWrapperDocExampleStructureTest.cxx
otbWrapperParameterKeyTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperInputImageListParameterTest.cxx
otbWrapperInputVectorDataListParameterTest.cxx
otbWrapperApplicationDocTests.cxx
otbWrapperApplicationHtmlDocGeneratorTest.cxx
otbWrapperInputVectorDataParameterTest.cxx
otbWrapperOutputImageParameterTest.cxx
)

add_executable(otbApplicationEngineTestDriver ${OTBApplicationEngineTests})
target_link_libraries(otbApplicationEngineTestDriver ${OTBApplicationEngine-Test_LIBRARIES})
otb_module_target_label(otbApplicationEngineTestDriver)

# Tests Declaration

otb_add_test(NAME owTuApplication COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationNew
  )

otb_add_test(NAME owTvParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterTest1
  "param1"
  )

otb_add_test(NAME owTuParameterNew COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterNew
  )

otb_add_test(NAME owTvInputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageParameterTest1
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuInputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageParameterNew
  )

otb_add_test(NAME owTuNumericalParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperNumericalParameterNew
  )

otb_add_test(NAME owTvNumericalParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperNumericalParameterTest1
  42.42
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuStringParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringParameterNew
  )

otb_add_test(NAME owTvStringParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringParameterTest1
  "value1"
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvApplicationRegistry COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationRegistry
  )

otb_add_test(NAME owTuStringListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringListParameterNew
  )

otb_add_test(NAME owTvStringListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperStringListParameterTest1
  "value1"
  "value2"
  "value3"
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvParameterRAM COMMAND otbApplicationEngineTestDriver
  otbWrapperRAMParameterNew
  )

otb_add_test(NAME owTvParameterKey COMMAND otbApplicationEngineTestDriver
  otbWrapperParameterKey
  )

otb_add_test(NAME owTuEmptyParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperEmptyParameterNew
  )

otb_add_test(NAME owTuInputImageListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageListParameterNew
  )

otb_add_test(NAME owTvInputImageListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputImageListParameterTest1
  ${INPUTDATA}/poupees_c1.hdr
  ${INPUTDATA}/poupees.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTuInputVectorDataListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataListParameterNew
  )

otb_add_test(NAME owTvInputVectorDataListParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataListParameterTest1
  ${INPUTDATA}/ToulousePoints-examples.shp
  ${INPUTDATA}/ToulouseRoads-examples.shp
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvApplicationHtmlDocGeneratorTest
  COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTvOReadImageInfoDoc.html
  ${TEMP}/owTvOReadImageInfoDoc.html
  otbWrapperApplicationHtmlDocGeneratorTest1
  ReadImageInfo
  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
  ${TEMP}/owTvOReadImageInfoDoc.html
  0 # don't show key in the parameterdescription
  )

otb_add_test(NAME owTvApplicationHtmlDocGeneratorNew COMMAND otbApplicationEngineTestDriver
  otbWrapperApplicationHtmlDocGeneratorNew
  )

otb_add_test(NAME owTuInputVectorDataParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperInputVectorDataParameterNew
  )

otb_add_test(NAME owTuOutputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterNew
  )

otb_add_test(NAME owTvOutputImageParameter COMMAND otbApplicationEngineTestDriver
  otbWrapperOutputImageParameterTest1
  ${INPUTDATA}/poupees.tif
  ${TEMP}/owTvOutputImageParameter2.tif
  "mykey"
  "my description"
  )

otb_add_test(NAME owTvDocExampleStructureTest COMMAND otbApplicationEngineTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE}/owTuDocExampleStructureTest.txt
  ${TEMP}/owTuDocExampleStructureTest.txt
  otbWrapperDocExampleStructureTest
  ${TEMP}/owTuDocExampleStructureTest.txt
  )

otb_add_test(NAME owTuDocExampleStructureNew COMMAND otbApplicationEngineTestDriver
  otbWrapperDocExampleStructureNew
  )

