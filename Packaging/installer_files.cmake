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
    install( PROGRAMS "${CMAKE_BINARY_DIR}/pkgsetup" DESTINATION ${PKG_STAGE_DIR})
    install( PROGRAMS "${CMAKE_BINARY_DIR}/make_symlinks" DESTINATION ${PKG_STAGE_DIR})
  endif()

  ########### install patchelf( linux only) ##################
  if(LINUX)
    install( PROGRAMS "${PATCHELF_PROGRAM}" DESTINATION ${PKG_STAGE_DIR})
  endif()


  ################## otb_loader executable ###################
  add_executable(otb_loader ${CMAKE_CURRENT_SOURCE_DIR}/Files/otb_loader.cxx)
  target_link_libraries(otb_loader ${CMAKE_DL_LIBS})
  install(TARGETS otb_loader DESTINATION ${PKG_STAGE_DIR}/bin)
  add_dependencies(otb_loader PATCHELF)
endmacro()
