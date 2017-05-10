function(process_file_recurse input_file)

  set(input_file_full_path)
  search_library(${input_file} PKG_SEARCHDIRS input_file_full_path)

  if(NOT input_file_full_path)
    if(LINUX)
      setif_value_in_list(is_gtk_lib "${input_file}" ALLOWED_SYSTEM_DLLS)
      if(is_gtk_lib)
	search_library(${input_file} PKG_GTK_SEARCHDIRS input_file_full_path)
	if( NOT input_file_full_path)
	  message(FATAL_ERROR "${input_file} not found. searched in ${PKG_GTK_SEARCHDIRS}")
	endif()
      endif()
      if( NOT input_file_full_path)
	message(FATAL_ERROR "${input_file} not found. searched in ${PKG_SEARCHDIRS}")
	endif()
    endif(LINUX)

  endif() #if(NOT input_file_full_path)

  if(NOT PKG_DEBUG)
    message("Processing ${input_file_full_path}")
  endif()

  set(is_executable FALSE)
  is_file_executable2(input_file_full_path is_executable)

  if(NOT is_executable)
    #copy back to input_file_full_path
    pkg_install_rule(${input_file_full_path})
    message("not is_executable ${input_file_full_path}")
    return()
  endif() #NOT is_executable

  if(UNIX)
    # Deal with symlinks.
    # For any valid symlinks, (see 'not_valid' below) 
    # we append ln -s source target commands to a file
    # That file is  executed during installation. 
    get_filename_component(bn_we ${input_file_full_path} NAME_WE)
    get_filename_component(bn_path ${input_file_full_path} PATH)
    
    file(GLOB sofiles "${bn_path}/${bn_we}*")
    foreach(sofile ${sofiles})
      get_filename_component(basename_of_sofile ${sofile} NAME)
      get_filename_component(sofile_ext ${sofile} EXT)
      set(not_valid FALSE)
      if(  "${sofile_ext}" MATCHES ".la"
          OR "${sofile_ext}" MATCHES ".prl"
          OR "${sofile_ext}" MATCHES ".a"
          OR  IS_DIRECTORY "${sofile}" )
        set(not_valid TRUE)
      endif()

      if(not_valid)
        continue()
      endif()

      func_is_file_a_symbolic_link("${sofile}" is_symlink linked_to_file)

      if(is_symlink)
        add_to_symlink_list("${linked_to_file}" "${basename_of_sofile}")	
      endif() # is_symlink

    endforeach()

  endif(UNIX)

  set(raw_items)

  execute_process(
    COMMAND ${LOADER_PROGRAM} ${LOADER_PROGRAM_ARGS} "${input_file_full_path}"
    RESULT_VARIABLE loader_rv
    OUTPUT_VARIABLE loader_ov
    ERROR_VARIABLE loader_ev
    )
  
  if(loader_rv)
    message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
  endif()

  string(REPLACE ";" "\\;" candidates "${loader_ov}")
  string(REPLACE "\n" "${eol_char};" candidates "${candidates}")
  
  get_filename_component(bn_name ${input_file_full_path} NAME)
  set(${bn_name}_USED TRUE CACHE INTERNAL "")

  if(PKG_DEBUG)
    message("Processing ${input_file} started. Set ${bn_name}_USED=${${bn_name}_USED}")
  endif()

  foreach(candidate ${candidates})
    if(NOT candidate)
      continue()
    endif()

    if(NOT "${candidate}" MATCHES "${loader_program_regex}")
      continue()
    endif()
   
    string(REGEX REPLACE "${loader_program_regex}" "\\1" raw_item "${candidate}")

    if(NOT raw_item)
      continue()
    endif()  

    string(STRIP ${raw_item} raw_item)
    set(is_system FALSE)
    setif_value_in_list(is_system "${raw_item}" SYSTEM_DLLS)

    if(APPLE AND NOT is_system)
      if("${raw_item}" MATCHES "@rpath")
        string(REGEX REPLACE "@rpath." "" raw_item "${raw_item}")
      else()
        message(FATAL_ERROR "'${raw_item}' does not have @rpath")
      endif()
    endif()

    if(PKG_DEBUG AND ${raw_item}_RESOLVED)
      message("${raw_item} is already resolved [${raw_item}_RESOLVED=${${raw_item}_RESOLVED}]")
    endif()

    if(is_system OR ${raw_item}_RESOLVED OR ${raw_item}_USED)
      continue()
    endif()
    
    list(APPEND raw_items ${raw_item})
    
  endforeach()

  if(PKG_DEBUG)
    string(REPLACE ";" "\n" raw_items_pretty_print "${raw_items}")
    # message(FATAL_ERROR "raw_items=${raw_items_pretty_print}")
  endif(PKG_DEBUG)

  if(raw_items)
    list(REVERSE raw_items)
    foreach(item ${raw_items})      
      search_library(${item} PKG_SEARCHDIRS item_full_path)
      set(is_a_symlink FALSE)
      set(item_target_file)
      func_is_file_a_symbolic_link("${item_full_path}" is_a_symlink item_target_file)      
      if(is_a_symlink)
        set(${item}_RESOLVED TRUE CACHE INTERNAL "")
        set(item ${item_target_file})
      endif()
      if(PKG_DEBUG)
        message("${bn_name} depends on '${item}'. So we now process '${item}'") # [ ${item}_USED=${${item}_USED} ${item}_RESOLVED=${${item}_RESOLVED}]")
      endif()
      func_process_deps(${item})
    endforeach()
  endif()

  set(${bn_name}_RESOLVED TRUE CACHE INTERNAL "")
   if(PKG_DEBUG)
     message("All dependencies of ${bn_name} are processed. Install file and set ${bn_name}_RESOLVED=${${bn_name}_RESOLVED}")
   endif()

   #Install the file with pkg_install_rule. This function has specific rules to decide wheather install file or not
   pkg_install_rule(${input_file_full_path})

endfunction() #function(func_process_deps infile)
