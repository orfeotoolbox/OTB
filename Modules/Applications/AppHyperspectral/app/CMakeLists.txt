set(OTBAppHyperspectral_LINK_LIBS
  ${OTBStatistics_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBEndmembersExtraction_LIBRARIES}
  ${OTBUnmixing_LIBRARIES}
)

otb_create_application(
  NAME           HyperspectralUnmixing
  SOURCES        otbHyperspectralUnmixing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VertexComponentAnalysis
  SOURCES        otbVertexComponentAnalysis.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
