function(install_rule src_file)

  get_filename_component(src_file_EXT ${src_file} EXT)
  get_filename_component(src_file_NAME ${src_file} NAME)
  
  get_filename_component(src_file_NAME_WE ${src_file} NAME_WE)
  
  get_filename_component(src_file_PATH ${src_file} PATH)

  if(NOT FILE_COMMAND)
    message(FATAL_ERROR "warning: No 'file' command, cannot continue...")
  endif()

  file(GLOB src_file_star "${src_file_PATH}/${src_file_NAME_WE}*")

  foreach(sfile ${src_file_star})
    #message( "sfile=${sfile}")
    get_filename_component(sfile_NAME ${sfile} NAME)
    get_filename_component(file_full "${sfile}" ABSOLUTE)
    string(TOLOWER "${file_full}" file_full_lower)

    string(TOLOWER "${sfile_NAME}" sfile_NAME_LOWER)
    if ("${sfile_NAME_LOWER}" MATCHES "^(otb|mvd)([a-z]+)(headertest|testdriver)")
      message( "SKIP INSTALL for ${sfile_NAME_LOWER}")
      continue()
    endif()
    
    set(install_type)
    set(install_dir)
    
    if("${file_full_lower}" MATCHES "(\\.exe)$")
      set(install_type PROGRAMS)
      set(install_dir bin)
    elseif("${file_full_lower}" MATCHES "(\\.dll)$")
      set(install_type FILES)
      set(install_dir bin) 
    elseif("${file_full_lower}" MATCHES "(\\.lib)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${file_full_lower}" MATCHES "(\\.a)$")
      set(install_type FILES)
      set(install_dir lib)
    elseif("${file_full_lower}" MATCHES "\\.lnk$")
      #Assuming the file is linked to a file with same name without .lnk extension
      get_filename_component(real_file "${file_full_lower}" NAME_WE)
      get_filename_component(real_file_PATH "${real_file}" PATH)
      get_filename_component(real_file_PATH "${real_file_PATH}" PATH)
      set(install_type "symlink")
      set(install_dir ${real_file_PATH})
      set(linked_to_file ${real_file})
    elseif("${file_full_lower}" MATCHES "\\.prl$")
      continue()
    elseif("${file_full_lower}" MATCHES "\\.la$")
      continue()
    else()
      execute_process(COMMAND "${FILE_COMMAND}" "${file_full}"
	RESULT_VARIABLE file_rv
	OUTPUT_VARIABLE file_ov
	ERROR_VARIABLE file_ev
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)
      if(NOT file_rv STREQUAL "0")
	message(FATAL_ERROR "${FILE_COMMAND} failed: ${file_rv}\n${file_ev}\n${file_ov}")
      endif()
      string(REPLACE "${file_full}" " _file_full_ " file_ov "${file_ov}")
      string(TOLOWER "${file_ov}" file_ov)

      # below executable check works for both mac osx and linux
      if("${file_ov}" MATCHES "executable")
	set(install_type PROGRAMS)
	set(install_dir bin)
	# detect position independent executables on Linux,
	# where "file" gives "dynamically linked (uses shared libraries)"
      elseif("${file_ov}" MATCHES "dynamically linked.*\(uses shared libs\)")
	set(install_type FILES)
	set(install_dir lib)
	# detect position independent executables on Linux,
	# where "file" gives "shared object ... (uses shared libraries)"
      elseif("${file_ov}" MATCHES "shared object.*\(uses shared libs\)")
	set(install_type FILES)
	set(install_dir lib)
	# detect shared libraries on Linux,
	# where "file" gives "ELF 64-bit LSB shared object, AMD x86-64, version 1 (SYSV), not stripped"
      elseif("${file_ov}" MATCHES "elf.*shared object.*version")
	set(install_type FILES)
	set(install_dir lib)
	# "file" version 5.22 does not print "(used shared libraries)"
	# but uses "interpreter"
      elseif("${file_ov}" MATCHES "shared object.*interpreter")
	set(install_type FILES)
	set(install_dir lib)
	# detect shared libraries on Mac OSX
	# where "file" gives "Mach-O 64-bit x86_64 dynamically linked shared library"
      elseif("${file_ov}" MATCHES "mach-o.*dynamically linked shared library")
	set(install_type FILES)
	set(install_dir lib)
	#below check is redundant.  detect executables on Mac OSX
	# where "file" gives "Mach-O 64-bit x86_64 executable"
      elseif("${file_ov}" MATCHES "mach-o.*executable")
	set(install_type PROGRAMS)
	set(install_dir bin)
	# detect shared libraries on Mac OSX
	# where "file" gives "Mach-O 64-bit x86_64 bundle"
      elseif("${file_ov}" MATCHES "mach-o.*bundle")
	set(install_type FILES)
	set(install_dir lib)
      elseif("${file_ov}" MATCHES "mach-o.*bundle")
	set(install_type FILES)
	set(install_dir lib)
      elseif("${file_ov}" MATCHES "symbolic link")
	#Now find where the symlink is linked to.
	#Do a regex replace
	string(REGEX REPLACE "_file_full_*.*symbolic.link.to." "" target_file ${file_ov})
	#older version of file command output has  [`} character(remove it).
	string(REPLACE "`" "" target_file ${target_file} )
	#older version of file command output has  {'} character(remove it).
	string(REPLACE "'" "" target_file "${target_file}")
	#strip final output
	string(STRIP ${target_file} target_file)
	setif_value_in_list(is_gtk_lib "${sfile_NAME}" GTK_LIB_LIST_1)
	set(lib_dir "lib")
	if(is_gtk_lib)
	  set(lib_dir "lib/gtk")
	endif()
	
	file(APPEND
	  ${CMAKE_BINARY_DIR}/make_symlinks
	  "ln -sf \"$OUT_DIR/${lib_dir}/${target_file}\" \"$OUT_DIR/${lib_dir}/${sfile_NAME}\" \n"
	  )

	continue()
      endif()

    endif()

    if("${sfile_NAME}" MATCHES "^otbapp_")
      set(output_dir "lib/otb/applications")
      set(install_type PROGRAMS)
      ########### install cli and gui script for each application ###########
      set(WRAPPER_SCRIPT_EXT "")
      if(WIN32)
	set(WRAPPER_SCRIPT_EXT ".bat")
      endif()
      
      set(${src_file_NAME_WE}_CLI "${SUPERBUILD_INSTALL_DIR}/bin/otbcli_${src_file_NAME_WE}${WRAPPER_SCRIPT_EXT}")
      set(${src_file_NAME_WE}_GUI "${SUPERBUILD_INSTALL_DIR}/bin/otbgui_${src_file_NAME_WE}${WRAPPER_SCRIPT_EXT}")
      
      string(REPLACE "otbapp_" "" ${src_file_NAME_WE}_GUI ${${src_file_NAME_WE}_GUI})
      string(REPLACE "otbapp_" "" ${src_file_NAME_WE}_CLI ${${src_file_NAME_WE}_CLI})
      
      if(NOT EXISTS "${${src_file_NAME_WE}_CLI}")
	message(FATAL_ERROR "Cannot find ${${src_file_NAME_WE}_CLI} for ${src_file_NAME_WE}")
      endif()

      if(NOT EXISTS "${${src_file_NAME_WE}_GUI}")
	message(FATAL_ERROR "Cannot find ${${src_file_NAME_WE}_GUI} for ${src_file_NAME_WE}")
      endif()

      install(PROGRAMS
	${${src_file_NAME_WE}_CLI}
      	${${src_file_NAME_WE}_GUI}
	DESTINATION
	${PKG_STAGE_DIR}/bin )
    endif()

    if(install_type AND install_dir)
      install(${install_type} "${sfile}"
	DESTINATION
	"${PKG_STAGE_DIR}/${install_dir}"
	)
    else()
      message(FATAL_ERROR "sfile=${sfile} || file_ov=${file_ov}")
    endif()    
    
  endforeach()

  
endfunction()   

