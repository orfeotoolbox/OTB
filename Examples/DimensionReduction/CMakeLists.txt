project(DimensionReductionExamples)

add_executable(ICAExample ICAExample.cxx)
target_link_libraries(ICAExample ${OTB_LIBRARIES})

add_executable(MaximumAutocorrelationFactor MaximumAutocorrelationFactor.cxx)
target_link_libraries(MaximumAutocorrelationFactor ${OTB_LIBRARIES})

add_executable(MNFExample MNFExample.cxx)
target_link_libraries(MNFExample ${OTB_LIBRARIES})

add_executable(NAPCAExample NAPCAExample.cxx)
target_link_libraries(NAPCAExample ${OTB_LIBRARIES})

add_executable(PCAExample PCAExample.cxx)
target_link_libraries(PCAExample ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
