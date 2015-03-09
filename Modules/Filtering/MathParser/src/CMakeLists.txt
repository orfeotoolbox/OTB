set(OTBMathParser_SRC
  otbParser.cxx
  )

add_library(OTBMathParser ${OTBMathParser_SRC})
target_link_libraries(OTBMathParser
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMuParser_LIBRARIES}
  ${OTBPath_LIBRARIES}
  ${OTBVectorDataBase_LIBRARIES}
  )

otb_module_target(OTBMathParser)
