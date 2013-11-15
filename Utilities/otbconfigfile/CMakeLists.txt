project(otbconfigfile)

file(GLOB otbconfigfilelib_SRCS "ConfigFile.cpp")
file(GLOB otbconfigfilelib_HDRS "ConfigFile.h")

add_library(otbconfigfile ${otbconfigfilelib_SRCS} )

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbconfigfile PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)



if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbconfigfile
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES ${otbconfigfilelib_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbconfigfile
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)


