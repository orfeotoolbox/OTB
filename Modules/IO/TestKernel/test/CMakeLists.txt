otb_module_test()
set(OTBTestKernelTests
  otbCompareAsciiTests2.cxx
  otbCompareAsciiTests3.cxx
  otbCompareAsciiTests4.cxx
  otbCompareAsciiTests5.cxx
  otbCompareAsciiTestScientificNotation.cxx
  otbCompareAsciiTests.cxx
  otbCompareAsciiTestsEpsilon3_WhiteSpace.cxx
  otbTestKernelTestDriver.cxx  )

add_executable(otbTestKernelTestDriver ${OTBTestKernelTests})
target_link_libraries(otbTestKernelTestDriver ${OTBTestKernel-Test_LIBRARIES})
otb_module_target_label(otbTestKernelTestDriver)
otb_add_test(NAME tsTvCompareAscii2 COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef2.txt
  ${TEMP}/tsTvCompareAsciiTest2.txt
  otbCompareAsciiTests2
  ${TEMP}/tsTvCompareAsciiRef2.txt
  ${TEMP}/tsTvCompareAsciiTest2.txt
  )
set_property(TEST tsTvCompareAscii2 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii2reverse COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef2reverse.txt
  ${TEMP}/tsTvCompareAsciiTest2reverse.txt
  otbCompareAsciiTests2
  ${TEMP}/tsTvCompareAsciiTest2reverse.txt
  ${TEMP}/tsTvCompareAsciiRef2reverse.txt
  )
set_property(TEST tsTvCompareAscii2reverse PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii3order COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef3order.txt
  ${TEMP}/tsTvCompareAsciiTest3order.txt
  otbCompareAsciiTests3
  ${TEMP}/tsTvCompareAsciiRef3order.txt
  ${TEMP}/tsTvCompareAsciiTest3order.txt
  )
set_property(TEST tsTvCompareAscii3order PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii3 COMMAND otbCommonTestDriver
  --ignore-order --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef3.txt
  ${TEMP}/tsTvCompareAsciiTest3.txt
  otbCompareAsciiTests3
  ${TEMP}/tsTvCompareAsciiRef3.txt
  ${TEMP}/tsTvCompareAsciiTest3.txt
  )
otb_add_test(NAME tsTvCompareAscii4 COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef4.txt
  ${TEMP}/tsTvCompareAsciiTest4.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4.txt
  ${TEMP}/tsTvCompareAsciiTest4.txt
  )
set_property(TEST tsTvCompareAscii4 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii4epsilon2 COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
  )
otb_add_test(NAME tsTvCompareAscii4epsilon3 COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_3}
  ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
  otbCompareAsciiTests4
  ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
  ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
  )
set_property(TEST tsTvCompareAscii4epsilon3 PROPERTY WILL_FAIL true)
otb_add_test(NAME tsTvCompareAscii5epsilon2 COMMAND otbCommonTestDriver
  --ignore-order  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
  otbCompareAsciiTests5
  ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
  ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
  )
otb_add_test(NAME tsTvCompareAsciiScientificNotation COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_2}
  ${TEMP}/tsTvCompareAsciiRef_SN.txt
  ${TEMP}/tsTvCompareAsciiTest_SN.txt
  otbCompareAsciiTestScientificNotation
  ${TEMP}/tsTvCompareAsciiRef_SN.txt
  ${TEMP}/tsTvCompareAsciiTest_SN.txt
  )
otb_add_test(NAME tsTvCompareAscii COMMAND otbCommonTestDriver
  --compare-ascii ${NOTOL}
  ${TEMP}/tsTvCompareAsciiRef.txt
  ${TEMP}/tsTvCompareAsciiTest.txt
  otbCompareAsciiTests
  ${TEMP}/tsTvCompareAsciiRef.txt
  ${TEMP}/tsTvCompareAsciiTest.txt
  )
otb_add_test(NAME tsTvCompareAsciiepsilon3_WhiteSpaceOK COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/tsTvCompareAsciiEpsilon3_Base.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestOK.txt
  otbCompareAsciiTestsEpsilon3_WhiteSpace
  ${INPUTDATA}/tsTvCompareAsciiEpsilon3_TestOK.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestOK.txt
  )
otb_add_test(NAME tsTvCompareAsciiepsilon3_WhiteSpaceKO COMMAND otbCommonTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/tsTvCompareAsciiEpsilon3_Base.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestKO.txt
  otbCompareAsciiTestsEpsilon3_WhiteSpace
  ${INPUTDATA}/tsTvCompareAsciiEpsilon3_TestKO.txt
  ${TEMP}/tsTvCompareAsciiEpsilon3_TestKO.txt
  )
set_property(TEST tsTvCompareAsciiepsilon3_WhiteSpaceKO PROPERTY WILL_FAIL true)
