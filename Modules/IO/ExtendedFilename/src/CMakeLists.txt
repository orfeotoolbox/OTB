set(OTBExtendedFilename_SRC
  otbExtendedFilenameToReaderOptions.cxx
  otbExtendedFilenameHelper.cxx
  otbExtendedFilenameToWriterOptions.cxx
  )

add_library(OTBExtendedFilename ${OTBExtendedFilename_SRC})
target_link_libraries(OTBExtendedFilename 
  ${OTBIOGDAL_LIBRARIES}
  ${OTBBoost_LIBRARIES}

  )

otb_module_target(OTBExtendedFilename)
