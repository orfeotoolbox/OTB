project(FeatureExtractionExamples)

add_executable(AlignmentsExample AlignmentsExample.cxx)
target_link_libraries(AlignmentsExample ${OTB_LIBRARIES})

add_executable(AssymmetricFusionOfLineDetectorExample AssymmetricFusionOfLineDetectorExample.cxx)
target_link_libraries(AssymmetricFusionOfLineDetectorExample ${OTB_LIBRARIES})

add_executable(CloudDetectionExample CloudDetectionExample.cxx)
target_link_libraries(CloudDetectionExample ${OTB_LIBRARIES})

add_executable(ComplexMomentPathExample ComplexMomentPathExample.cxx)
target_link_libraries(ComplexMomentPathExample ${OTB_LIBRARIES})

add_executable(ComplexMomentsImageFunctionExample ComplexMomentsImageFunctionExample.cxx)
target_link_libraries(ComplexMomentsImageFunctionExample ${OTB_LIBRARIES})

add_executable(CorrelationLineDetectorExample CorrelationLineDetectorExample.cxx)
target_link_libraries(CorrelationLineDetectorExample ${OTB_LIBRARIES})

add_executable(EdgeDensityExample EdgeDensityExample.cxx)
target_link_libraries(EdgeDensityExample ${OTB_LIBRARIES})

add_executable(ExtractRoadByStepsExample ExtractRoadByStepsExample.cxx)
target_link_libraries(ExtractRoadByStepsExample ${OTB_LIBRARIES})

add_executable(ExtractRoadExample ExtractRoadExample.cxx)
target_link_libraries(ExtractRoadExample ${OTB_LIBRARIES})

add_executable(ExtractSegmentsByStepsExample ExtractSegmentsByStepsExample.cxx)
target_link_libraries(ExtractSegmentsByStepsExample ${OTB_LIBRARIES})

add_executable(ExtractSegmentsExample ExtractSegmentsExample.cxx)
target_link_libraries(ExtractSegmentsExample ${OTB_LIBRARIES})

add_executable(FlusserMomentsImageFunctionExample FlusserMomentsImageFunctionExample.cxx)
target_link_libraries(FlusserMomentsImageFunctionExample ${OTB_LIBRARIES})

add_executable(HarrisExample HarrisExample.cxx)
target_link_libraries(HarrisExample ${OTB_LIBRARIES})

add_executable(HuMomentsImageFunctionExample HuMomentsImageFunctionExample.cxx)
target_link_libraries(HuMomentsImageFunctionExample ${OTB_LIBRARIES})

if (Module_OTBVectorDataRendering)
  add_executable(LineSegmentDetectorExample LineSegmentDetectorExample.cxx)
  target_link_libraries(LineSegmentDetectorExample ${OTB_LIBRARIES})
endif()

add_executable(LocalHoughExample LocalHoughExample.cxx)
target_link_libraries(LocalHoughExample ${OTB_LIBRARIES})

add_executable(PanTexExample PanTexExample.cxx)
target_link_libraries(PanTexExample ${OTB_LIBRARIES})

add_executable(ParallelLineDetectionExample ParallelLineDetectionExample.cxx)
target_link_libraries(ParallelLineDetectionExample ${OTB_LIBRARIES})

add_executable(RatioLineDetectorExample RatioLineDetectorExample.cxx)
target_link_libraries(RatioLineDetectorExample ${OTB_LIBRARIES})

if (Module_OTBVectorDataRendering)
  add_executable(RightAngleDetectionExample RightAngleDetectionExample.cxx)
  target_link_libraries(RightAngleDetectionExample ${OTB_LIBRARIES})
endif()

add_executable(SeamCarvingExample SeamCarvingExample.cxx)
target_link_libraries(SeamCarvingExample ${OTB_LIBRARIES})

add_executable(SeamCarvingOtherExample SeamCarvingOtherExample.cxx)
target_link_libraries(SeamCarvingOtherExample ${OTB_LIBRARIES})

add_executable(SFSExample SFSExample.cxx)
target_link_libraries(SFSExample ${OTB_LIBRARIES})

add_executable(SURFExample SURFExample.cxx)
target_link_libraries(SURFExample ${OTB_LIBRARIES})

add_executable(TextureExample TextureExample.cxx)
target_link_libraries(TextureExample ${OTB_LIBRARIES})

add_executable(ThresholdToPointSetExample ThresholdToPointSetExample.cxx)
target_link_libraries(ThresholdToPointSetExample ${OTB_LIBRARIES})

add_executable(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx)
target_link_libraries(TouziEdgeDetectorExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
