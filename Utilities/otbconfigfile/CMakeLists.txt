PROJECT(otbconfigfile)

FILE(GLOB otbconfigfilelib_SRCS "ConfigFile.cpp")
FILE(GLOB otbconfigfilelib_HDRS "ConfigFile.h")
FILE(GLOB otbconfigfileCreation_SRCS "generate-config-properties.cpp")
FILE(GLOB otbconfigfile_DEFAULT "otb_config.inp")

ADD_EXECUTABLE(generate-config-properties ${otbconfigfileCreation_SRCS})
ADD_CUSTOM_COMMAND(
          OUTPUT ${OTB_BINARY_DIR}/${otbconfigfile_DEFAULT}
          COMMAND ${OTB_BINARY_DIR}/bin/generate-config-properties
          ARGS "${OTB_BINARY_DIR}/${otbconfigfile_DEFAULT}" "${OTB_LANG}"
          DEPENDS generate-config-properties
          COMMENT "Generating otb_config.inp" )
          
          
SET_SOURCE_FILES_PROPERTIES(
     ${otbconfigfilelib_SRCS} PROPERTIES
     OBJECT_DEPENDS "${OTB_BINARY_DIR}/${otbconfigfile_DEFAULT}"
     )


ADD_LIBRARY(otbconfigfile ${otbconfigfilelib_SRCS} )

TARGET_LINK_LIBRARIES(otbconfigfile )
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbconfigfile PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)



IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbconfigfile
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${otbconfigfilelib_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbconfigfile
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)


