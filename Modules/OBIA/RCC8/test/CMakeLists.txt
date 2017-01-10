otb_module_test()

set(OTBRCC8Tests
otbRCC8TestDriver.cxx
otbRCC8GraphNew.cxx
otbRCC8GraphFileReader.cxx
otbRCC8GraphIOEndToEnd.cxx
otbPolygonListToRCC8GraphFilter.cxx
otbRCC8VertexBase.cxx
otbRCC8GraphFileReaderNew.cxx
otbRCC8VertexWithCompacityNew.cxx
otbImageMultiSegmentationToRCC8GraphFilterNew.cxx
otbRCC8GraphFileWriterNew.cxx
otbPolygonToPolygonRCC8Calculator.cxx
otbImageListToRCC8GraphFilterNew.cxx
otbImageToImageRCC8Calculator.cxx
otbRCC8Edge.cxx
otbPolygonListToRCC8GraphFilterNew.cxx
otbRCC8VertexWithCompacity.cxx
otbRCC8EdgeNew.cxx
otbPolygonToPolygonRCC8CalculatorNew.cxx
otbRCC8VertexBaseNew.cxx
otbRCC8Graph.cxx
otbRCC8GraphSourceNew.cxx
otbRCC8GraphFileWriter.cxx
otbImageMultiSegmentationToRCC8GraphFilter.cxx
otbImageToImageRCC8CalculatorNew.cxx
)

add_executable(otbRCC8TestDriver ${OTBRCC8Tests})
target_link_libraries(otbRCC8TestDriver ${OTBRCC8-Test_LIBRARIES})
otb_module_target_label(otbRCC8TestDriver)

# Tests Declaration

otb_add_test(NAME srTuRCC8GraphNew COMMAND otbRCC8TestDriver
  otbRCC8GraphNew)

otb_add_test(NAME srTvRCC8GraphFileReader COMMAND otbRCC8TestDriver
  otbRCC8GraphFileReader
  ${TEMP}/srRCC8GraphWriterOutput1.dot
  )
set_property(TEST srTvRCC8GraphFileReader PROPERTY DEPENDS srTvRCC8GraphFileWriter)

otb_add_test(NAME srTvRCC8GraphIOEndToEnd COMMAND otbRCC8TestDriver
  --compare-ascii        ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
  ${TEMP}/srRCC8GraphIOEndToEndOut.dot
  otbRCC8GraphIOEndToEnd
  ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
  ${TEMP}/srRCC8GraphIOEndToEndOut.dot
  )

otb_add_test(NAME srTvPolygonListToRCC8GraphFilter COMMAND otbRCC8TestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/srTvPolygonListToRCC8GraphFilterOutput.dot
  ${TEMP}/srTvPolygonListToRCC8GraphFilterOutput.dot
  otbPolygonListToRCC8GraphFilter
  ${TEMP}/srTvPolygonListToRCC8GraphFilterOutput.dot)

otb_add_test(NAME srTvRCC8VertexBase COMMAND otbRCC8TestDriver
  otbRCC8VertexBase
  5
  5
  )

otb_add_test(NAME srTuRCC8GraphFileReaderNew COMMAND otbRCC8TestDriver
  otbRCC8GraphFileReaderNew)

otb_add_test(NAME srTuRCC8VertexWithCompacityNew COMMAND otbRCC8TestDriver
  otbRCC8VertexWithCompacityNew)

otb_add_test(NAME srTuMultiSegToRCC8GraphFilterNew COMMAND otbRCC8TestDriver
  otbImageMultiSegmentationToRCC8GraphFilterNew)

otb_add_test(NAME srTuRCC8GraphFileWriterNew COMMAND otbRCC8TestDriver
  otbRCC8GraphFileWriterNew)

otb_add_test(NAME srTvPolygonRCC8Calculator COMMAND otbRCC8TestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/srRCC8PolygonToPolygonCalculatorOutput.txt
  ${TEMP}/srRCC8PolygonToPolygonCalculatorOutput.txt
  otbPolygonToPolygonRCC8Calculator
  4
  ${TEMP}/srRCC8PolygonToPolygonCalculatorOutput.txt
  ${INPUTDATA}/rcc8_mire1.png
  ${INPUTDATA}/rcc8_mire2.png
  ${INPUTDATA}/rcc8_mire3.png
  ${INPUTDATA}/rcc8_mire4.png
  )

otb_add_test(NAME srTuImageListToRCC8GraphFilterNew COMMAND otbRCC8TestDriver
  otbImageListToRCC8GraphFilterNew)

otb_add_test(NAME srTvRCC8Calculator COMMAND otbRCC8TestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/srRCC8CalculatorOutput.txt
  ${TEMP}/srRCC8CalculatorOutput.txt
  otbImageToImageRCC8Calculator
  4
  ${TEMP}/srRCC8CalculatorOutput.txt
  ${INPUTDATA}/rcc8_mire1.png
  ${INPUTDATA}/rcc8_mire2.png
  ${INPUTDATA}/rcc8_mire3.png
  ${INPUTDATA}/rcc8_mire4.png
  )

otb_add_test(NAME srTvRCC8Edge COMMAND otbRCC8TestDriver
  otbRCC8Edge
  )

otb_add_test(NAME srTuPolygonListToRCC8GraphFilterNew COMMAND otbRCC8TestDriver
  otbPolygonListToRCC8GraphFilterNew)

otb_add_test(NAME srTvRCC8VertexWithCompacity COMMAND otbRCC8TestDriver
  otbRCC8VertexWithCompacity
  5
  5
  0.125354
  )

otb_add_test(NAME srTuRCC8EdgeNew COMMAND otbRCC8TestDriver
  otbRCC8EdgeNew)

otb_add_test(NAME srTuPolygonRCC8CalculatorNew COMMAND otbRCC8TestDriver
  otbPolygonToPolygonRCC8CalculatorNew)

otb_add_test(NAME srTuRCC8VertexBaseNew COMMAND otbRCC8TestDriver
  otbRCC8VertexBaseNew)

otb_add_test(NAME srTvRCC8Graph COMMAND otbRCC8TestDriver
  otbRCC8Graph
  )

otb_add_test(NAME srTuRCC8GraphSourceNew COMMAND otbRCC8TestDriver
  otbRCC8GraphSourceNew)

otb_add_test(NAME srTvRCC8GraphFileWriter COMMAND otbRCC8TestDriver
  --compare-ascii        ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphWriterOutput1.dot
  ${TEMP}/srRCC8GraphWriterOutput1.dot
  otbRCC8GraphFileWriter
  ${TEMP}/srRCC8GraphWriterOutput1.dot
  )

otb_add_test(NAME srTvMultiSegToRCC8GraphFilter1 COMMAND otbRCC8TestDriver
  --compare-ascii        ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
  ${TEMP}/srRCC8GraphFilterOutput.dot
  otbImageMultiSegmentationToRCC8GraphFilter
  ${TEMP}/srRCC8GraphFilterOutput.dot
  0
  2
  ${INPUTDATA}/Seg1InputForRCC8Graph.tif
  ${INPUTDATA}/Seg2InputForRCC8Graph.tif
  )

otb_add_test(NAME srTvMultiSegToRCC8GraphFilter2 COMMAND otbRCC8TestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphFilterOutput2.dot
  ${TEMP}/srRCC8GraphFilterOutput2.dot
  otbImageMultiSegmentationToRCC8GraphFilter
  ${TEMP}/srRCC8GraphFilterOutput2.dot
  0
  3
  ${INPUTDATA}/Seg3InputForRCC8Graph.tif
  ${INPUTDATA}/Seg4InputForRCC8Graph.tif
  ${INPUTDATA}/Seg5InputForRCC8Graph.tif
  )

otb_add_test(NAME srTvMultiSegToRCC8GraphFilter2WithOpti COMMAND otbRCC8TestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphFilterOutput2.dot
  ${TEMP}/srRCC8GraphFilterOutput2WithOpti.dot
  otbImageMultiSegmentationToRCC8GraphFilter
  ${TEMP}/srRCC8GraphFilterOutput2WithOpti.dot
  1
  3
  ${INPUTDATA}/Seg3InputForRCC8Graph.tif
  ${INPUTDATA}/Seg4InputForRCC8Graph.tif
  ${INPUTDATA}/Seg5InputForRCC8Graph.tif
  )

otb_add_test(NAME srTvMultiSegToRCC8GraphFilter1WithOpti COMMAND otbRCC8TestDriver
  --compare-ascii        ${NOTOL}
  ${BASELINE_FILES}/srRCC8GraphFilterOutput.dot
  ${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
  otbImageMultiSegmentationToRCC8GraphFilter
  ${TEMP}/srRCC8GraphFilterOutputWithOpti.dot
  1
  2
  ${INPUTDATA}/Seg1InputForRCC8Graph.tif
  ${INPUTDATA}/Seg2InputForRCC8Graph.tif
  )

otb_add_test(NAME srTuRCC8CalculatorNew COMMAND otbRCC8TestDriver
  otbImageToImageRCC8CalculatorNew)

