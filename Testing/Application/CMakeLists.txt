set(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${Monteverdi_BINARY_DIR}/Testing/Temporary)

# Large Input
if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(Application_TESTS ${CXX_TEST_PATH}/otbApplicationTests)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  
  ${Monteverdi2_SOURCE_DIR}/Code/Application
  ${Monteverdi2_BINARY_DIR}/Code/Application
  )

#-------------- Monteverdi Application Tests ---------------------------------
add_test(apTvMonteverdi2Application ${Application_TESTS} otbMonteverdi2ApplicationTest)


set(Application_SRCS
otbMonteverdi2ApplicationTest.cxx
)

add_executable(otbApplicationTests otbApplicationTests.cxx ${Application_SRCS})
target_link_libraries(otbApplicationTests Monteverdi2_Application OTBTesting)
