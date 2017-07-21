function(install_script_files)
  #For GNU/Linux & Mac OSX, we write them in the *pkgsetup.in
  if(WIN32)
    install(
      PROGRAMS
      Files/otbenv.bat
      Files/otbenv.bash
      DESTINATION ${PKG_STAGE_DIR}
      )
   

  else()
    configure_file(Files/setup_python.sh.in setup_python.sh @ONLY)
    
    install(PROGRAMS
      ${CMAKE_BINARY_DIR}/setup_python.sh
      Files/otbenv.profile
      DESTINATION "${PKG_STAGE_DIR}")
  endif()

  # We also need wrapper script to start mapla & monteverdi
  if(NOT HAVE_MVD)
    message(FATAL_ERROR "not have mvd")
  endif()
  if(HAVE_MVD)
    install(PROGRAMS
      Files/mapla${SCRIPT_EXT}
      Files/monteverdi${SCRIPT_EXT}
      DESTINATION "${PKG_STAGE_DIR}"
      )
  endif()

  set(my_file_ext "")
  if(WIN32)
    set(my_file_ext ".bat")
  endif()
  install(PROGRAMS
    Files/selftester${SCRIPT_EXT}
    "${SUPERBUILD_INSTALL_DIR}/bin/otbcli${my_file_ext}"
    "${SUPERBUILD_INSTALL_DIR}/bin/otbgui${my_file_ext}"
    DESTINATION ${PKG_STAGE_DIR}/bin)
endfunction()
