otb_module_test()

set(OTBQtWidgetTests
otbQtWidgetTestDriver.cxx
otbWrapperQtWidgetShowWidget.cxx
otbQtLogOutputTests.cxx
otbWrapperQtWidgetParameterFactory.cxx
)

add_executable(otbQtWidgetTestDriver ${OTBQtWidgetTests})
target_link_libraries(otbQtWidgetTestDriver ${OTBQtWidget-Test_LIBRARIES})
otb_module_target_label(otbQtWidgetTestDriver)

# Tests Declaration

otb_add_test(NAME owTvQtLogOutputNew COMMAND otbQtWidgetTestDriver
  otbQtLogOutputNew)

otb_add_test(NAME owTvQtWidgetParameterGroup COMMAND otbQtWidgetTestDriver
  otbWrapperQtWidgetParameterGroup)

otb_add_test(NAME owTvQtWidgetParameterFactory COMMAND otbQtWidgetTestDriver
  otbWrapperQtWidgetParameterFactory)

