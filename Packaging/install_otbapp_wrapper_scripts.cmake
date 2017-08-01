function(install_otbapp_wrapper_scripts otbapp_libfile)
  ########### install cli and gui script for each application ###########
  set(my_script_ext "")
  if(WIN32)
    set(my_script_ext ".bat")
  endif()
  get_filename_component(otbapp_libfile_WE ${otbapp_libfile} NAME_WE)
  #just remove otbapp_ first. we will add otbcli_ or otbgui_ later
  set(otbapp_name)
  string(REPLACE "otbapp_" "" otbapp_name ${otbapp_libfile_WE})

  #now define cli_script with valid name otbcli_<APPNAME><EXT>
  set(cli_script "otbcli_${otbapp_name}${my_script_ext}")
  if(NOT EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/${cli_script}")
    #report error!
    message(FATAL_ERROR "Cannot find ${cli_script} for ${otbapp_libfile}")
  endif()
  #install file
  install(
    PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/${cli_script}"
    DESTINATION ${PKG_STAGE_DIR}/bin
    )

  #Do the same for otbgui script but only if HAVE_QT4 is set.
  if(NOT HAVE_QT4)
    return()
  endif()
  
  #now define gui_script_actual with valid name otbgui_<APPNAME><EXT>
  set(gui_script "otbgui_${otbapp_name}${my_script_ext}")
  if(NOT EXISTS "${SUPERBUILD_INSTALL_DIR}/bin/${gui_script}")
    #report error
    message(FATAL_ERROR "Cannot find ${gui_script} for ${otbapp_libfile}")
  endif()
  #install script file
  install(
  PROGRAMS "${SUPERBUILD_INSTALL_DIR}/bin/${gui_script}"
  DESTINATION ${PKG_STAGE_DIR}/bin )
endfunction() 
