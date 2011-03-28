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
  
  
# NumericalParameter class test
ADD_TEST(owTuNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterNew
  ) 
  
# EmptyParameter class test
ADD_TEST(owTuEmptyParameter ${OTB_WRAPPER_TESTS}
	otbWrapperEmptyParameterNew
  )
  
# NumericalParameter class test
ADD_TEST(owTuNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterNew
  )
# ----------------Source files CXX -----------------------------------

SET(Wrapper_SRCS
otbWrapperTests.cxx
otbWrapperParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperEmptyParameterTest.cxx
#otbWrapperApplicationTest.cxx
otbWrapperParameterListTest.cxx
)

ADD_EXECUTABLE(otbWrapperTests ${Wrapper_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting)

