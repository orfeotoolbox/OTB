otb_module_test()

set(OTBCommonTests
otbCommonTestDriver.cxx
otbVariableLengthVectorConverter.cxx
otbImageRegionTileMapSplitter.cxx
otbImageRegionAdaptativeSplitter.cxx
otbRGBAPixelConverter.cxx
otbRectangle.cxx
otbImageRegionNonUniformMultidimensionalSplitterNew.cxx
otbTestCommandLineArgumentParserHelp.cxx
otbTestCommandLineArgumentParserWithError.cxx
otbSystemTest.cxx
otbImageRegionSquareTileSplitter.cxx
otbTestCommandLineArgumentParserList.cxx
otbImageRegionTileMapSplitterNew.cxx
otbTestCommandLineArgumentParser.cxx
otbImageRegionNonUniformMultidimensionalSplitter.cxx
otbRectangleNew.cxx
otbConfigurationManagerTest.cxx
)

add_executable(otbCommonTestDriver ${OTBCommonTests})
target_link_libraries(otbCommonTestDriver ${OTBCommon-Test_LIBRARIES})
otb_module_target_label(otbCommonTestDriver)

# Tests Declaration


otb_add_test(NAME coTuVariableLengthVectorConverter COMMAND otbCommonTestDriver
  otbVariableLengthVectorConverterNew
  )

otb_add_test(NAME coTvImageRegionTileMapSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionTileMapSplitter.txt
  ${TEMP}/coImageRegionTileMapSplitter.txt
  otbImageRegionTileMapSplitter
  ${TEMP}/coImageRegionTileMapSplitter.txt
  )


otb_add_test(NAME coTuImageRegionAdaptativeSplitterNew COMMAND otbCommonTestDriver
  otbImageRegionAdaptativeSplitterNew)

otb_add_test(NAME coTvImageRegionAdaptativeSplitterStripSmallStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 1000 20
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripSmallStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterSquaredTileLargeStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileLargeStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterSquaredTileSmallStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8192 8192 2048 2048 32
  ${TEMP}/coTvImageRegionAdaptativeSplitterSquaredTileSmallStreamOutput.txt
  )

otb_add_test(NAME coTvImageRegionAdaptativeSplitterStripLargeStream COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  otbImageRegionAdaptativeSplitter
  0 0 8000 8003 8000 16 8
  ${TEMP}/coTvImageRegionAdaptativeSplitterStripLargeStreamOutput.txt
  )

otb_add_test(NAME coTuRGBAPixelConverter COMMAND otbCommonTestDriver
  otbRGBAPixelConverterNew
  )

otb_add_test(NAME coTvRGBAPixelConverter COMMAND otbCommonTestDriver
  otbRGBAPixelConverter
  )

otb_add_test(NAME coTvRectangle COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/otbRectangleTest.txt
  ${TEMP}/otbRectangleTest.txt
  otbRectangle
  ${TEMP}/otbRectangleTest.txt
  2.12 2.35 12.54 2.45   3. 0.15  1. 1.
  )






otb_add_test(NAME coTuImageRegionNonUniformMultidimensionalSplitterNew COMMAND otbCommonTestDriver
  otbImageRegionNonUniformMultidimensionalSplitterNew
  )

otb_add_test(NAME coTuCmdLineArgParserHelpWithArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserHelp
  --help -image image.png)

otb_add_test(NAME coTuCmdLineArgParserHelpWithoutArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserHelp
  --help)



otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_LessArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 )

otb_add_test(NAME coTuCmdLineArgParser_MissingObligatoryArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -entier 150 -double 12.25 -deuxentiers 78 56 32)

otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_MoreArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 32)

otb_add_test(NAME coTuSystemTests_IsA_Methods COMMAND otbCommonTestDriver
  otbSystemTest
  ${OTB_DATA_ROOT}/README-OTB-Data
  ${OTB_DATA_ROOT}
  )

otb_add_test(NAME coTvParseHdfSubsetName COMMAND otbCommonTestDriver
  otbParseHdfSubsetName)

otb_add_test(NAME coTvParseHdfFileName COMMAND otbCommonTestDriver
  otbParseHdfFileName)

otb_add_test(NAME coTuImageRegionSquareTileSplitterNew COMMAND otbCommonTestDriver
  otbImageRegionSquareTileSplitterNew
  )

otb_add_test(NAME coTvImageRegionSquareTileSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionSquareTileSplitter.txt
  ${TEMP}/coImageRegionSquareTileSplitter.txt
  otbImageRegionSquareTileSplitter
  ${TEMP}/coImageRegionSquareTileSplitter.txt
  )

otb_add_test(NAME coTuCmdLineArgParser_List2Arg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.0)

otb_add_test(NAME coTuCmdLineArgParser_List2ArgBis COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -doubles 1.0 2.0 -deuxentiers 78 56 )

otb_add_test(NAME coTuCmdLineArgParser_EmptyList COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles)

otb_add_test(NAME coTuCmdLineArgParser_List1Arg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5)

otb_add_test(NAME coTuCmdLineArgParser_List3ArgWithError COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserList
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 -doubles 1.0 2.5 -3.5)

otb_add_test(NAME coTuImageRegionTileMapSplitterNew COMMAND otbCommonTestDriver
  otbImageRegionTileMapSplitterNew
  )

otb_add_test(NAME coTuCmdLineArgParserDeuxEntiers_ExactNumberArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParser
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )



otb_add_test(NAME coTvImageRegionNonUniformMultidimensionalSplitter COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coImageRegionNonUniformMultidimensionalSplitter.txt
  ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
  otbImageRegionNonUniformMultidimensionalSplitter
  45 45      # Index
  1000 1500  # Size
  10         # Nb split
  2          # Number of slipt
  ${TEMP}/coImageRegionNonUniformMultidimensionalSplitter.txt
  )


otb_add_test(NAME coTuRectangleNew COMMAND otbCommonTestDriver
  otbRectangleNew)

otb_add_test(NAME coTvConfigurationManagerDefault COMMAND otbCommonTestDriver
  otbConfigurationManagerTest
  128)



otb_add_test(NAME coTvConfigurationManagerEnvVar COMMAND otbTestDriver
  --add-before-env OTB_MAX_RAM_HINT "256"
  --add-before-env OTB_DEM_DIRECTORY "/path/to/dem/"
  --add-before-env OTB_GEOID_FILE "/path/to/geoid.file"
  Execute $<TARGET_FILE:otbCommonTestDriver>
  otbConfigurationManagerTest
  256 /path/to/dem/ /path/to/geoid.file)
