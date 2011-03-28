SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(OTB_WRAPPER_TESTS ${CXX_TEST_PATH}/otbWrapperTests)

ADD_TEST(owTuParameterNew ${OTB_WRAPPER_TESTS}
	otbWrapperParameterNew
  )

ADD_TEST(owTvParameter ${OTB_WRAPPER_TESTS}
	otbWrapperParameterTest1
	"param1"
  )
  
# -------       Source files CXX -----------------------------------

SET(Wrapper_SRCS
otbWrapperTests.cxx
otbWrapperParameterTest.cxx
)

ADD_EXECUTABLE(otbWrapperTests ${Wrapper_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperTests OTBIO OTBCommon  ITKIO ITKCommon OTBTesting)

