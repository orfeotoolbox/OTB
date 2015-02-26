otb_module_test()
set(OTBCommandLineParserTests
  otbTestCommandLineArgumentParser.cxx
  otbTestCommandLineArgumentParserHelp.cxx
  otbTestCommandLineArgumentParserList.cxx
  otbTestCommandLineArgumentParserWithError.cxx
  otbCommandLineParserTestDriver.cxx  )

add_executable(otbCommandLineParserTestDriver ${OTBCommandLineParserTests})
target_link_libraries(otbCommandLineParserTestDriver ${OTBCommandLineParser-Test_LIBRARIES})
otb_module_target_label(otbCommandLineParserTestDriver)
otb_add_test(NAME coTuCmdLineArgParserDeuxEntiers_ExactNumberArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParser
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 )
otb_add_test(NAME coTuCmdLineArgParserHelpWithArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserHelp
  --help -image image.png)
otb_add_test(NAME coTuCmdLineArgParserHelpWithoutArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserHelp
  --help)
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
otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_LessArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 )
otb_add_test(NAME coTuCmdLineArgParser_MissingObligatoryArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -entier 150 -double 12.25 -deuxentiers 78 56 32)
otb_add_test(NAME coTuCmdLineArgParserDeuxEntier_MoreArg COMMAND otbCommonTestDriver
  otbTestCommandLineArgumentParserWithError
  -image image.png -entier 150 -double 12.25 -deuxentiers 78 56 32)
