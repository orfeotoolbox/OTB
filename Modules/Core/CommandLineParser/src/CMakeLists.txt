set(OTBCommandLineParser_SRC
  otbCommandLineArgumentParser.cxx
  )

add_library(OTBCommandLineParser ${OTBCommandLineParser_SRC})
target_link_libraries(OTBCommandLineParser
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  )

otb_module_target(OTBCommandLineParser)
