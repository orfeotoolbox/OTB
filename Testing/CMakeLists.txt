SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

IF(WIN32)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(WIN32)

SET(OTB_WRAPPER_TESTS ${CXX_TEST_PATH}/otbWrapperTests)
SET(OTB_WRAPPER_QT_TESTS ${CXX_TEST_PATH}/otbWrapperQtWidgetTests)

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
    
# Application class test
ADD_TEST(owTuApplication ${OTB_WRAPPER_TESTS}
	otbWrapperApplicationNew
  )
  
IF(OTB_USE_QT)
ADD_TEST(owTvQtWidgetParameterFactory ${OTB_WRAPPER_QT_TESTS}
        otbWrapperQtWidgetParameterFactory
)

ENDIF(OTB_USE_QT)

# ----------------Source files CXX -----------------------------------

SET(Wrapper_SRCS
otbWrapperTests.cxx
otbWrapperParameterTest.cxx
otbWrapperNumericalParameterTest.cxx
otbWrapperEmptyParameterTest.cxx
otbWrapperApplicationTest.cxx
otbWrapperParameterListTest.cxx
otbWrapperInputImageParameterTest.cxx
)

ADD_EXECUTABLE(otbWrapperTests ${Wrapper_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCore)

IF(OTB_USE_QT)

SET(WrapperQtWidget_SRCS
otbWrapperQtWidgetTests.cxx
otbWrapperQtWidgetParameterFactory.cxx
)

ADD_EXECUTABLE(otbWrapperQtWidgetTests ${WrapperQtWidget_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperQtWidgetTests OTBIO OTBCommon ITKIO ITKCommon OTBTesting OTBWrapperCore OTBWrapperQtWidget)

ENDIF(OTB_USE_QT)
