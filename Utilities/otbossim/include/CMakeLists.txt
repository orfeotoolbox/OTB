
FILE(GLOB_RECURSE ossim_HDRS "*.h")
INSTALL(FILES ${ossim_HDRS}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include
    COMPONENT Development)

