otb_module_test()
set(OTBMathParserXTests
  otbParserXTest.cxx
  otbBandMathXImageFilter.cxx
  otbMathParserXTestDriver.cxx  )

add_executable(otbMathParserXTestDriver ${OTBMathParserXTests})
target_link_libraries(otbMathParserXTestDriver ${OTBMathParserX-Test_LIBRARIES})
otb_module_target_label(otbMathParserXTestDriver)
otb_add_test(NAME coTvParserX COMMAND otbMathParserXTestDriver
  otbParserXTest
  )
otb_add_test(NAME coTuParserX COMMAND otbMathParserXTestDriver
  otbParserXTestNew
  )
otb_add_test(NAME bfTuBandMathXImageFilterNew COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterNew)
otb_add_test(NAME bfTvBandMathXImageFilterConv COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterConv
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
otb_add_test(NAME bfTvBandMathXImageFilter COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilter)
otb_add_test(NAME bfTvBandMathXImageFilterWithIdx COMMAND otbMathParserXTestDriver
  otbBandMathXImageFilterWithIdx
  ${TEMP}/bfTvBandMathImageFilterWithIdx1.tif
  ${TEMP}/bfTvBandMathImageFilterWithIdx2.tif
  )
otb_add_test(NAME bfTvBandMathXImageFilterTxt COMMAND otbMathParserXTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  otbBandMathXImageFilterTxt
  ${BASELINE_FILES}/bfTvExportBandMathX.txt
  ${TEMP}/bfTvExportBandMathXOut.txt
  )
