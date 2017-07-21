# The below function is modified from GetPrerequisities.cmake
# which is distributed with CMake.
function(isfile_symlink file result_var1 result_var2)
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
