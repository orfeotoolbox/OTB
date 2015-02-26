set(OTBSpatialObjects_SRC
  )

add_library(OTBSpatialObjects ${OTBSpatialObjects_SRC})
target_link_libraries(OTBSpatialObjects 
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBSpatialObjects)
