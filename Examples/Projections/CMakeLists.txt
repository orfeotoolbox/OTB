project(ProjectionsExamples)

add_executable(CoordinateToNameExample CoordinateToNameExample.cxx)
target_link_libraries(CoordinateToNameExample ${OTB_LIBRARIES})

add_executable(EstimateRPCSensorModelExample EstimateRPCSensorModelExample.cxx)
target_link_libraries(EstimateRPCSensorModelExample ${OTB_LIBRARIES})

add_executable(GeometriesProjectionExample GeometriesProjectionExample.cxx)
target_link_libraries(GeometriesProjectionExample ${OTB_LIBRARIES})

add_executable(MapProjectionExample MapProjectionExample.cxx)
target_link_libraries(MapProjectionExample ${OTB_LIBRARIES})

add_executable(OrthoRectificationExample OrthoRectificationExample.cxx)
target_link_libraries(OrthoRectificationExample ${OTB_LIBRARIES})

add_executable(PlaceNameToLonLatExample PlaceNameToLonLatExample.cxx)
target_link_libraries(PlaceNameToLonLatExample ${OTB_LIBRARIES})

add_executable(SensorModelExample SensorModelExample.cxx)
target_link_libraries(SensorModelExample ${OTB_LIBRARIES})

add_executable(VectorDataExtractROIExample VectorDataExtractROIExample.cxx)
target_link_libraries(VectorDataExtractROIExample ${OTB_LIBRARIES})

add_executable(VectorDataProjectionExample VectorDataProjectionExample.cxx)
target_link_libraries(VectorDataProjectionExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
