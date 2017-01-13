set(OTBAppMorphology_LINK_LIBS
  ${OTBImageBase_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBMorphologicalProfiles_LIBRARIES}
)

otb_create_application(
  NAME           BinaryMorphologicalOperation
  SOURCES        otbBinaryMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           GrayScaleMorphologicalOperation
  SOURCES        otbGrayScaleMorphologicalOperation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})


otb_create_application(
  NAME           MorphologicalProfilesClassification
  SOURCES        otbMorphologicalProfilesClassification.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MorphologicalProfilesAnalysis
  SOURCES        otbMorphologicalProfilesAnalysis.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MorphologicalMultiScaleDecomposition
  SOURCES        otbMorphologicalMultiScaleDecomposition.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
