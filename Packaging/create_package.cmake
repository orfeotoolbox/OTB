macro(create_package)
  install(CODE "
    message(STATUS \"Creating ${PACKAGE_OUTPUT_FILE}\") 
    if(WIN32)
      execute_process(
        COMMAND ${ZIP_EXECUTABLE} \"a\" \"-r\" \"-y\"
        \"${PACKAGE_OUTPUT_FILE}\" \"${PKG_STAGE_DIR}/*\"
        WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\"
      )
    else()
      execute_process(
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/makeself/makeself.sh
        \"--quiet\"
        \"--target\"
        \"${PKG_STAGE_DIR}\"
        \"${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}\"
        \"${PKG_STAGE_DIR}.run\"
        \"${PACKAGE_LONG_NAME} ${PKG_OTB_VERSION_STRING}\"
        \"./pkgsetup\"
        WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\"
     )
    endif()
  ")

endmacro()

