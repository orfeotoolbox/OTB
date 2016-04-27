macro(superbuild_package)
  cmake_parse_arguments(PKG  "" "STAGE_DIR" "SEARCHDIRS" ${ARGN} )

  if(APPLE)
    set(loader_program_names otool)
    set(LOADER_PROGRAM_ARGS "-l")
  else()
    set(loader_program_names objdump)
    set(LOADER_PROGRAM_ARGS "-p")
  endif()
  find_program(LOADER_PROGRAM "${loader_program_names}")
  if(NOT EXISTS ${LOADER_PROGRAM})
    message(FATAL_ERROR "${loader_program_names} not found. please check LOADER_PROGRAM variable is set correctly")
  endif()

  include(GetPrerequisites)

  set(DEPENDENCIES_INSTALL_DIR ${PKG_INSTALL_PREFIX})
  set(OTB_INSTALL_DIR ${PKG_INSTALL_PREFIX})

  set(PKG_SEARCHDIRS)
  set(OTB_APPLICATIONS_DIR "${OTB_INSTALL_DIR}/lib/otb/applications")
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/bin") #exe
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/lib") #so
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/lib/otb") #mvd so
  list(APPEND PKG_SEARCHDIRS "${OTB_APPLICATIONS_DIR}") #otb apps

  empty_package_staging_directory()

  file(WRITE
    ${CMAKE_BINARY_DIR}/make_symlinks
    "#!/bin/sh\n")

  set(PKG_PEFILES)
  #NOTE: VAR_IN_PKGSETUP_CONFIGURE is copied to linux_pkgsetup.in during configure_file
  set(VAR_IN_PKGSETUP_CONFIGURE)
  set(PKG_SO_FILES)
  configure_package()

  ############# install client configure script ################

  set(PKGSETUP_IN_FILENAME linux_pkgsetup.in)
  if(APPLE)
    set(PKGSETUP_IN_FILENAME macx_pkgsetup.in)
  endif()
  configure_file(${PACKAGE_SUPPORT_FILES_DIR}/${PKGSETUP_IN_FILENAME}
    ${CMAKE_BINARY_DIR}/pkgsetup @ONLY)

  install(FILES
    ${CMAKE_BINARY_DIR}/pkgsetup
    ${CMAKE_BINARY_DIR}/make_symlinks
    DESTINATION ${PKG_STAGE_DIR}
    PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

  if(UNIX)
    if(NOT APPLE)
      ####################### install patchelf #####################
      install(FILES ${PKG_INSTALL_PREFIX}/tools/patchelf
        DESTINATION ${PKG_STAGE_DIR}/tools
        PERMISSIONS
        OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ)
    endif()
  endif()

  if(PKG_GENERATE_XDK)
    install_xdk_files()
  endif()

endmacro(superbuild_package)


function(install_xdk_files)
  install(DIRECTORY ${DEPENDENCIES_INSTALL_DIR}/share
    DESTINATION ${PKG_STAGE_DIR})

  install(DIRECTORY ${DEPENDENCIES_INSTALL_DIR}/include
    DESTINATION ${PKG_STAGE_DIR}
    PATTERN "include/OTB*" EXCLUDE )

  install(DIRECTORY ${DEPENDENCIES_INSTALL_DIR}/lib/cmake
    DESTINATION ${PKG_STAGE_DIR}/lib/
    PATTERN "lib/cmake/OTB*" EXCLUDE)
endfunction()
