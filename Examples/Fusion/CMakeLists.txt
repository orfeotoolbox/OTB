project(FusionExamples)

add_executable(BayesianFusionImageFilter BayesianFusionImageFilter.cxx)
target_link_libraries(BayesianFusionImageFilter ${OTB_LIBRARIES})

add_executable(PanSharpeningExample PanSharpeningExample.cxx)
target_link_libraries(PanSharpeningExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
