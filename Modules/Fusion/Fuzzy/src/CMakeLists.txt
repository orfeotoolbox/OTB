set(OTBFuzzy_SRC
  otbFuzzyDescriptorsModelManager.cxx
  )

add_library(OTBFuzzy ${OTBFuzzy_SRC})
target_link_libraries(OTBFuzzy 
  ${OTBCommon_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}

  )

otb_module_target(OTBFuzzy)
