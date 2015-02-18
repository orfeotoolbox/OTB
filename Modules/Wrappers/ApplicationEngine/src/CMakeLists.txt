set(OTBApplicationEngine_SRC
  otbWrapperApplicationHtmlDocGenerator.cxx
  otbWrapperComplexOutputImageParameter.cxx
  otbWrapperInputVectorDataListParameter.cxx
  otbWrapperListViewParameter.cxx
  otbWrapperComplexInputImageParameter.cxx
  otbWrapperParameterGroup.cxx
  otbWrapperOutputProcessXMLParameter.cxx
  otbWrapperInputImageListParameter.cxx
  otbWrapperElevationParametersHandler.cxx
  otbWrapperInputFilenameListParameter.cxx
  otbWrapperOutputImageParameter.cxx
  otbWrapperInputImageParameter.cxx
  otbWrapperParameterKey.cxx
  otbWrapperDocExampleStructure.cxx
  otbWrapperInputVectorDataParameter.cxx
  otbWrapperInputProcessXMLParameter.cxx
  otbWrapperMapProjectionParametersHandler.cxx
  otbWrapperApplication.cxx
  otbWrapperChoiceParameter.cxx
  otbWrapperApplicationRegistry.cxx
  )

add_library(OTBApplicationEngine ${OTBApplicationEngine_SRC})
target_link_libraries(OTBApplicationEngine 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBApplicationEngine)
