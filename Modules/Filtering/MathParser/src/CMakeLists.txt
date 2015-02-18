set(OTBMathParser_SRC
  otbParserXPlugins.cxx
  otbParser.cxx
  otbParserX.cxx
  )

add_library(OTBMathParser ${OTBMathParser_SRC})
target_link_libraries(OTBMathParser 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBMuParser_LIBRARIES}
  ${OTBMuParserX_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}

  )

otb_module_target(OTBMathParser)
