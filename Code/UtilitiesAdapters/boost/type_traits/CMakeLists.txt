IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  FILE(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  # INSTALL(FILES ${__files1}
  # INSTALL(FILES ${__files1} ${__files2}
  INSTALL(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/boost/type_traits
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
