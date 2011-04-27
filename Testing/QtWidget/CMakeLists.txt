SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

IF(OTB_USE_QT)

SET(OTB_WRAPPER_QTWIDGET_TESTS ${CXX_TEST_PATH}/otbWrapperQtWidgetTests)

SET(WrapperQtWidget_SRCS
    otbWrapperQtWidgetTests.cxx
    otbWrapperQtWidgetParameterFactory.cxx)

ADD_EXECUTABLE(otbWrapperQtWidgetTests ${WrapperQtWidget_SRCS})
TARGET_LINK_LIBRARIES(otbWrapperQtWidgetTests
                        OTBIO
                        OTBCommon
                        OTBTesting
                        OTBWrapperCore
                        OTBWrapperQtWidget)

ADD_TEST(owTvQtWidgetParameterFactory ${OTB_WRAPPER_QTWIDGET_TESTS}
        otbWrapperQtWidgetParameterFactory)

ADD_TEST(owTvQtWidgetParameterGroup ${OTB_WRAPPER_QTWIDGET_TESTS}
        otbWrapperQtWidgetParameterGroup)

ENDIF(OTB_USE_QT)
