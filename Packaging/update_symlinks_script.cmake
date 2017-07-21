function(update_symlinks_script src_file_name link_file_name)
  #TODO: avoid code duplication here and later in install_rule
  set(SKIP_INSTALL FALSE)
  get_filename_component(src_file_name_NAME ${src_file_name} NAME)  
  setif_value_in_list(is_gtk_lib "${src_file_name_NAME}" GTK_LIB_LIST_1)
  if(is_gtk_lib)
    set(SKIP_INSTALL TRUE)
  endif()

  # NOTE: $OUT_DIR is set actually in pkgsetup.in. So don't try
  # any pre-mature optimization on that variable names
  if(NOT SKIP_INSTALL)
    set(lib_dir "lib")
    if(is_gtk_lib)
      set(lib_dir "lib/gtk")
    endif()
    file(APPEND
      ${CMAKE_BINARY_DIR}/make_symlinks_temp
      "ln -sf \"$OUT_DIR/${lib_dir}/${src_file_name}\" \"$OUT_DIR/${lib_dir}/${link_file_name}\" \n"
      )
  endif()
endfunction()
