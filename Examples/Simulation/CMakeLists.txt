project(SimulationExamples)

#OTBSimulation -> OTBOpticalCalibration -> OTB6S
if(OTB6S_LOADED)
add_executable(LAIAndPROSAILToSensorResponse LAIAndPROSAILToSensorResponse.cxx)
target_link_libraries(LAIAndPROSAILToSensorResponse ${OTB_LIBRARIES})

add_executable(ProsailModel ProsailModel.cxx)
target_link_libraries(ProsailModel ${OTB_LIBRARIES})
endif()

add_executable(LAIFromNDVIImageTransform LAIFromNDVIImageTransform.cxx)
target_link_libraries(LAIFromNDVIImageTransform ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
