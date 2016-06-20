
find_program(PATCH_PROGRAM NAMES patch)
if(PATCH_PROGRAM)
  set(OPTS)
  if(WIN32)
    set(OPTS "--binary")
  endif()

  separate_arguments(DOT_DIFF_FILES)

  list(SORT DOT_DIFF_FILES)

  foreach(dot_diff_file ${DOT_DIFF_FILES})
    message("Input patch file: ${dot_diff_file}")
    execute_process(
      COMMAND
      ${CMAKE_COMMAND} -E chdir ${SOURCE_DIR} 
      ${PATCH_PROGRAM} "-p1" "-i" "${dot_diff_file}" ${OPTS}
      RESULT_VARIABLE patch_rv
      OUTPUT_VARIABLE patch_ov
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_STRIP_TRAILING_WHITESPACE)
    if(NOT "${patch_rv}" STREQUAL "0")
      if( NOT "${patch_ov}" MATCHES "previously applied")
        message(FATAL_ERROR "${PATCH_PROGRAM} returned non-zero exit status '${patch_ov}'")
      endif()
    endif()

    if( "${patch_ov}" MATCHES "FAILED")
      message(FATAL_ERROR "${PATCH_PROGRAM} returned non-zero exit status '${patch_ov}'")
    else()
      message("${patch_ov}")      
    endif()
    
    unset(patch_rv)
    unset(patch_ov)
  endforeach()
else()
  message(FATAL_ERROR "patch program not found. PATCH_PROGRAM. search names :' patch'")
endif()

