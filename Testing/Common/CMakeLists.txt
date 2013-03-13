set(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${Monteverdi_BINARY_DIR}/Testing/Temporary)

# Large Input
if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(Common_TESTS ${CXX_TEST_PATH}/mvdCommonTests)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  
  ${Monteverdi2_SOURCE_DIR}/Code/Application/Viewer
  
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Common
  )

# add source files
set(Common_SRCS
  mvdApplicationsToolBoxTest.cxx
  mvdApplicationsBrowserTest.cxx
  mvdFillToolBoxWidgetTreeTest.cxx
)

#-------------- Monteverdi Common Tests ---------------------------------

# Applications toolBox test 
add_test(apTvMonteverdi2ApplicationsToolBox ${Common_TESTS} mvdApplicationsToolBoxTest)

# Applications browser test
add_test(apTvMonteverdi2ApplicationsBrowser ${Common_TESTS} 
  mvdApplicationsBrowserTest
  ${OTB_DIR}/bin
  )

# Fill ToolBox algorithms tree
add_test(apTvMonteverdi2FillToolBoxTree ${Common_TESTS} 
  mvdFillToolBoxWidgetTreeTest
  ${OTB_DIR}/bin
  )


add_executable(mvdCommonTests mvdCommonTests.cxx ${Common_SRCS})
target_link_libraries(mvdCommonTests Monteverdi2_Common OTBTesting)




