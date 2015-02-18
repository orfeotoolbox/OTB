set(OTBAppEdge_LINK_LIBS
  ${OTBStatistics_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBEdge_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
)

otb_create_application(
  NAME           LineSegmentDetection
  SOURCES        otbLineSegmentDetection.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           EdgeExtraction
  SOURCES        otbEdgeExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
