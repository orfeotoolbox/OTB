otb_module_test()

set(OTBStreamingTests
otbStreamingTestDriver.cxx
otbStreamingManager.cxx
otbPipelineMemoryPrintCalculatorTest.cxx
)

add_executable(otbStreamingTestDriver ${OTBStreamingTests})
target_link_libraries(otbStreamingTestDriver ${OTBStreaming-Test_LIBRARIES})
otb_module_target_label(otbStreamingTestDriver)

# Tests Declaration

otb_add_test(NAME coTvRAMDrivenAdaptativeStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenAdaptativeStreamingManager.txt
  ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
  otbRAMDrivenAdaptativeStreamingManager
  ${TEMP}/coTvRAMDrivenAdaptativeStreamingManager.txt
  )

otb_add_test(NAME coTvRAMDrivenStrippedStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenStrippedStreamingManager.txt
  ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  otbRAMDrivenStrippedStreamingManager
  ${TEMP}/coTvRAMDrivenStrippedStreamingManager.txt
  )

otb_add_test(NAME coTvNumberOfLinesStrippedStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvNumberOfLinesStrippedStreamingManager.txt
  ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  otbNumberOfLinesStrippedStreamingManager
  ${TEMP}/coTvNumberOfLinesStrippedStreamingManager.txt
  )

otb_add_test(NAME coTvRAMDrivenTiledStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvRAMDrivenTiledStreamingManager.txt
  ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  otbRAMDrivenTiledStreamingManager
  ${TEMP}/coTvRAMDrivenTiledStreamingManager.txt
  )

otb_add_test(NAME coTuStreamingManagerNew COMMAND otbStreamingTestDriver
  otbStreamingManagerNew
  )

otb_add_test(NAME coTvTileDimensionTiledStreamingManager COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvTileDimensionTiledStreamingManager.txt
  ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  otbTileDimensionTiledStreamingManager
  ${TEMP}/coTvTileDimensionTiledStreamingManager.txt
  )

otb_add_test(NAME coTvPipelineMemoryPrintCalculator COMMAND otbStreamingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPipelineMemoryPrintCalculatorOutput.txt
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  otbPipelineMemoryPrintCalculatorTest
  ${INPUTDATA}/qb_RoadExtract.img
  ${TEMP}/coTvPipelineMemoryPrintCalculatorOutput.txt
  )
otb_add_test(NAME coTuPipelineMemoryPrintCalculatorNew COMMAND otbStreamingTestDriver
  otbPipelineMemoryPrintCalculatorNew
  )
