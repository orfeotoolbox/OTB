if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  # install(FILES ${__files1}
  # install(FILES ${__files1} ${__files2}
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/UtilitiesAdapters/boost/type_traits
    COMPONENT Development)
endif()
