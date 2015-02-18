set(OTBAppVectorUtils_LINK_LIBS
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBVectorDataManipulation_LIBRARIES}
  ${OTBCarto_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           VectorDataTransform
  SOURCES        otbVectorDataTransform.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataExtractROI
  SOURCES        otbVectorDataExtractROI.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           VectorDataSetField
  SOURCES        otbVectorDataSetField.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           OSMDownloader
  SOURCES        otbOSMDownloader.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConcatenateVectorData
  SOURCES        otbConcatenateVectorData.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
