set(OTBAppFiltering_LINK_LIBS
  ${OTBPolarimetry_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
)

otb_create_application(
  NAME           SARDecompositions
  SOURCES        otbSARDecompositions.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

