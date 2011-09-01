set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB-Wrapper_BINARY_DIR}/Testing/Temporary)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

set(OTB_WRAPPER_COMMAND_LINE_TESTS ${CXX_TEST_PATH}/otbWrapperCommandLineTests)

# ----------------Command Line Parser tests -----------------------------------
add_test(clTuWrapperCommandLineParserNew ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserNew
  )


add_test(clTvWrapperCommandLineParserTest_NoModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        ""
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NoModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "-Re+scale"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_NonExistingModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "Fictive"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NonExistingModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_GoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "Rescale"
)

add_test(clTvWrapperCommandLineParserTest_WithKeyNoModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "--moduleName"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNoModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "--moduleName -Re+scale"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyNonExistingModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "--moduleName Fictive"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNonExistingModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyGoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest
        "--moduleName Rescale"
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

