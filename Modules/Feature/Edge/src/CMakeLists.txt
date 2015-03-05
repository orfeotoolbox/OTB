set(OTBEdge_SRC
  otbFillGapsFilter.cxx
  )

add_library(OTBEdge ${OTBEdge_SRC})
target_link_libraries(OTBEdge
  ${OTBCommon_LIBRARIES}
  ${OTBConversion_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBITKPendingPatches_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBPath_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBSpatialObjects_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBVectorDataBase_LIBRARIES}
)

otb_module_target(OTBEdge)
