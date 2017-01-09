function(find_python_soname result)
  set(${result} PARENT_SCOPE)
  execute_process(
    COMMAND ${PYTHON_EXECUTABLE}
    -c
    "import sys; from distutils import sysconfig; print (sysconfig.get_config_var('INSTSONAME'));"
    RESULT_VARIABLE python_INSTALLED_SONAME_rv
    OUTPUT_VARIABLE python_INSTALLED_SONAME_ov
    )

  if(NOT python_INSTALLED_SONAME_rv EQUAL 0)
    message( FATAL_ERROR
      "python_INSTALLED_SONAME_rv=${python_INSTALLED_SONAME_rv}: Cannot find python library")
  endif()
  set(${result} "${python_INSTALLED_SONAME_ov}" PARENT_SCOPE)
endfunction()


function(func_install_without_message src_dir dst_dir_suffix)
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
endfunction() #func_install_without_message


macro(macro_empty_package_staging_directory)
  message(STATUS "Empty package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
  execute_process(
    COMMAND ${CMAKE_COMMAND}
    -E remove_directory
    "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}"
    )
endmacro() #macro_empty_package_staging_directory


function(search_library input_file pkg_searchdirs result)
  set(${result} "" PARENT_SCOPE)
  foreach(pkg_searchdir ${${pkg_searchdirs}})

    if(EXISTS ${pkg_searchdir}/${input_file} )
      if(PKG_DEBUG)
	message("searching for '${input_file}' in '${pkg_searchdir}'")
      endif()
      set(${result} "${pkg_searchdir}/${input_file}" PARENT_SCOPE)
      return()
    endif()

    #check for file with lowercase
    string(TOLOWER "${input_file}" input_file_lower )
    if(EXISTS ${pkg_searchdir}/${input_file_lower})
      set(${result} "${pkg_searchdir}/${input_file_lower}"  PARENT_SCOPE)
      return()
    endif()
  endforeach()

endfunction()

macro(add_to_symlink_list src_file target_file)
  #TODO: avoid code duplication here and later in install_rule

  set(SKIP_INSTALL FALSE)
  if(PKG_GENERATE_XDK)
    get_filename_component(src_file_NAME ${src_file} NAME)
    setif_value_in_list(is_gtk_lib "${src_file_NAME}" ALLOWED_SYSTEM_DLLS)
    if ("${src_file_NAME}"
	MATCHES
	"libOTB|libotb|otbApp|otbapp_|otbTest|libMonteverdi|monteverdi|mapla|iceViewer"
	)
      set(SKIP_INSTALL TRUE)
    endif()

    if(is_gtk_lib)
      set(SKIP_INSTALL TRUE)
    endif()
    
  endif(PKG_GENERATE_XDK)
  # NOTE: $OUT_DIR is set actually in pkgsetup.in. So don't try
  # any pre-mature optimization on that variable names
  if(NOT SKIP_INSTALL)
    file(APPEND
      ${CMAKE_BINARY_DIR}/make_symlinks_temp
      "ln -sf \"$OUT_DIR/lib/${src_file}\" \"$OUT_DIR/lib/${target_file}\" \n"
      )
  endif()
endmacro()

function(is_file_executable2 file result_var)
  #
  # A file is not executable until proven otherwise:
  #
  set(${result_var} 0 PARENT_SCOPE)

  get_filename_component(file_full "${file}" ABSOLUTE)
  string(TOLOWER "${file_full}" file_full_lower)

  # If file name ends in .exe on Windows, *assume* executable:
  #
  if(WIN32 AND NOT UNIX)
    if("${file_full_lower}" MATCHES "(\\.exe|\\.dll)$")
      set(${result_var} 1 PARENT_SCOPE)
      return()
    endif()

    # A clause could be added here that uses output or return value of dumpbin
    # to determine ${result_var}. In 99%+? practical cases, the exe name
    # match will be sufficient...
    #
  endif() #WIN32 AND NOT UNIX

  func_is_file_a_symbolic_link("${file_full}" is_a_symlink file_full_target)
  if(is_a_symlink)
    message("resolving '${file_full}' to  '${file_full_target}")
    get_filename_component(file_full_path "${file_full}" PATH)
    set(file_full "${file_full_path}/${file_full_target}")
    string(TOLOWER "${file_full}" file_full_lower)
  endif()

  # Use the information returned from the Unix shell command "file" to
  # determine if ${file_full} should be considered an executable file...
  #
  # If the file command's output contains "executable" and does *not* contain
  # "text" then it is likely an executable suitable for prerequisite analysis
  # via the get_prerequisites macro.
  #
  #if we are not on a unix or unix-like platform, then we don't have any business here
  if(NOT UNIX)
    return()
  endif()

  if(NOT FILE_COMMAND)
    message(FATAL_ERROR "warning: No 'file' command, cannot continue...")
  endif()

  if(NOT EXISTS "${file_full}")
    message(FATAL_ERROR "err. '${file_full}' does not exists or is not absolute path")
  endif()

  execute_process(COMMAND "${FILE_COMMAND}" "${file_full}"
    RESULT_VARIABLE file_rv
    OUTPUT_VARIABLE file_ov
    ERROR_VARIABLE file_ev
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT file_rv STREQUAL "0")
    message(FATAL_ERROR "${FILE_COMMAND} failed: ${file_rv}\n${file_ev}")
  endif()

  # Replace the name of the file in the output with a placeholder token
  # (the string " _file_full_ ") so that just in case the path name of
  # the file contains the word "text" or "executable" we are not fooled
  # into thinking "the wrong thing" because the file name matches the
  # other 'file' command output we are looking for...
  #
  string(REPLACE "${file_full}" " _file_full_ " file_ov "${file_ov}")
  string(TOLOWER "${file_ov}" file_ov)

  # message(STATUS "file_ov='${file_ov}'")
  # below executable check works for both mac osx and linux
  # message("file_full=${file_ov}")
  if("${file_ov}" MATCHES "executable")
    #message(STATUS "executable!")
    if("${file_ov}" MATCHES "text")
      set(${result_var} 0 PARENT_SCOPE)
      #message(FATAL_ERROR "but text, so *not* a binary executable!")
    else()
      set(${result_var} 1 PARENT_SCOPE)
      return()
    endif()
  endif()

  # detect position independent executables on Linux,
  # where "file" gives "dynamically linked (uses shared libraries)"
  if("${file_ov}" MATCHES "dynamically linked.*\(uses shared libs\)")
    set(${result_var} 1 PARENT_SCOPE)
    return()
  endif()

  # detect position independent executables on Linux,
  # where "file" gives "shared object ... (uses shared libraries)"
  if("${file_ov}" MATCHES "shared object.*\(uses shared libs\)")
    set(${result_var} 1 PARENT_SCOPE)
    return()
  endif()

  # detect shared libraries on Linux,
  # where "file" gives "ELF 64-bit LSB shared object, AMD x86-64, version 1 (SYSV), not stripped"
  if("${file_ov}" MATCHES "elf.*shared object.*version")
    set(${result_var} 1 PARENT_SCOPE)
    return()
  endif()

  # "file" version 5.22 does not print "(used shared libraries)"
  # but uses "interpreter"
  if("${file_ov}" MATCHES "shared object.*interpreter")
    set(${result_var} 1 PARENT_SCOPE)
    return()
  endif()

  if(APPLE)
    # detect shared libraries on Mac OSX
    # where "file" gives "Mach-O 64-bit x86_64 dynamically linked shared library"
    if("${file_ov}" MATCHES "mach-o.*dynamically linked shared library")
      set(${result_var} 1 PARENT_SCOPE)
      return()
    endif()

    #below check is redundant.  detect executables on Mac OSX
    # where "file" gives "Mach-O 64-bit x86_64 executable"
    if("${file_ov}" MATCHES "mach-o.*executable")
      set(${result_var} 1 PARENT_SCOPE)
      return()
    endif()

  endif(APPLE)

endfunction()


# The below function is modified from GetPrerequisities.cmake
# which is distributed with CMake.
function(func_is_file_a_symbolic_link file result_var1 result_var2)
  #
  # A file is not executable until proven otherwise:
  #
  set(${result_var1} 0 PARENT_SCOPE)
  set(${result_var2} "" PARENT_SCOPE)

  get_filename_component(file_full "${file}" ABSOLUTE)
  string(TOLOWER "${file_full}" file_full_lower)

  # If file name ends in .exe on Windows, *assume* executable:
  #
  if(WIN32 AND NOT UNIX)
    if("${file_full_lower}" MATCHES "\\.lnk$")
      set(${result_var1} 1 PARENT_SCOPE)
      #Assuming the file is linked to a file with same name without .lnk extension
      get_filename_component(name_we_lnk "${file_full_lower}" NAME_WE)
      set(${result_var2} "${name_we_lnk}" PARENT_SCOPE)
      return()
    endif()

    # A clause could be added here that uses output or return value of dumpbin
    # to determine ${result_var}. In 99%+? practical cases, the exe name
    # match will be sufficient...
    #
  endif()

  #if we are not on a unix or unix-like platform, then we don't have any business here
  if(NOT UNIX)
    return()
  endif()

  # Use the information returned from the Unix shell command "file" to
  # determine if ${file_full} should be considered an executable file...
  #
  # If the file command's output contains "executable" and does *not* contain
  # "text" then it is likely an executable suitable for prerequisite analysis
  # via the get_prerequisites macro.
  #
  if(NOT FILE_COMMAND)
    message(FATAL_ERROR "warning: No 'file' command, cannot continue...")
  endif()

  execute_process(COMMAND "${FILE_COMMAND}" "${file_full}"
    RESULT_VARIABLE file_rv
    OUTPUT_VARIABLE file_ov
    ERROR_VARIABLE file_ev
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(NOT file_rv STREQUAL "0")
    message(FATAL_ERROR "${FILE_COMMAND} failed: ${file_rv}\n${file_ev}")
  endif()

  # Replace the name of the file in the output with a placeholder token
  # (the string " _file_full_ ") so that just in case the path name of
  # the file contains the word "text" or "executable" we are not fooled
  # into thinking "the wrong thing" because the file name matches the
  # other 'file' command output we are looking for...
  #
  string(REPLACE "${file_full}" " _file_full_ " file_ov "${file_ov}")
  string(TOLOWER "${file_ov}" file_ov_lower)

  # message(FATAL_ERROR "file_ov='${file_ov}'")
  if("${file_ov_lower}" MATCHES "symbolic link")
    set(${result_var1} 1 PARENT_SCOPE)
    #Now find where the symlink is linked to.
    #Do a regex replace
    string(REGEX REPLACE "_file_full_*.*symbolic.link.to." "" symlinked_to ${file_ov})

    #older version of file command output has  [`} character(remove it).
    string(REPLACE "`" "" symlinked_to ${symlinked_to} )

    #older version of file command output has  {'} character(remove it).
    string(REPLACE "'" "" symlinked_to "${symlinked_to}")

    #strip final output
    string(STRIP ${symlinked_to} symlinked_to)
    set(${result_var2} "${symlinked_to}" PARENT_SCOPE)

    #message(FATAL_ERROR "${file_full} is symlinked_to ${symlinked_to}")
    return()
  endif()

endfunction()

macro(setif_value_in_list matched value list)
  set(${matched})
  string(TOLOWER ${value} value_)
  foreach (pattern ${${list}})
    string(TOLOWER ${pattern} pattern_)
    if("${value_}" MATCHES "${pattern_}")
      set(${matched} TRUE)
    endif()
  endforeach()
  #message("'${value}' not found in ${list}")
endmacro()

function (get_vars_ending_with suffix result)
  get_cmake_property(all_cmake_vars VARIABLES)
  string (REGEX MATCHALL "(^|;)[A-Za-z0-9_\\.\\-]*(${suffix})" _matchedVars "${all_cmake_vars}")
  set(${result} ${_matchedVars} PARENT_SCOPE)
endfunction()
