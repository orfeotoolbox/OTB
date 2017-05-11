function(install_rule src_file)

  get_filename_component(src_file_EXT ${src_file} EXT)
  get_filename_component(src_file_NAME ${src_file} NAME)

  set(file_type PROGRAMS)
  if(NOT src_file_EXT)
    set(output_dir "bin")  
  elseif("${src_file_EXT}" MATCHES "(.[Dd][Ll][Ll]|.exe)")
    set(output_dir "bin")
  elseif("${src_file_EXT}" MATCHES "(.lib|.so|.dylib)")
    set(output_dir "lib")
    set(file_type FILES)
  else()
    message(FATAL_ERROR "unreachable code")
  endif()
  
  set(SKIP_INSTALL FALSE)

  setif_value_in_list(is_gtk_lib "${src_file_NAME}" GTK_LIB_LIST_1)
  if(is_gtk_lib)
    if(PKG_GENERATE_XDK)
      set(SKIP_INSTALL TRUE)
    else()
      set(output_dir "lib/gtk")
    endif() #if(PKG_GENERATE_XDK)
  endif() #if(is_gtk_lib)
  

  set(SKIP_INSTALL FALSE)
  string(TOLOWER "${src_file_NAME}" src_file_NAME_LOWER)
  #avoid test executables

  #TODO:! a special case
  if ("${src_file_NAME_LOWER}" MATCHES "(otb|mvd)*.*test*.*${EXE_EXT}")
    if (NOT "${src_file_NAME_LOWER}" MATCHES "\\${LIB_EXT}" AND
	NOT "${src_file_NAME_LOWER}" MATCHES "otbtestdriver" )
      set(SKIP_INSTALL TRUE)
      message("SKIP_INSTALL for ${src_file_NAME}")
    endif()
  endif()

  #special case
  if("${src_file_NAME}" MATCHES "^otbapp_")
    set(output_dir "lib/otb/applications")
    set(file_type PROGRAMS)
    set(SKIP_INSTALL FALSE)

    get_filename_component(src_file_NAME_WE ${src_file} NAME_WE)
    
    ########### install cli and gui script for each application ###########
    set(WRAPPER_SCRIPT_EXT "")
    if(WIN32)
      set(WRAPPER_SCRIPT_EXT ".bat")
    endif()
    set(${src_file_NAME_WE}_CLI "${SUPERBUILD_INSTALL_DIR}/bin/otbcli_${src_file_NAME_WE}${WRAPPER_SCRIPT_EXT}")
    set(${src_file_NAME_WE}_GUI "${SUPERBUILD_INSTALL_DIR}/bin/otbgui_${src_file_NAME_WE}${WRAPPER_SCRIPT_EXT}")
    string(REPLACE "otbapp_" "" ${src_file_NAME_WE}_GUI ${${src_file_NAME_WE}_GUI})
    string(REPLACE "otbapp_" "" ${src_file_NAME_WE}_CLI ${${src_file_NAME_WE}_CLI})
    
    if(EXISTS "${${src_file_NAME_WE}_CLI}")
      install(PROGRAMS ${${src_file_NAME_WE}_CLI} DESTINATION ${PKG_STAGE_DIR}/bin)
    else()
      message(FATAL_ERROR "Cannot find ${${src_file_NAME_WE}_CLI} for ${src_file_NAME_WE}")
    endif()

    if(EXISTS "${${src_file_NAME_WE}_GUI}")
      install(PROGRAMS ${${src_file_NAME_WE}_GUI} DESTINATION ${PKG_STAGE_DIR}/bin)
    else()
      message(FATAL_ERROR "Cannot find ${${src_file_NAME_WE}_GUI} for ${src_file_NAME_WE}")
    endif()
    
  endif()

  if(NOT SKIP_INSTALL)
    install(${file_type} "${src_file}"
      DESTINATION
      "${PKG_STAGE_DIR}/${output_dir}"
      )
  endif()
  
endfunction()   

