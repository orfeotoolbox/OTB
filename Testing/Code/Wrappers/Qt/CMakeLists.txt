find_package(Qt4 REQUIRED)
set(QT_USE_QTXML 1)
include(${QT_USE_FILE})

set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

set(OTB_WRAPPER_QTWIDGET_TESTS ${CXX_TEST_PATH}/otbWrapperQtWidgetTests)

set(WrapperQtWidget_SRCS
    otbWrapperQtWidgetTests.cxx
    otbWrapperQtWidgetParameterFactory.cxx
    otbWrapperQtWidgetShowWidget.cxx
    otbQtLogOutputTests.cxx
    )

include_directories(${QT_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/Code/Core)
include_directories(${CMAKE_SOURCE_DIR}/Code/Common)
include_directories(${CMAKE_SOURCE_DIR}/Code/Wrappers/QtWidget)

add_executable(otbWrapperQtWidgetTests ${WrapperQtWidget_SRCS})
target_link_libraries(otbWrapperQtWidgetTests
                        OTBIO
                        OTBCommon
                        OTBTesting
                        OTBApplicationEngine
                        OTBWrapperQtWidget)

add_test(owTvQtWidgetParameterFactory ${OTB_WRAPPER_QTWIDGET_TESTS}
        otbWrapperQtWidgetParameterFactory)

add_test(owTvQtWidgetParameterGroup ${OTB_WRAPPER_QTWIDGET_TESTS}
        otbWrapperQtWidgetParameterGroup)


foreach(app ${OTB_APPLICATIONS_NAME_LIST})

add_test(NAME owTvQtWidgetShow_${app}
         COMMAND ${OTB_WRAPPER_QTWIDGET_TESTS}
           otbWrapperQtWidgetShowWidget
           ${app}
           $<TARGET_FILE_DIR:otbapp_${app}> )

endforeach(app)

add_test(owTvQtLogOutputNew ${OTB_WRAPPER_QTWIDGET_TESTS}
        otbQtLogOutputNew)
