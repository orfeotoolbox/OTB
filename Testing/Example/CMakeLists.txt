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


# ---------------- CopyInput example test
set(OTB_WRAPPER_CopyInput_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_CopyInput)
add_test(exTuCopyInputNew ${OTB_WRAPPER_CopyInput_TESTS}
	otbCopyInputNew
  )

add_test(exTvCopyInputTest ${OTB_WRAPPER_CopyInput_TESTS}
    otbCopyInputTest1
    ${INPUTDATA}/poupees.tif
    ${TEMP}/owTvCopyInputTest.tif
  )

include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Example)
add_executable(otbWrapperExampleTests_CopyInput otbWrapperExampleTests_CopyInput.cxx otbCopyInputTest.cxx)
target_link_libraries(otbWrapperExampleTests_CopyInput OTBIO OTBCommon OTBTesting OTBWrapperCore)


# ---------------- AllocateOutput example test
set(OTB_WRAPPER_AllocateOutput_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_AllocateOutput)
add_test(exTuAllocateOutputNew ${OTB_WRAPPER_AllocateOutput_TESTS}
	otbAllocateOutputNew
  )

add_test(exTvAllocateOutputTest ${OTB_WRAPPER_AllocateOutput_TESTS}
    otbAllocateOutputTest1
    ${TEMP}/owTvAllocateOutputTest.tif
  )

include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Example)
add_executable(otbWrapperExampleTests_AllocateOutput otbWrapperExampleTests_AllocateOutput.cxx otbAllocateOutputTest.cxx)
target_link_libraries(otbWrapperExampleTests_AllocateOutput OTBIO OTBCommon OTBTesting OTBWrapperCore)


# ---------------- InternalReader example test
set(OTB_WRAPPER_InternalReader_TESTS ${CXX_TEST_PATH}/otbWrapperExampleTests_InternalReader)
add_test(exTuInternalReaderNew ${OTB_WRAPPER_InternalReader_TESTS}
	otbInternalReaderNew
  )

add_test(exTvInternalReaderTest ${OTB_WRAPPER_InternalReader_TESTS}
    otbInternalReaderTest1
    ${INPUTDATA}/poupees.tif
    ${TEMP}/owTvInternalReaderTest.tif
  )

include_directories(${CMAKE_SOURCE_DIR}/Code/Core ${CMAKE_SOURCE_DIR}/Example)
add_executable(otbWrapperExampleTests_InternalReader otbWrapperExampleTests_InternalReader.cxx otbInternalReaderTest.cxx)
target_link_libraries(otbWrapperExampleTests_InternalReader OTBIO OTBCommon OTBTesting OTBWrapperCore)
