message(STATUS "Empty package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
execute_process(COMMAND
g  ${CMAKE_COMMAND} -E remove_directory
  "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}"
  )
message(STATUS "Remove PACKAGE_OUTPUT_FILE: ${PACKAGE_OUTPUT_FILE}")
execute_process(COMMAND
  ${CMAKE_COMMAND} -E remove -f "${PACKAGE_OUTPUT_FILE}"
  )
