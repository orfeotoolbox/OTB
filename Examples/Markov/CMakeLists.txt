project(MarkovExamples)

add_executable(MarkovClassification1Example MarkovClassification1Example.cxx)
target_link_libraries(MarkovClassification1Example ${OTB_LIBRARIES})

add_executable(MarkovClassification2Example MarkovClassification2Example.cxx)
target_link_libraries(MarkovClassification2Example ${OTB_LIBRARIES})

add_executable(MarkovClassification3Example MarkovClassification3Example.cxx)
target_link_libraries(MarkovClassification3Example ${OTB_LIBRARIES})

add_executable(MarkovRegularizationExample MarkovRegularizationExample.cxx)
target_link_libraries(MarkovRegularizationExample ${OTB_LIBRARIES})

add_executable(MarkovRestaurationExample MarkovRestaurationExample.cxx)
target_link_libraries(MarkovRestaurationExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
