
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(EXAMPLESDATA ${OTB_DATA_ROOT}/Examples)
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUTDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)


#Tolerance sur diff pixel image
SET(NOTOL 0.0)

#Tolerance sur la difference des valeurs numeriques dans le --compare-ascii
SET(EPS 0.001)

SET(TESTSYSTEM_TESTS ${CXX_TEST_PATH}/otbTestSystemTests)

#Test a basic ascii comparison
ADD_TEST(tsTvCompareAscii ${TESTSYSTEM_TESTS}
   --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef.txt
       ${TEMP}/tsTvCompareAsciiTest.txt
     otbCompareAsciiTests
       ${TEMP}/tsTvCompareAsciiRef.txt
       ${TEMP}/tsTvCompareAsciiTest.txt
  )

#make sure that it fails when it has to.. (baseline shorter than test)
ADD_TEST(tsTvCompareAscii2 ${TESTSYSTEM_TESTS}
   --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef2.txt
       ${TEMP}/tsTvCompareAsciiTest2.txt
     otbCompareAsciiTests2
       ${TEMP}/tsTvCompareAsciiRef2.txt
       ${TEMP}/tsTvCompareAsciiTest2.txt
  )

SET_TESTS_PROPERTIES(tsTvCompareAscii2 PROPERTIES WILL_FAIL true)

#with reverse order (test shorter than baseline)
ADD_TEST(tsTvCompareAscii2reverse ${TESTSYSTEM_TESTS}
   --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef2reverse.txt
       ${TEMP}/tsTvCompareAsciiTest2reverse.txt
     otbCompareAsciiTests2
       ${TEMP}/tsTvCompareAsciiTest2reverse.txt
       ${TEMP}/tsTvCompareAsciiRef2reverse.txt
  )

SET_TESTS_PROPERTIES(tsTvCompareAscii2reverse PROPERTIES WILL_FAIL true)

#ignoring the line order
ADD_TEST(tsTvCompareAscii3 ${TESTSYSTEM_TESTS}
   --ignore-order --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef3.txt
       ${TEMP}/tsTvCompareAsciiTest3.txt
     otbCompareAsciiTests3
       ${TEMP}/tsTvCompareAsciiRef3.txt
       ${TEMP}/tsTvCompareAsciiTest3.txt
  )

#fails if the line order is not ignored
ADD_TEST(tsTvCompareAscii3order ${TESTSYSTEM_TESTS}
   --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef3order.txt
       ${TEMP}/tsTvCompareAsciiTest3order.txt
     otbCompareAsciiTests3
       ${TEMP}/tsTvCompareAsciiRef3order.txt
       ${TEMP}/tsTvCompareAsciiTest3order.txt
  )

SET_TESTS_PROPERTIES(tsTvCompareAscii3order PROPERTIES WILL_FAIL true)

# -------       CXX source files -----------------------------------
SET(TESTSYSTEM_SRCS
otbTestSystemTests.cxx
otbCompareAsciiTests.cxx
otbCompareAsciiTests2.cxx
otbCompareAsciiTests3.cxx
)

INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)

OTB_ADD_EXECUTABLE(otbTestSystemTests "${TESTSYSTEM_SRCS}" "OTBTesting;OTBCommon;OTBIO")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
