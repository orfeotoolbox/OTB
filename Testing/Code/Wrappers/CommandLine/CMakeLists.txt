
SET(APPLICATIONSPATH ${OTB_BINARY_DIR}/bin)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

set(OTB_WRAPPER_COMMAND_LINE_TESTS ${CXX_TEST_PATH}/otbWrapperCommandLineTests)

# ----------------Command Line Parser tests -----------------------------------
add_test(NAME clTuWrapperCommandLineParserNew
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserNew )

add_test(NAME clTvWrapperCommandLineParserTest_GetAtt
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest1
                 "-key"
                 "this is a line test to extract -key name1 name2" )

add_test(NAME clTvWrapperCommandLineParserTest_IsAttExists_Wrong
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest3
                    "-m"
                    " TrainImagesClassifier test -il QB_1_ortho.tif -vd vd .shape " )


add_test(NAME clTvWrapperCommandLineParserTest_IsAttExists
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest4
                 "-m"
                 " TrainImagesClassifier test -il QB_1_ortho.tif -vd vd .shape -m name")

add_test(NAME clTvWrapperCommandLineParserTest_IsAttExistsEnd
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest4
                 "-m"
                 " TrainImagesClassifier test -il QB_1_ortho.tif -vd vd .shape -m name")

add_test(NAME clTvWrapperCommandLineParserTest_IsAttExistsEndNoParam
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest4
                 "-m"
                 " TrainImagesClassifier test -il QB_1_ortho.tif -vd vd .shape test -m")

add_test(NAME clTvWrapperCommandLineParserTest_GetAttWithoutValue
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest1
                    "-key"
                    "-key" )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_GetAttWithoutValue PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_NoModule
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "")
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NoModule PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_NonAlphaNumModuleName
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	             otbWrapperCommandLineParserTest2
                 "-Re+scale"
                 $<TARGET_FILE_DIR:otbapp_Rescale> )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_GoodModule
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS}
	             otbWrapperCommandLineParserTest2
                 "Rescale"
                 $<TARGET_FILE_DIR:otbapp_Rescale> )

add_test(NAME clTvWrapperCommandLineParserTest_WithKeyNoModule
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 $<TARGET_FILE_DIR:otbapp_Rescale> )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNoModule PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "-Re+scale" $<TARGET_FILE_DIR:otbapp_Rescale> )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WithKeyNonAlphaNumModuleName PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_WithKeyGoodModule
	     COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "Rescale"  $<TARGET_FILE_DIR:otbapp_Rescale>)
add_test(NAME clTvWrapperCommandLineParserTest_NoPath
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "Rescale" )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_NoPath PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_WrongPath
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "Rescale" "FakePath")
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_WrongPath PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_MultiplePathOneWrong
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> "FakePath")
SET_TESTS_PROPERTIES(clTvWrapperCommandLineParserTest_MultiplePathOneWrong PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineParserTest_MultiplePath
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineParserTest2
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> $<TARGET_FILE_DIR:otbapp_Rescale>)


# ----------------Command Laine Launcher tests -----------------------------------
add_test(NAME clTuWrapperCommandLineLauncherNew
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherNew )

add_test(NAME clTvWrapperCommandLineLauncherTest
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale>
                 -in ${INPUTDATA}/poupees.tif
                 -out ${TEMP}/clTvWrapperCommandLineLauncherTest.tif
                 -outmin 15
                 -outmax 200 )

add_test(NAME clTvWrapperCommandLineLauncherTest_Help
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> "-help")
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_Help PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineLauncherTest_Help2
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale>
                 -in ${INPUTDATA}/poupees.tif
                 -out ${TEMP}/clTvWrapperCommandLineLauncherTest_Help2.tif
                 -outmin 15
                 -outmax 200
                 -help)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_Help2 PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineLauncherTest_MissingManParam
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_MissingManParam PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineLauncherTest_MissingDash
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -in image1)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_MissingDash PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineLauncherTest_DoubleParam
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -in image1 -in image2 )
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_DoubleParam PROPERTIES WILL_FAIL true)

add_test(NAME clTvWrapperCommandLineLauncherTest_WrongParam
         COMMAND ${OTB_WRAPPER_COMMAND_LINE_TESTS} otbWrapperCommandLineLauncherTest
                 "Rescale" $<TARGET_FILE_DIR:otbapp_Rescale> -inn image)
SET_TESTS_PROPERTIES(clTvWrapperCommandLineLauncherTest_WrongParam PROPERTIES WILL_FAIL true)


# --------------------------------------------------------------------
# ----------------Source files CXX -----------------------------------
# --------------------------------------------------------------------
set(Wrapper_Command_Line_SRCS
otbWrapperCommandLineTests.cxx

otbWrapperCommandLineParserTests.cxx
otbWrapperCommandLineLauncherTests.cxx
)

include_directories(${CMAKE_SOURCE_DIR}/Code/Wrappers/CommandLine)
add_executable(otbWrapperCommandLineTests ${Wrapper_Command_Line_SRCS})
target_link_libraries(otbWrapperCommandLineTests OTBIO OTBCommon OTBTesting OTBWrapperCommandLine)
