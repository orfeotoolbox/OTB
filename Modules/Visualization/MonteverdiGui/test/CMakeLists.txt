otb_module_test()

set(OTBMonteverdiGUITests
mvdApplicationsBrowserTest.cxx
mvdApplicationsToolBoxTest.cxx
mvdFillToolBoxWidgetTreeTest.cxx
mvdMonteverdiGuiTestDriver.cxx
)

add_executable(mvdMonteverdiGuiTestDriver ${OTBMonteverdiGUITests})
target_link_libraries(mvdMonteverdiGuiTestDriver ${OTBMonteverdiGUI-Test_LIBRARIES})
otb_module_target_label(mvdMonteverdiGuiTestDriver)

# Tests Declaration
# TODO
