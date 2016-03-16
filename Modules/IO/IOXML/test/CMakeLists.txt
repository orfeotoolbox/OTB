otb_module_test()

set(OTBIOXMLTests
otbIOXMLTestDriver.cxx
otbStatisticsXMLFileWriteAndRead.cxx
)

add_executable(otbIOXMLTestDriver ${OTBIOXMLTests})
target_link_libraries(otbIOXMLTestDriver ${OTBIOXML-Test_LIBRARIES})
otb_module_target_label(otbIOXMLTestDriver)

# Tests Declaration

otb_add_test(NAME ioTvStatisticsXMLReaderWriter COMMAND otbIOXMLTestDriver
  --compare-ascii ${EPSILON_5}
  ${BASELINE_FILES}/ioTvStatisticsXMLReaderWriterOut3.xml
  ${TEMP}/ioTvStatisticsXMLReaderWriterOut3.xml
  otbStatisticsXMLFileWriteAndRead
  ${TEMP}/ioTvStatisticsXMLReaderWriterOut1.xml
  ${TEMP}/ioTvStatisticsXMLReaderWriterOut2.xml
  ${TEMP}/ioTvStatisticsXMLReaderWriterOut3.xml)
