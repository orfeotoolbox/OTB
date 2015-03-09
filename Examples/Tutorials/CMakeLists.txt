project(TutorialsExamples)

add_executable(FilteringPipeline FilteringPipeline.cxx)
target_link_libraries(FilteringPipeline ${OTB_LIBRARIES})

add_executable(HelloWorldOTB HelloWorldOTB.cxx)
target_link_libraries(HelloWorldOTB ${OTB_LIBRARIES})

add_executable(Multispectral Multispectral.cxx)
target_link_libraries(Multispectral ${OTB_LIBRARIES})

add_executable(OrthoFusion OrthoFusion.cxx)
target_link_libraries(OrthoFusion ${OTB_LIBRARIES})

add_executable(Pipeline Pipeline.cxx)
target_link_libraries(Pipeline ${OTB_LIBRARIES})

add_executable(ScalingPipeline ScalingPipeline.cxx)
target_link_libraries(ScalingPipeline ${OTB_LIBRARIES})

add_executable(SmarterFilteringPipeline SmarterFilteringPipeline.cxx)
target_link_libraries(SmarterFilteringPipeline ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
