set(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${Monteverdi_BINARY_DIR}/Testing/Temporary)

# Large Input
if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(Common_TESTS ${CXX_TEST_PATH}/otbCommonTests)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  
  ${Monteverdi2_SOURCE_DIR}/Code/Application/Viewer
  
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Common
  )

#-------------- Monteverdi Common Tests ---------------------------------
add_test(apTvMonteverdi2ApplicationsToolBox ${Common_TESTS} otbApplicationsToolBoxTest)


set(Common_SRCS
otbApplicationsToolBoxTest.cxx
)

add_executable(otbCommonTests otbCommonTests.cxx ${Common_SRCS})
target_link_libraries(otbCommonTests Monteverdi2_Common OTBTesting)
