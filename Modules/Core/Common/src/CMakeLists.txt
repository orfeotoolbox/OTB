set(OTBCommon_SRC
  otbStandardFilterWatcher.cxx
  otbFilterWatcherBase.cxx
  otbSystem.cxx
  otbStandardWriterWatcher.cxx
  otbUtils.cxx
  otbConfigurationManager.cxx
  otbStandardOneLineFilterWatcher.cxx
  otbWriterWatcherBase.cxx
  )

add_library(OTBCommon ${OTBCommon_SRC})
target_link_libraries(OTBCommon 
  ${OTBITK_LIBRARIES}

  )

otb_module_target(OTBCommon)
