if(WIN32)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES inttypes.h stdint.h
          DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbmsinttypes
          COMPONENT Development)
endif()

endif()
