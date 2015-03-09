set(OTBAppIndices_LINK_LIBS
  ${OTBIndices_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
)

otb_create_application(
  NAME           RadiometricIndices
  SOURCES        otbRadiometricIndices.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
