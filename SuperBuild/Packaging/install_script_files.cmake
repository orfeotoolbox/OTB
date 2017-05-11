function(install_script_files)
  #For Unixes we make them in the *pkgsetup.in
  ##################### install environment setup file ##########################
  if(WIN32)
    install(
      FILES
      ${PACKAGE_SUPPORT_FILES_DIR}/otbenv.bat
      ${PACKAGE_SUPPORT_FILES_DIR}/otbenv.bash
      DESTINATION ${PKG_STAGE_DIR}
      )
    
    #we need startup files for mapla monteverdi in the root directory
    #For Unixes, we make them inside pkgsetup script!
    if(HAVE_MVD)
      install(
        PROGRAMS ${PACKAGE_SUPPORT_FILES_DIR}/mapla.bat
	PROGRAMS ${PACKAGE_SUPPORT_FILES_DIR}/monteverdi.bat
        DESTINATION "${PKG_STAGE_DIR}"
        )
    endif()
  endif()
  
  ########################## selftest script ##############################
  install(
    FILES ${PACKAGE_SUPPORT_FILES_DIR}/selftester${SCRIPT_EXT}
    DESTINATION ${PKG_STAGE_DIR}/bin
    )


endfunction()
