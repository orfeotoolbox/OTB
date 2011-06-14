FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTXML 1)
INCLUDE(${QT_USE_FILE})

SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

SET(OTB_WRAPPER_QTWIDGET_TESTS ${CXX_TEST_PATH}/otbWrapperQtWidgetTests)

SET(WrapperQtWidget_SRCS
    otbWrapperQtWidgetTests.cxx
    otbWrapperQtWidgetParameterFactory.cxx)

INCLUDE_DIRECTORIES(${QT_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Code/Core)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Code/Wrappers/QtWidget)

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
