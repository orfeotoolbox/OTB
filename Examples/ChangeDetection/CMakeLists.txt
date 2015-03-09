project(ChangeDetectionExamples)

add_executable(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx)
target_link_libraries(ChangeDetectionFrameworkExample ${OTB_LIBRARIES})

add_executable(CorrelChDet CorrelChDet.cxx)
target_link_libraries(CorrelChDet ${OTB_LIBRARIES})

add_executable(DiffChDet DiffChDet.cxx)
target_link_libraries(DiffChDet ${OTB_LIBRARIES})

add_executable(JHMIChDet JHMIChDet.cxx)
target_link_libraries(JHMIChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerDistanceChDet KullbackLeiblerDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerDistanceChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerProfileChDet KullbackLeiblerProfileChDet.cxx)
target_link_libraries(KullbackLeiblerProfileChDet ${OTB_LIBRARIES})

add_executable(KullbackLeiblerSupervizedDistanceChDet KullbackLeiblerSupervizedDistanceChDet.cxx)
target_link_libraries(KullbackLeiblerSupervizedDistanceChDet ${OTB_LIBRARIES})

add_executable(LHMIChDet LHMIChDet.cxx)
target_link_libraries(LHMIChDet ${OTB_LIBRARIES})

add_executable(MultivariateAlterationDetector MultivariateAlterationDetector.cxx)
target_link_libraries(MultivariateAlterationDetector ${OTB_LIBRARIES})

add_executable(RatioChDet RatioChDet.cxx)
target_link_libraries(RatioChDet ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
