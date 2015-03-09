otb_module_test()

set(OTBVectorDataIOTests
otbVectorDataIOTestDriver.cxx
otbVectorDataFileReaderWriter.cxx
otbVectorDataFileWriterPolygons.cxx
otbVectorDataFileReaderNew.cxx
otbVectorDataIOFactory.cxx
otbVectorDataFileWriterMultiPolygons.cxx
otbVectorDataFileReader.cxx
otbVectorDataFileGeoReaderWriter.cxx
otbVectorDataFileWriterNew.cxx
otbVectorDataFileWriter.cxx
)

add_executable(otbVectorDataIOTestDriver ${OTBVectorDataIOTests})
target_link_libraries(otbVectorDataIOTestDriver ${OTBVectorDataIO-Test_LIBRARIES})
otb_module_target_label(otbVectorDataIOTestDriver)

# Tests Declaration

otb_add_test(NAME ioTvVectorDataFileReaderWriter COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileReaderWriterOutput.shp
  ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
  otbVectorDataFileReaderWriter
  LARGEINPUT{QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp}
  ${TEMP}/ioTvVectorDataFileReaderWriterOutput.shp
  )

otb_add_test(NAME ioTvVectorDataFileWriterPolygons COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileWriterPolygonsOutput.shp
  ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
  otbVectorDataFileWriterPolygons
  ${TEMP}/ioTvVectorDataFileWriterPolygonsOutput.shp
  )

otb_add_test(NAME ioTuVectorDataFileReaderNew COMMAND otbVectorDataIOTestDriver
  otbVectorDataFileReaderNew )

otb_add_test(NAME ioTuVectorDataIOFactory COMMAND otbVectorDataIOTestDriver
  otbVectorDataIOFactory
  ${INPUTDATA}/ToulouseRoad-examples.shp )

otb_add_test(NAME ioTvVectorDataFileWriterMultiPolygons COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
  ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
  otbVectorDataFileWriterMultiPolygons
  ${TEMP}/ioTvVectorDataFileWriterMultiPolygonsOutput.shp
  )

otb_add_test(NAME ioTvSHPVectorDataFileReader COMMAND otbVectorDataIOTestDriver
  --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader.txt
  ${TEMP}/ioSHPVectorDataFileReader.txt
  otbVectorDataFileReader
  LARGEINPUT{QUICKBIRD/TOULOUSE/GIS_FILES/000000128955_01_ORDER_SHAPE.shp}
  ${TEMP}/ioSHPVectorDataFileReader.txt
  )

otb_add_test(NAME ioTvKMLVectorDataFileReader COMMAND otbVectorDataIOTestDriver
  --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader.txt
  ${TEMP}/ioTvKMLVectorDataFileReader.txt
  otbVectorDataFileReader
  ${INPUTDATA}/lines.kml
  ${TEMP}/ioTvKMLVectorDataFileReader.txt
  )

otb_add_test(NAME ioTvKMLVectorDataFileReader2 COMMAND otbVectorDataIOTestDriver
  --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioTvKMLVectorDataFileReader2.txt
  ${TEMP}/ioTvKMLVectorDataFileReader2.txt
  otbVectorDataFileReader
  ${INPUTDATA}/multigeometry.kml
  ${TEMP}/ioTvKMLVectorDataFileReader2.txt
  )

otb_add_test(NAME ioTvSHPVectorDataFileReader3 COMMAND otbVectorDataIOTestDriver
  --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader3.txt
  ${TEMP}/ioSHPVectorDataFileReader3.txt
  otbVectorDataFileReader
  LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
  ${TEMP}/ioSHPVectorDataFileReader3.txt
  )

otb_add_test(NAME ioTvSHPVectorDataFileReader2 COMMAND otbVectorDataIOTestDriver
  --compare-ascii ${EPSILON_9}  ${BASELINE_FILES}/ioSHPVectorDataFileReader2.txt
  ${TEMP}/ioSHPVectorDataFileReader2.txt
  otbVectorDataFileReader
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/ioSHPVectorDataFileReader2.txt
  )

otb_add_test(NAME ioTvKMLVectorDataFileGeoReaderWriter COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
  ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
  otbVectorDataFileGeoReaderWriter
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput.kml
  )

otb_add_test(NAME ioTvKMLVectorDataFileGeoReaderWriter2 COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
  ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
  otbVectorDataFileGeoReaderWriter
  ${INPUTDATA}/Capitole-Shadows.kml
  ${TEMP}/ioTvKMLVectorDataFileGeoReaderWriterOutput2.kml
  )

otb_add_test(NAME ioTvVectorDataFileGeoReaderWriter2 COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
  ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
  otbVectorDataFileGeoReaderWriter
  ${INPUTDATA}/Capitole-Shadows.sqlite
  ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput2.shp
  )

otb_add_test(NAME ioTvVectorDataFileGeoReaderWriter COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileGeoReaderWriterOutput.shp
  ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
  otbVectorDataFileGeoReaderWriter
  ${INPUTDATA}/ToulouseRoad-examples.shp
  ${TEMP}/ioTvVectorDataFileGeoReaderWriterOutput.shp
  )

otb_add_test(NAME ioTuVectorDataFileWriterNew COMMAND otbVectorDataIOTestDriver
  otbVectorDataFileWriterNew )

otb_add_test(NAME ioTvVectorDataFileWriter COMMAND otbVectorDataIOTestDriver
  --compare-ogr  ${EPSILON_9}
  ${BASELINE_FILES}/ioTvVectorDataFileWriterOutput.shp
  ${TEMP}/ioTvVectorDataFileWriterOutput.shp
  otbVectorDataFileWriter
  ${TEMP}/ioTvVectorDataFileWriterOutput.shp
  )

otb_add_test(NAME ioTvVectorDataFileWriterTwice COMMAND otbVectorDataIOTestDriver
  otbVectorDataFileWriter
  ${TEMP}/ioTvVectorDataFileWriterOutput.shp
  )
set_property(TEST ioTvVectorDataFileWriterTwice PROPERTY DEPENDS ioTvVectorDataFileWriter)

