project(PatentedExamples)

add_executable(EstimateAffineTransformationExample EstimateAffineTransformationExample.cxx)
target_link_libraries(EstimateAffineTransformationExample ${OTB_LIBRARIES})

add_executable(SIFTDisparityMapEstimation SIFTDisparityMapEstimation.cxx)
target_link_libraries(SIFTDisparityMapEstimation ${OTB_LIBRARIES})

add_executable(SIFTExample SIFTExample.cxx)
target_link_libraries(SIFTExample ${OTB_LIBRARIES})

if (Module_OTBSiftFast)
  add_executable(SIFTDensityExample SIFTDensityExample.cxx)
  target_link_libraries(SIFTDensityExample ${OTB_LIBRARIES})

  add_executable(SIFTFastExample SIFTFastExample.cxx)
  target_link_libraries(SIFTFastExample ${OTB_LIBRARIES})
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
