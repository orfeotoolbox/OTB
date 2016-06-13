file(GLOB all_dot_diff_files "${PATCH_DIR}/*${DIFF_FILE_MATCH_STRING}*diff")
find_program(PATCH_PROGRAM NAMES patch)
if(PATCH_PROGRAM)
  list(REVERSE all_dot_diff_files)
  foreach(dot_diff_file ${all_dot_diff_files})
    # message("dot_diff_file=${dot_diff_file}")
    execute_process(
      COMMAND
      ${CMAKE_COMMAND} -E chdir ${SOURCE_DIR}  ${PATCH_PROGRAM} "-p1" INPUT_FILE "${dot_diff_file}"
      RESULT_VARIABLE patch_rv
      OUTPUT_VARIABLE patch_ov
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
    if(NOT "${patch_rv}" STREQUAL "0")
      if( NOT "${patch_ov}" MATCHES "previously applied")
        message(FATAL_ERROR "${PATCH_PROGRAM} returned non-zero exit status '${patch_ov}'")
      endif()
    else()
      message(STATUS "${patch_ov}")
    endif()
  endforeach()
else()
  if(NOT WIN32)
    #Raise a fatal error if not on windows
    message(FATAL_ERROR "patch program not found. PATCH_PROGRAM. search names :' patch'")
  endif()
endif()