macro(installer_files)
  set(RSTDOC_DIR "${PACKAGE_OTB_SRC_DIR}/Documentation/Cookbook/rst")
  if(APPLE)
    set(README_FILE ${RSTDOC_DIR}/Installation_Macx.txt)
  elseif(LINUX) #not osx
    set(README_FILE ${RSTDOC_DIR}/Installation_Linux.txt)
  elseif(WIN32) #windows
    set(README_FILE ${RSTDOC_DIR}/Installation_Windows.txt)
  endif()
  configure_file("${README_FILE}" ${CMAKE_BINARY_DIR}/README )
  install(FILES ${CMAKE_BINARY_DIR}/README DESTINATION ${PKG_STAGE_DIR} )

  if(LINUX)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/Files/linux_pkgsetup.in
      ${CMAKE_CURRENT_BINARY_DIR}/pkgsetup @ONLY
      )
  endif()

  if(APPLE)
    set(ORIGINAL_RPATH_TO_REPLACE ${SUPERBUILD_INSTALL_DIR}/lib)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/Files/macx_pkgsetup.in
      ${CMAKE_CURRENT_BINARY_DIR}/pkgsetup @ONLY
      )
  endif()

  if(UNIX)
    install( PROGRAMS
      ${CMAKE_BINARY_DIR}/pkgsetup
      ${CMAKE_BINARY_DIR}/make_symlinks
      Files/setup_python.sh
      Files/otbenv.profile
      DESTINATION ${PKG_STAGE_DIR}
      )
  endif()

  set(my_file_ext "")
  #For GNU/Linux & Mac OSX, we write them in the *pkgsetup.in
  if(WIN32)
    set(my_file_ext ".bat")
    install( PROGRAMS
      Files/otbenv.bat
      Files/otbenv.bash
      DESTINATION ${PKG_STAGE_DIR}
      )
  endif()

  # We also need wrapper script to start mapla & monteverdi
  if(NOT HAVE_MVD)
    message(FATAL_ERROR "not have mvd")
  endif()
  if(HAVE_MVD)
    install(PROGRAMS
      Files/mapla${SCRIPT_EXT}
      Files/monteverdi${SCRIPT_EXT}
      DESTINATION "${PKG_STAGE_DIR}"
      )
  endif()

  install(PROGRAMS
    Files/selftester${SCRIPT_EXT}
    "${SUPERBUILD_INSTALL_DIR}/bin/otbcli${my_file_ext}"
    "${SUPERBUILD_INSTALL_DIR}/bin/otbgui${my_file_ext}"
    DESTINATION ${PKG_STAGE_DIR}/bin)

  if(LINUX)
    install( PROGRAMS "${PATCHELF_PROGRAM}"
      DESTINATION ${PKG_STAGE_DIR})
  endif()

  file(READ "${OTB_BINARY_DIR}/install_manifest.txt"  mfile_lines)
  string(
    REPLACE
    "${CMAKE_INSTALL_PREFIX}"
    "OUT_DIR"
    mfile_lines_NEW ${mfile_lines} )

  file(WRITE "${CMAKE_BINARY_DIR}/install_manifest.txt"
    ${mfile_lines_NEW})

  install(FILES "${CMAKE_BINARY_DIR}/install_manifest.txt"
    DESTINATION ${PKG_STAGE_DIR})

  install(FILES "Files/uninstall_otb.sh"
    DESTINATION ${PKG_STAGE_DIR})

  add_executable(otb_loader ${CMAKE_CURRENT_SOURCE_DIR}/Files/otb_loader.cxx)
  target_link_libraries(otb_loader ${CMAKE_DL_LIBS})
  install(TARGETS otb_loader DESTINATION ${PKG_STAGE_DIR}/bin)
  add_dependencies(otb_loader PATCHELF)
endmacro()
