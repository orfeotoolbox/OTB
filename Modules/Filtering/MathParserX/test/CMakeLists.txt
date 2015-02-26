otb_module_test()
set(OTBMathParserXTests
  otbParserXTest.cxx
  otbBandMathXImageFilter.cxx
  otbMathParserXTestDriver.cxx  )

add_executable(otbMathParserXTestDriver ${OTBMathParserXTests})
target_link_libraries(otbMathParserXTestDriver ${OTBMathParserX-Test_LIBRARIES})
otb_module_target_label(otbMathParserXTestDriver)
otb_add_test(NAME coTvParserX COMMAND otbMathParserTestDriver
  otbParserXTest
  )
otb_add_test(NAME coTuParserX COMMAND otbMathParserTestDriver
  otbParserXTestNew
  )
otb_add_test(NAME bfTuBandMathXImageFilterNew COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterNew)
otb_add_test(NAME bfTvBandMathXImageFilterConv COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterConv
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
otb_add_test(NAME bfTvBandMathXImageFilter COMMAND otbMathParserTestDriver
  otbBandMathXImageFilter)
otb_add_test(NAME bfTvBandMathXImageFilterWithIdx COMMAND otbMathParserTestDriver
  otbBandMathXImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
  )
otb_add_test(NAME bfTvBandMathXImageFilterTxt COMMAND otbMathParserTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  otbBandMathXImageFilterTxt
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
