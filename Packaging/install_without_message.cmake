function(install_without_message src_dir dst_dir_suffix)
  set (extra_func_args ${ARGN})
  list(LENGTH extra_func_args num_extra_args)
  if (${num_extra_args} GREATER 0)
    list(GET extra_func_args 0 optional_msg)
  endif()

  if( "${dst_dir_suffix}" STREQUAL "")
    set(dst_dir "${PKG_STAGE_DIR}")
  else()
    set(dst_dir "${PKG_STAGE_DIR}/${dst_dir_suffix}")
  endif()

  get_filename_component(src_dir_name ${src_dir} NAME)
  set(install_msg "message(STATUS \"Installing: ${CMAKE_INSTALL_PREFIX}/${dst_dir}/${src_dir_name} ${optional_msg}\")")
  install(CODE "${install_msg}" )
  install(
    DIRECTORY   ${src_dir}
    DESTINATION ${dst_dir}
    MESSAGE_NEVER )
endfunction() #install_without_message
