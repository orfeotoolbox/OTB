set(OTBAppTest_LINK_LIBS
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           TestApplication
  SOURCES        otbTestApplication.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
