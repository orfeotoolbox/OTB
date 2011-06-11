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

ADD_TEST(owTvNumericalParameter ${OTB_WRAPPER_TESTS}
	otbWrapperNumericalParameterTest1
	42.42
	"mykey"
	"my description"
  )

# Input Image parameter class test
ADD_TEST(owTuInputImageParameter ${OTB_WRAPPER_TESTS}
	otbWrapperInputImageParameterNew
  )
    
# Input VectorData parameter class test
ADD_TEST(owTuInputVectorDataParameter ${OTB_WRAPPER_TESTS}
    otbWrapperInputVectorDataParameterNew
  )

# Application class test
ADD_TEST(owTuApplication ${OTB_WRAPPER_TESTS}
	otbWrapperApplicationNew
  )

# Application class test
ADD_TEST(owTvApplicationFactory ${OTB_WRAPPER_TESTS}
    otbWrapperApplicationFactory
  )

# ----------------Source files CXX -----------------------------------

SET(Wrapper_SRCS
otbWrapperCoreTests.cxx

otbWrapperApplicationTest.cxx
otbWrapperChoiceParameterTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperInputImageParameterTest.cxx
otbWrapperInputVectorDataParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperOutputImageParameterTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperParameterTest.cxx
otbWrapperApplicationRegistryTest.cxx
)

ADD_EXECUTABLE(otbWrapperTests ${Wrapper_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCore)
