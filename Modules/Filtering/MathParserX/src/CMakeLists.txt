set(OTBMathParserX_SRC
  otbParserX.cxx
  otbParserXPlugins.cxx
  )

add_library(OTBMathParserX ${OTBMathParserX_SRC})
target_link_libraries(OTBMathParserX
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBMuParserX_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  )

otb_module_target(OTBMathParserX)
