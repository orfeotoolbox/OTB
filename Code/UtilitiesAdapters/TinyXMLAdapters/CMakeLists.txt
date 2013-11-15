if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES otbTinyXML.h
          DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/TinyXMLAdapters
          COMPONENT Development)
endif()
