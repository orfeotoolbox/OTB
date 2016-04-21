macro(create_monteverdi_application)
  cmake_parse_arguments(APPLICATION  "" "NAME;OUTPUT_NAME;COMPONENT_NAME;NEEDS_OTB_APPS" "SOURCES;LINK_LIBRARIES" ${ARGN} )

  if(NOT DEFINED APPLICATION_NEEDS_OTB_APPS OR APPLICATION_NEEDS_OTB_APPS)
    set(APPLICATION_NEEDS_OTB_APPS TRUE)
  else()
    set(APPLICATION_NEEDS_OTB_APPS FALSE)
  endif()

  if(WIN32)
    add_executable(${APPLICATION_NAME}
      WIN32
      ${APPLICATION_SOURCES})
  else() #Unix
    add_executable(${APPLICATION_NAME}
      ${APPLICATION_SOURCES})
  endif()

  set(EXECUTABLE_NAME ${APPLICATION_NAME})

  if (APPLICATION_OUTPUT_NAME)
    set_target_properties(${APPLICATION_NAME} PROPERTIES OUTPUT_NAME ${APPLICATION_OUTPUT_NAME})
    set(EXECUTABLE_NAME ${APPLICATION_OUTPUT_NAME})
  endif()

  if(APPLE)
    if(Monteverdi_USE_CPACK)
      set(MACOS_FILES_DIR "${CMAKE_SOURCE_DIR}/Packaging/MacOS")

      configure_file(${MACOS_FILES_DIR}/Info.plist.in
        ${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/Info.plist
	      @ONLY)

      configure_file(${MACOS_FILES_DIR}/StartupCommand.in
        ${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/${EXECUTABLE_NAME}_start
	      @ONLY)

      set_target_properties(${APPLICATION_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/Code/Application/${APPLICATION_NAME}/Info.plist")
    endif(Monteverdi_USE_CPACK)

  endif(APPLE)

  target_link_libraries(${APPLICATION_NAME} ${APPLICATION_LINK_LIBRARIES})

#############################################################################
install(TARGETS ${APPLICATION_NAME}
  BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT ${APPLICATION_COMPONENT_NAME}
  RUNTIME DESTINATION ${Monteverdi_INSTALL_BIN_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  LIBRARY DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  ARCHIVE DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT ${APPLICATION_COMPONENT_NAME}
  )

#############################################################################

endmacro(create_monteverdi_application)