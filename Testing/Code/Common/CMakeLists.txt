
IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Common)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

SET(COMMON_TESTS ${CXX_TEST_PATH}/otbCommonTests)

ADD_TEST(otbExceptionTest ${COMMON_TESTS} otbExceptionTest)


SET(BasicCommon_SRCS
otbExceptionTest.cxx
)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")


ADD_EXECUTABLE(otbCommonTests otbCommonTests.cxx ${BasicCommon_SRCS})
TARGET_LINK_LIBRARIES(otbCommonTests OTBCommon ITKIO ITKCommon)


ENDIF( NOT OTB_DISABLE_CXX_TESTING )




