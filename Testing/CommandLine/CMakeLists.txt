set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB-Wrapper_BINARY_DIR}/Testing/Temporary)
set(PATH ${OTB-Wrapper_BINARY_DIR}/Example)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

set(OTB_WRAPPER_COMMAND_LINE_TESTS ${CXX_TEST_PATH}/otbWrapperCommandLineTests)

# ----------------Command Line Parser tests -----------------------------------
add_test(clTuWrapperCommandLineParserNew ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserNew
  )

add_test(clTvWrapperCommandLineParserTest_GetAtt ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest1
        "--key"
        "this is a line test to extract --key name1 name2"
  )

add_test(clTvWrapperCommandLineParserTest_GetAttWithoutValue ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest1
        "--key"
        "--key"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_GetAttWithoutValue PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_NoModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        ""
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NoModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "-Re+scale --modulePath ${PATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_GoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath ${PATH}"
)

add_test(clTvWrapperCommandLineParserTest_WithKeyNoModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName --modulePath ${PATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNoModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName -Re+scale --modulePath ${PATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyGoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName Rescale --modulePath ${PATH}"
)

add_test(clTvWrapperCommandLineParserTest_NoPath ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath"
)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NoPath PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WrongPath ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath FakePath"
)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WrongPath PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_MultiplePathOneWrong ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath ${PATH} FakePath"
)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_MultiplePathOneWrong PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_MultiplePath ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath ${PATH} ${PATH}"
)


# ----------------Command Laine Launcher tests -----------------------------------
add_test(clTuWrapperCommandLineLauncherNew ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherNew
  )

add_test(clTvWrapperCommandLineLauncherTest ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
  )


# --------------------------------------------------------------------
# ----------------Source files CXX -----------------------------------
# --------------------------------------------------------------------
set(Wrapper_Command_Line_SRCS
otbWrapperCommandLineTests.cxx

otbWrapperCommandLineParserTests.cxx
otbWrapperCommandLineLauncherTests.cxx
)

include_directories(${CMAKE_SOURCE_DIR}/Code/Wrappers/CommandLine ${CMAKE_SOURCE_DIR}/Code/Core)
add_executable(otbWrapperCommandLineTests ${Wrapper_Command_Line_SRCS})
target_link_libraries(otbWrapperCommandLineTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCommandLine)
#target_link_libraries(otbWrapperCommandLineTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCommandLine)

