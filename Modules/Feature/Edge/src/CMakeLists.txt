set(OTBEdge_SRC
  otbFillGapsFilter.cxx
  )

add_library(OTBEdge ${OTBEdge_SRC})
target_link_libraries(OTBEdge
  ${OTBSpatialObjects_LIBRARIES}
)

otb_module_target(OTBEdge)
