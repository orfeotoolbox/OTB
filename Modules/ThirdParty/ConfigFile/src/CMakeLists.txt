set(OTB3P_INSTALL_EXPORT_NAME "${OTBConfigFile-targets}")
set(OTB3P_INSTALL_INCLUDE_DIR "${OTBConfigFile_INSTALL_INCLUDE_DIR}")
set(OTB3P_INSTALL_RUNTIME_DIR "${OTBConfigFile_INSTALL_RUNTIME_DIR}")
set(OTB3P_INSTALL_LIBRARY_DIR "${OTBConfigFile_INSTALL_LIBRARY_DIR}")
set(OTB3P_INSTALL_ARCHIVE_DIR "${OTBConfigFile_INSTALL_ARCHIVE_DIR}")

add_subdirectory(otbconfigfile)
otb_module_target(otbconfigfile NO_INSTALL)

install(FILES otb_configfile.h
  DESTINATION ${OTBConfigFile_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
