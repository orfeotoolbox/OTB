set(OTBAppTextures_LINK_LIBS
  ${OTBTextures_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
)

otb_create_application(
  NAME           HaralickTextureExtraction
  SOURCES        otbHaralickTextureExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SFSTextureExtraction
  SOURCES        otbSFSTextureExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
