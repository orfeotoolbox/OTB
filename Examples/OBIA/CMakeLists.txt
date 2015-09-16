project(OBIAExamples)

add_executable(HooverMetricsEstimation HooverMetricsEstimation.cxx)
target_link_libraries(HooverMetricsEstimation ${OTB_LIBRARIES})

add_executable(ImageToLabelToImage ImageToLabelToImage.cxx)
target_link_libraries(ImageToLabelToImage ${OTB_LIBRARIES})

add_executable(KeepNObjects KeepNObjects.cxx)
target_link_libraries(KeepNObjects ${OTB_LIBRARIES})

add_executable(LabelMapToVectorData LabelMapToVectorData.cxx)
target_link_libraries(LabelMapToVectorData ${OTB_LIBRARIES})

if(OTBMeanShift_LOADED)
add_executable(RadiometricAttributesLabelMapFilterExample RadiometricAttributesLabelMapFilterExample.cxx)
target_link_libraries(RadiometricAttributesLabelMapFilterExample ${OTB_LIBRARIES})
endif()

add_executable(ShapeAttributeComputation ShapeAttributeComputation.cxx)
target_link_libraries(ShapeAttributeComputation ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
