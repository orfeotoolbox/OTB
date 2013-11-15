# Make sure the user does not try to install on top of the build tree.
if(WIN32 OR APPLE OR UNIX)
  string(TOLOWER "${CMAKE_INSTALL_PREFIX}" _PREFIX)
  string(TOLOWER "${OTB_BINARY_DIR}" _BUILD)
else(WIN32 OR APPLE OR UNIX)
  set(_PREFIX "${CMAKE_INSTALL_PREFIX}")
  set(_BUILD "${OTB_BINARY_DIR}")
endif(WIN32 OR APPLE OR UNIX)
if("${_PREFIX}" STREQUAL "${_BUILD}")
  message(FATAL_ERROR
    "The current CMAKE_INSTALL_PREFIX points at the build tree:\n"
    "  ${CMAKE_INSTALL_PREFIX}\n"
    "This is not supported."
    )
endif("${_PREFIX}" STREQUAL "${_BUILD}")
