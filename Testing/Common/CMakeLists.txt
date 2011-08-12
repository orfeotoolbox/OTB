set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(INPUTDATA ${OTB_DATA_ROOT}/Input)
set(TEMP ${OTB-Wrapper_BINARY_DIR}/Testing/Temporary)

if(WIN32)
  add_definitions(-DWIN32)
endif(WIN32)

set(OTB_WRAPPER_COMMON_TESTS ${CXX_TEST_PATH}/otbWrapperCommonTests)


add_test(coTuWrapperEventsSenderNew ${OTB_WRAPPER_COMMON_TESTS}
	otbWrapperEventsSenderNew
  )

add_test(coTuWrapperEventsListenerNew ${OTB_WRAPPER_COMMON_TESTS}
	otbWrapperEventsListenerNew
  )

add_test(coTuWrapperEventNew ${OTB_WRAPPER_COMMON_TESTS}
	otbWrapperEventNew
  )



# ----------------Source files CXX -----------------------------------

set(Wrapper_Common_SRCS
otbWrapperCommonTests.cxx

otbWrapperEventClassesTests.cxx

)

include_directories(${CMAKE_SOURCE_DIR}/Code/Common)
add_executable(otbWrapperCommonTests ${Wrapper_Common_SRCS})
target_link_libraries(otbWrapperCommonTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCommon)
