otb_module_test()

set(OTBAnomalyDetectionTests
otbAnomalyDetectionTestDriver.cxx
otbLocalRxDetectorRoiTest.cxx
otbLocalRxDetectorTest.cxx
)

add_executable(otbAnomalyDetectionTestDriver ${OTBAnomalyDetectionTests})
target_link_libraries(otbAnomalyDetectionTestDriver ${OTBAnomalyDetection-Test_LIBRARIES})
otb_module_target_label(otbAnomalyDetectionTestDriver)

# Tests Declaration

