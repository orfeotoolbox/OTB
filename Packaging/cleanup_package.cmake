macro(cleanup_package)

if(EXISTS "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")

message(STATUS "Deleting package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
execute_process(COMMAND 
${CMAKE_COMMAND} -E remove_directory
  "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}"
  RESULT_VARIABLE cleanup_stagedir_rv
  OUTPUT_VARIABLE cleanup_stagedir_ov
  ERROR_VARIABLE cleanup_stagedir_ov
  )
  
  if(cleanup_stagedir_rv)
  message(FATAL_ERROR  "Delete ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR} - FAILED\ncleanup_stagedir_ov=${cleanup_stagedir_ov}")
  else()
  message(STATUS "Delete ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR} - OK")  
  endif()
endif()

if(EXISTS "${PACKAGE_OUTPUT_FILE}")
message(STATUS "Remove PACKAGE_OUTPUT_FILE: ${PACKAGE_OUTPUT_FILE}")
execute_process(COMMAND 
${CMAKE_COMMAND} -E remove -f "${PACKAGE_OUTPUT_FILE}"
  RESULT_VARIABLE cleanup_pkgfile_rv
  OUTPUT_VARIABLE cleanup_pkgfile_ov
  ERROR_VARIABLE cleanup_pkgfile_ov
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  
    if(cleanup_pkgfile_rv)
  message(FATAL_ERROR  "Delete ${PACKAGE_OUTPUT_FILE} - FAILED\ncleanup_pkgfile_ov=${cleanup_pkgfile_ov}")
  else()
  message(STATUS "Delete ${PACKAGE_OUTPUT_FILE} - OK")  
  endif()
  
endif()
endmacro()
