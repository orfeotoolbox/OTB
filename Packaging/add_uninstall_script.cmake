function(add_uninstall_script)
  file(READ "${OTB_BINARY_DIR}/install_manifest.txt"  mfile_lines)
  string(
    REPLACE
    "${CMAKE_INSTALL_PREFIX}"
    "$\{MY_INSTALL_DIR}"
    mfile_lines_NEW ${mfile_lines} )

  file(WRITE "${CMAKE_BINARY_DIR}/install_manifest_otb.txt"
    ${mfile_lines_NEW})

  #read cmake_uninstall.cmake.in
  file(READ "${PACKAGE_OTB_SRC_DIR}/CMake/cmake_uninstall.cmake.in"  cmake_uninstall_lines)

  # Replace @CMAKE_INSTALL_PREFIX@ with ${MY_INSTALL_DIR}.
  # The result is stored in cmake_uninstall_lines_1
  string(
    REPLACE
    "@CMAKE_INSTALL_PREFIX@"
    "$\{MY_INSTALL_DIR}"
    cmake_uninstall_lines_1 ${cmake_uninstall_lines} )

  # Replace @CMAKE_BINARY_DIR@ with ${MY_INSTALL_DIR}.
  # The result is stored in cmake_uninstall_lines_2
  # Input is 'cmake_uninstall_lines_1' created from string(REPLACE...
  string(
    REPLACE
    "@CMAKE_BINARY_DIR@"
    "$\{MY_INSTALL_DIR}"
    cmake_uninstall_lines_2 ${cmake_uninstall_lines_1} )

  # Replace @CMAKE_COMMAND@ with ${CMAKE_COMMAND}.
  # The result is stored in cmake_uninstall_lines_NEW
  # Input is 'cmake_uninstall_lines_2' created from string(REPLACE...
  string(
    REPLACE
    "@CMAKE_COMMAND@"
    "$\{CMAKE_COMMAND}"
    cmake_uninstall_lines_NEW ${cmake_uninstall_lines_2} )
  

  #write cmake_uninstall_otb.cmake
  file(WRITE "${CMAKE_BINARY_DIR}/cmake_uninstall_otb.cmake"
    ${cmake_uninstall_lines_NEW})

  #install all required files to perform uninstall
  install(FILES
    Files/uninstall_otb${SCRIPT_EXT}
    ${CMAKE_BINARY_DIR}/install_manifest_otb.txt
    ${CMAKE_BINARY_DIR}/cmake_uninstall_otb.cmake
    DESTINATION ${PKG_STAGE_DIR})

endfunction()
