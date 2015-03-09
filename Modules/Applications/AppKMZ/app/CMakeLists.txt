set(OTBAppImageUtils_LINK_LIBS
  ${OTBKMZWriter_LIBRARIES}
)

otb_create_application(
  NAME           KmzExport
  SOURCES        otbKmzExport.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
