file(GLOB all_dot_diff_files "${PATCH_DIR}/*${DIFF_FILE_MATCH_STRING}*diff" "${PATCH_DIR}/*all.diff")
message("all_dot_diff_files=${all_dot_diff_files}") 
find_program(PATCH_PROGRAM NAMES patch)
if(PATCH_PROGRAM)
set(OPTS)
if(WIN32)
set(OPTS "--binary")
endif()
  list(REVERSE all_dot_diff_files)
  foreach(dot_diff_file ${all_dot_diff_files})
    message("dot_diff_file=${dot_diff_file}")
    execute_process(
      COMMAND
      ${CMAKE_COMMAND} -E chdir ${SOURCE_DIR} 
      ${PATCH_PROGRAM} "-p1" "-i" "${dot_diff_file}" ${OPTS}
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
    message(FATAL_ERROR "patch program not found. PATCH_PROGRAM. search names :' patch'")
  endif()
endif()
