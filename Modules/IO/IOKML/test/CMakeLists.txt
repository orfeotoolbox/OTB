otb_module_test()

set(OTBIOKMLTests
otbIOKMLTestDriver.cxx
otbKMLVectorDataIONew.cxx
otbKMLVectorDataIOTestFileReader.cxx
otbKMLVectorDataIOTestCanWrite.cxx
otbKMLVectorDataIOTestCanRead.cxx
)

add_executable(otbIOKMLTestDriver ${OTBIOKMLTests})
target_link_libraries(otbIOKMLTestDriver ${OTBIOKML-Test_LIBRARIES})
otb_module_target_label(otbIOKMLTestDriver)

# Tests Declaration

otb_add_test(NAME ioTuKMLVectorDataIO COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIONew )

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderExample COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/MODIS20040214212340.kml
  ${TEMP}/MODIS20040214212340.kml)

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderMultiGeom COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/multigeometry.kml
  ${TEMP}/multigeometry.kml)

otb_add_test(NAME ioTvKMLVectorDataIOFileReaderLines COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestFileReader
  ${INPUTDATA}/lines.kml
  ${TEMP}/lines.kml)

otb_add_test(NAME ioTuKMLVectorDataIOCanWrite COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestCanWrite
  ${TEMP}/MODIS20040214212340.kml)

otb_add_test(NAME ioTuKMLVectorDataIOCanRead COMMAND otbIOKMLTestDriver
  otbKMLVectorDataIOTestCanRead
  ${INPUTDATA}/MODIS20040214212340.kml)

