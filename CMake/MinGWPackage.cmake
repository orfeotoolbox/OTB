macro(package_mingw)
  cmake_parse_arguments(PKG  "" "STAGE_DIR;ARCH;MXEROOT" "SEARCHDIRS" ${ARGN} )

  set(loader_program_PATHS)
  if(WIN32 OR CMAKE_CROSSCOMPILING)
      set(loader_program_names "${PKG_ARCH}-w64-mingw32.shared-objdump")
      set(loader_program_PATHS "${PKG_MXEROOT}/usr/bin")
      set(DEPENDENCIES_INSTALL_DIR "${PKG_MXEROOT}/usr/${PKG_ARCH}-w64-mingw32.shared")
      set(LOADER_PROGRAM_ARGS "-p")
  else()
    set(loader_program_PATHS /usr/bin) # a path that is already listed in default path on unix
  endif()

  find_program(LOADER_PROGRAM "${loader_program_names}" PATHS ${loader_program_PATHS})
  if(NOT EXISTS ${LOADER_PROGRAM})
    message(FATAL_ERROR "${loader_program_names} not found in ${loader_program_PATHS}. please check LOADER_PROGRAM variable is set correctly")
  endif()

  include(GetPrerequisites)

  #guess install directory from OTB_MODULES_DIR
  set(OTB_INSTALL_DIR ${OTB_MODULES_DIR}/../../../..)
  set(OTB_APPLICATIONS_DIR ${OTB_MODULES_DIR}/../../../otb/applications)

  set(MXE_BIN_DIR "${DEPENDENCIES_INSTALL_DIR}/bin")
  file(GLOB MXE_GCC_LIB_DIR "${MXE_BIN_DIR}/gcc*")
  list(APPEND PKG_SEARCHDIRS ${MXE_GCC_LIB_DIR})
  list(APPEND PKG_SEARCHDIRS ${MXE_BIN_DIR})
  list(APPEND PKG_SEARCHDIRS "${MXE_BIN_DIR}/../qt/bin") #Qt
  list(APPEND PKG_SEARCHDIRS "${MXE_BIN_DIR}/../qt/lib") #Qwt
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/bin") #mvd
  list(APPEND PKG_SEARCHDIRS "${OTB_APPLICATIONS_DIR}") #otb apps
  list(APPEND PKG_SEARCHDIRS "${OTB_INSTALL_DIR}/bin") #otbApplicationLauncher*.exe

  empty_package_staging_directory()

  set(PKG_PEFILES)

  configure_package()

endmacro(package_mingw)


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
