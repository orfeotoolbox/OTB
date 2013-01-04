SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${Monteverdi_BINARY_DIR}/Testing/Temporary)

# Large Input
IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(Application_TESTS ${CXX_TEST_PATH}/otbApplicationTests)

#-------------- Monteverdi Application Tests ---------------------------------
ADD_TEST(apTvMonteverdi2Application ${Application_TESTS} otbMonteverdi2ApplicationTest)


SET(Application_SRCS
otbMonteverdi2ApplicationTest.cxx
)

ADD_EXECUTABLE(otbApplicationTests otbApplicationTests.cxx ${Application_SRCS})
TARGET_LINK_LIBRARIES(otbApplicationTests Monteverdi2_Application OTBTesting)
