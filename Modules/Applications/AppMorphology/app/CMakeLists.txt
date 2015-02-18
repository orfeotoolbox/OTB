set(OTBAppMorphology_LINK_LIBS
  ${OTBImageBase_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
)

otb_create_application(
  NAME           BinaryMorphologicalOperation
  SOURCES        otbBinaryMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           GrayScaleMorphologicalOperation
  SOURCES        otbGrayScaleMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
