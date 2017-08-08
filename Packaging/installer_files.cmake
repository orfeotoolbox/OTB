#thi macro calls add_uninstall_otb_script macro(another file)
macro(installer_files)

  #configure README from rst docs
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

  #configure pkgsetup for mac and linux
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

  #start installing script and other stuff related to package
  if(UNIX)
    install( PROGRAMS
      ${CMAKE_BINARY_DIR}/pkgsetup
      ${CMAKE_BINARY_DIR}/make_symlinks
      Files/setup_python.sh
      Files/otbenv.profile
      DESTINATION ${PKG_STAGE_DIR}
      )
  endif()

  # We also need wrapper script to start mapla & monteverdi
  #if(NOT HAVE_MVD)
  #  message(FATAL_ERROR "not have mvd")
  #endif()
  if(HAVE_MVD)
    install(PROGRAMS
      Files/mapla${SCRIPT_EXT}
      Files/monteverdi${SCRIPT_EXT}
      DESTINATION ${PKG_STAGE_DIR}
      )

    #for mac osx, we have a template.app which is
    # later processed in pkgsetup (macx_pkgsetup.in)
    if(APPLE)
      install(DIRECTORY Files/template.app
	DESTINATION "${PKG_STAGE_DIR}")
    endif()
  endif() #HAVE_MVD

  #my_file_ext is required for otbcli and otbgui files which has
  # no extension on linux. (hence we cannot use SCRIPT_EXT

  #For windows otbenv.bash and otbenv.bat is required.
  # Former is for windows shell users and latter is for other
  # linux emulator such as bash for windows or mobxterm
  set(my_file_ext "")
  if(WIN32)
    set(my_file_ext ".bat")
    install( PROGRAMS
      Files/otbenv.bat
      Files/otbenv.bash
      DESTINATION ${PKG_STAGE_DIR}
      )
  endif()

  #these scripts are not auto-installed in PKG_STAGE_DIR
  #could this be move them to somewhere else?
  install(PROGRAMS
    "${SUPERBUILD_INSTALL_DIR}/bin/otbcli${my_file_ext}"
    "${SUPERBUILD_INSTALL_DIR}/bin/otbgui${my_file_ext}"
    DESTINATION ${PKG_STAGE_DIR}/bin)

  install(PROGRAMS
    Files/selftester${SCRIPT_EXT}
    Files/uninstall_otb${SCRIPT_EXT}
    DESTINATION ${PKG_STAGE_DIR}/tools)

  # only required for linux.
  # THIS WILL BE REMOVED ONCE INSTALLATION IS DONE
  # USE --noexec option when running .run file to keep this file
  # along with other installation files
  if(LINUX)
    install(PROGRAMS
      "${PATCHELF_PROGRAM}"
      DESTINATION ${PKG_STAGE_DIR})
  endif()

  
  #add otb_loader
  add_executable(otb_loader ${CMAKE_CURRENT_SOURCE_DIR}/Files/otb_loader.cxx)
  target_link_libraries(otb_loader ${CMAKE_DL_LIBS})
  install(TARGETS otb_loader DESTINATION ${PKG_STAGE_DIR}/tools)
  add_dependencies(otb_loader PATCHELF)
endmacro()
