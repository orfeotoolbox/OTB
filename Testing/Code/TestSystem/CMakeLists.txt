
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
SET(EPSILON_2 0.01)
SET(EPSILON_3 0.001)

SET(TESTSYSTEM_TESTS ${CXX_TEST_PATH}/otbTestSystemTests)

#Test if the current working copy corresponds to the nightly revision number
ADD_TEST(tsTuIsNightlyRevision ${TESTSYSTEM_TESTS}   
         otbIsNightlyRevision
         ${OTB_WC_REVISION}
         http://www.orfeo-toolbox.org/nightly/libNightlyNumber )
  
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

#Check precision
#without tolerance, it should not pass
ADD_TEST(tsTvCompareAscii4 ${TESTSYSTEM_TESTS}
   --compare-ascii ${NOTOL}
       ${TEMP}/tsTvCompareAsciiRef4.txt
       ${TEMP}/tsTvCompareAsciiTest4.txt
     otbCompareAsciiTests4
       ${TEMP}/tsTvCompareAsciiRef4.txt
       ${TEMP}/tsTvCompareAsciiTest4.txt
  )

SET_TESTS_PROPERTIES(tsTvCompareAscii4 PROPERTIES WILL_FAIL true)

# 10 ^-2 is just enough to make it work
ADD_TEST(tsTvCompareAscii4epsilon2 ${TESTSYSTEM_TESTS}
   --compare-ascii ${EPSILON_2}
       ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
       ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
     otbCompareAsciiTests4
       ${TEMP}/tsTvCompareAsciiRef4epsilon2.txt
       ${TEMP}/tsTvCompareAsciiTest4epsilon2.txt
  )

# 10 ^-3 will fail on the last value and only this one
ADD_TEST(tsTvCompareAscii4epsilon3 ${TESTSYSTEM_TESTS}
   --compare-ascii ${EPSILON_3}
       ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
       ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
     otbCompareAsciiTests4
       ${TEMP}/tsTvCompareAsciiRef4epsilon3.txt
       ${TEMP}/tsTvCompareAsciiTest4epsilon3.txt
  )

SET_TESTS_PROPERTIES(tsTvCompareAscii4epsilon3 PROPERTIES WILL_FAIL true)

# Tests about precision (10^-3) and strings with white space
ADD_TEST(tsTvCompareAsciiepsilon3_WhiteSpace ${TESTSYSTEM_TESTS}
   --compare-ascii ${EPSILON_3}
       ${BASELINE_FILES}/tsTvCompareAsciiEpsilon3_Base.txt
       ${INPUTDATA}/tsTvCompareAsciiEpsilon3_Test.txt
       #${TEMP}/tsTvCompareAsciiRefepsilon3_WS.txt
       #${TEMP}/tsTvCompareAsciiTestepsilon3_WS.txt
     otbCompareAsciiTestsEpsilon3_WhiteSpace
       #${TEMP}/tsTvCompareAsciiRefepsilon3_WS.txt
       #${TEMP}/tsTvCompareAsciiTestepsilon3_WS.txt
  )

#Check that the order and the tolerance works together
ADD_TEST(tsTvCompareAscii5epsilon2 ${TESTSYSTEM_TESTS}
  --ignore-order  --compare-ascii ${EPSILON_2}
       ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
       ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
     otbCompareAsciiTests5
       ${TEMP}/tsTvCompareAsciiRef5epsilon2.txt
       ${TEMP}/tsTvCompareAsciiTest5epsilon2.txt
  )

#Check the case of scientific notation 1e25 vs 1E025
ADD_TEST(tsTvCompareAsciiScientificNotation ${TESTSYSTEM_TESTS}
    --compare-ascii ${EPSILON_2}
       ${TEMP}/tsTvCompareAsciiRef_SN.txt
       ${TEMP}/tsTvCompareAsciiTest_SN.txt
     otbCompareAsciiTestScientificNotation
       ${TEMP}/tsTvCompareAsciiRef_SN.txt
       ${TEMP}/tsTvCompareAsciiTest_SN.txt
  )

# -------       CXX source files -----------------------------------
SET(TESTSYSTEM_SRCS
otbTestSystemTests.cxx
otbIsNightlyRevision.cxx
otbCompareAsciiTests.cxx
otbCompareAsciiTests2.cxx
otbCompareAsciiTests3.cxx
otbCompareAsciiTests4.cxx
otbCompareAsciiTests5.cxx
otbCompareAsciiTestsEpsilon3_WhiteSpace.cxx
otbCompareAsciiTestScientificNotation.cxx
)

OTB_ADD_EXECUTABLE(otbTestSystemTests "${TESTSYSTEM_SRCS}" "OTBTesting;OTBCommon;OTBIO")

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
