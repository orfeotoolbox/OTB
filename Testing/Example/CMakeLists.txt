set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB-Wrapper_BINARY_DIR}/Testing/Temporary)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)


# ---------------- Smoothing example test
set(OTB_WRAPPER_Smoothing_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_Smoothing)
add_test(exTuSmoothingNew ${OTB_WRAPPER_Smoothing_TESTS}
	otbSmoothingNew
  )

add_test(exTvSmoothingTest ${OTB_WRAPPER_Smoothing_TESTS}
    otbSmoothingTest1
    ${INPUTDATA}/poupees.tif
    ${TEMP}/owTvSmoothingTest.tif
    5
  )
include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Example)
add_executable(otbWrapperExampleTests_Smoothing otbWrapperExampleTests_Smoothing.cxx otbSmoothingTest.cxx)
target_link_libraries(otbWrapperExampleTests_Smoothing OTBIO OTBCommon OTBTesting OTBWrapperCore)


# ---------------- Rescale example test
set(OTB_WRAPPER_Rescale_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_Rescale)
add_test(exTuRescaleNew ${OTB_WRAPPER_Rescale_TESTS}
	otbRescaleNew
  )

add_test(exTvRescaleTest ${OTB_WRAPPER_Rescale_TESTS}
    otbRescaleTest1
    ${INPUTDATA}/poupees.tif
    ${TEMP}/owTvRescaleTest.tif
    20
    150
  )

include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Example)
add_executable(otbWrapperExampleTests_Rescale otbWrapperExampleTests_Rescale.cxx otbRescaleTest.cxx)
target_link_libraries(otbWrapperExampleTests_Rescale OTBIO OTBCommon OTBTesting OTBWrapperCore)
