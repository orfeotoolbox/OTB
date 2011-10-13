
SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(APPLICATIONSPATH ${OTB_BINARY_DIR}/bin)

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

add_test(clTvWrapperCommandLineParserTest_IsAttExists_Wrong ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest3
        "--m"
        " TrainImagesClassifier --il QB_1_ortho.tif --vd vd .shape --modulePath test"
  )
  
  
add_test(clTvWrapperCommandLineParserTest_IsAttExists ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest4
        "--m"
         " TrainImagesClassifier --il QB_1_ortho.tif --vd vd .shape --m name --modulePath test"
  )
   
add_test(clTvWrapperCommandLineParserTest_IsAttExistsEnd ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest4
        "--m"
         " TrainImagesClassifier --il QB_1_ortho.tif --vd vd .shape --modulePath test --m name"
  )
    
add_test(clTvWrapperCommandLineParserTest_IsAttExistsEndNoParam ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest4
        "--m"
        " TrainImagesClassifier --il QB_1_ortho.tif --vd vd .shape --modulePath test --m"
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
        "-Re+scale --modulePath ${APPLICATIONSPATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_GoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath ${APPLICATIONSPATH}"
)

add_test(clTvWrapperCommandLineParserTest_WithKeyNoModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName --modulePath ${APPLICATIONSPATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNoModule PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName -Re+scale --modulePath ${APPLICATIONSPATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_WithKeyGoodModule ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "--moduleName Rescale --modulePath ${APPLICATIONSPATH}"
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
        "Rescale --modulePath ${APPLICATIONSPATH} FakePath"
)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_MultiplePathOneWrong PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineParserTest_MultiplePath ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineParserTest2
        "Rescale --modulePath ${APPLICATIONSPATH} ${APPLICATIONSPATH}"
)


# ----------------Command Laine Launcher tests -----------------------------------
add_test(clTuWrapperCommandLineLauncherNew ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherNew
  )

add_test(clTvWrapperCommandLineLauncherTest ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} --in ${INPUTDATA}/poupees.tif --out ${TEMP}/clTvWrapperCommandLineLauncherTest.tif --outmin 15 --outmax 200"
  )

add_test(clTvWrapperCommandLineLauncherTest_Help ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} --help"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_Help PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineLauncherTest_Help2 ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} --in ${INPUTDATA}/poupees.tif --out ${TEMP}/clTvWrapperCommandLineLauncherTest_Help2.tif --outmin 15 --outmax 200 --help"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_Help2 PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineLauncherTest_MissingManParam ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH}"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_MissingManParam PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineLauncherTest_MissingDash ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} -in image1"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_MissingDash PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineLauncherTest_DoubleParam ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} --in image1 --in image2"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_DoubleParam PROPERTIES WILL_FAIL true)

add_test(clTvWrapperCommandLineLauncherTest_WrongParam ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	otbWrapperCommandLineLauncherTest
        "Rescale --modulePath ${APPLICATIONSPATH} --inn image"
  )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_WrongParam PROPERTIES WILL_FAIL true)


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
