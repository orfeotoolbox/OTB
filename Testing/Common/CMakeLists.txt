set(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${Monteverdi_BINARY_DIR}/Testing/Temporary)

# Large Input
if(OTB_DATA_USE_LARGEINPUT)
  set(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
endif(OTB_DATA_USE_LARGEINPUT)

set(Common_TESTS ${CXX_TEST_PATH}/mvdCommonTests)
set(Common_TESTS1 ${CXX_TEST_PATH}/mvdCommonTests1)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  ${Monteverdi2_SOURCE_DIR}/Code/Application/Viewer
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Common/
  )

# add source files
set(Common_SRCS mvdApplicationsToolBoxTest.cxx )

if (OTB_USE_QT)
  set(Common_SRCS1
    mvdApplicationsBrowserTest.cxx
    mvdFillToolBoxWidgetTreeTest.cxx
    )
endif()
#-------------- Monteverdi Common Tests ---------------------------------

# Applications toolBox test 
add_test(apTvMonteverdi2ApplicationsToolBox ${Common_TESTS} mvdApplicationsToolBoxTest)

# test when OTB_USE_QT is set to ON
if (OTB_USE_QT)

  # Applications browser test
  add_test(apTvMonteverdi2ApplicationsBrowser ${Common_TESTS1} 
    mvdApplicationsBrowserTest
    ${OTB_DIR}/bin
    )
  
  # Fill ToolBox algorithms tree
  add_test(apTvMonteverdi2FillToolBoxTree ${Common_TESTS1}
    mvdFillToolBoxWidgetTreeTest
    ${OTB_DIR}/bin
    )

endif()

#-------------- Link ---------------------------------

add_executable(mvdCommonTests mvdCommonTests.cxx ${Common_SRCS})
target_link_libraries(mvdCommonTests 
  Monteverdi2_Gui 
  Monteverdi2_Core 
  OTBTesting)

if (OTB_USE_QT)
  add_executable(mvdCommonTests1 mvdCommonTests1.cxx ${Common_SRCS1})
  target_link_libraries(mvdCommonTests1 
    Monteverdi2_Core
    Monteverdi2_Gui
    Monteverdi2_ApplicationsWrapper
    OTBTesting)
endif()



