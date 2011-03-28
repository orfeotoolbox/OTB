ADD_TEST(owTuParameterNew ${OTB_WRAPPER_TESTS}
	otbWrapperParameterNew
  )

ADD_TEST(owTvParameter ${OTB_WRAPPER_TESTS}
	otbWrapperParameterTest1
  )
# -------       Source files CXX -----------------------------------

SET(Wrapper_SRCS
otbWrapperTests.cxx
otbWrapperParameterTest.cxx
)
ADD_EXECUTABLE(otbWrapperTests "${Wrapper_SRCS}")
TARGET_LINK_LIBRARIES(otbWrapperTests OTBIO OTBCommon  ITKIO ITKCommon OTBTesting)

