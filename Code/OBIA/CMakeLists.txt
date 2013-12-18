# Sources of non-templated classes.

file(GLOB OTBOBIA_SRCS "*.cxx" )


add_library(OTBOBIA ${OTBOBIA_SRCS})
#Apparently not supported by opensolaris at least
#set_target_properties(OTBOBIA
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#)
target_link_libraries(OTBOBIA OTBIO)
if(OTB_USE_MAPNIK)
    target_link_libraries(OTBOBIA ${MAPNIK_LIBRARY} ${ICUUC_LIBRARY})
endif()

if(OTB_I18N)
    target_link_libraries(OTBOBIA ${GETTEXT_LIBRARY})
    if(APPLE OR WIN32)
       target_link_libraries(OTBOBIA ${GETTEXT_INTL_LIBRARY})
    endif()
endif()

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBOBIA PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif()

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBOBIA
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif()

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  file(GLOB __files3 "${CMAKE_CURRENT_BINARY_DIR}/*.h")
  install(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/OBIA
    COMPONENT Development)
endif()
