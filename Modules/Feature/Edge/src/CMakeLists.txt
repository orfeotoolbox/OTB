set(OTBEdge_SRC
  otbFillGapsFilter.cxx
  )

add_library(OTBEdge ${OTBEdge_SRC})
target_link_libraries(OTBEdge
# Fill with libraries from DEPENDS list)

otb_module_target(OTBEdge)
