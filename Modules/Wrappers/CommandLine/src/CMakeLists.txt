set(OTBCommandLine_SRC
  otbWrapperCommandLineLauncher.cxx
  otbWrapperCommandLineParser.cxx
  )

add_library(OTBCommandLine ${OTBCommandLine_SRC})
target_link_libraries(OTBCommandLine 
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  )
otb_module_target(OTBCommandLine)

add_executable(otbApplicationLauncherCommandLine otbApplicationLauncherCommandLine.cxx)
target_link_libraries(otbApplicationLauncherCommandLine OTBCommandLine)
otb_module_target_label(otbApplicationLauncherCommandLine)
