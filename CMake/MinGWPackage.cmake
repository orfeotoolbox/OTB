macro(package_mingw)
  cmake_parse_arguments(PKG  "" "STAGE_DIR;ARCH;MXEROOT" "SEARCHDIRS" ${ARGN} )
  if("${PKG_ARCH}" STREQUAL "x86")
    set(DEPENDENCIES_INSTALL_DIR "${PKG_MXEROOT}/usr/i686-w64-mingw32.shared")
    set(MXE_OBJDUMP "${PKG_MXEROOT}/usr/bin/i686-w64-mingw32.shared-objdump")
  elseif("${PKG_ARCH}" STREQUAL "x64")
    set(DEPENDENCIES_INSTALL_DIR "${PKG_MXEROOT}/usr/x86_64-w64-mingw32.shared")
    set(MXE_OBJDUMP "${PKG_MXEROOT}/usr/bin/x86_64-w64-mingw32.shared-objdump")
  endif()

#  message()
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

  clear_package_staging_directory()

  set(PKG_PEFILES)

  configure_package()

endmacro(package_mingw)


function(process_deps infile)

  get_filename_component(bn ${infile} NAME)

  list_contains(contains "${bn}" "${alldlls}")
  if(NOT contains)
    set(DLL_FOUND FALSE)
    foreach(SEARCHDIR ${PKG_SEARCHDIRS})
      if(NOT DLL_FOUND)
        if(EXISTS ${SEARCHDIR}/${infile})
          set(DLL_FOUND TRUE)
        else()
          string(TOLOWER ${infile} infile_lower)
          if(EXISTS ${SEARCHDIR}/${infile_lower})
            set(DLL_FOUND TRUE)
            set(infile ${infile_lower})
          endif()
        endif()
        if(DLL_FOUND)
          message(STATUS "Processing ${SEARCHDIR}/${infile}")
          if(NOT "${infile}" MATCHES "otbapp")
            install(FILES "${SEARCHDIR}/${infile}"
              DESTINATION ${PKG_STAGE_DIR}/bin)
          else()
            ##message(STATUS "skipping..${infile}")
          endif()
          if(NOT EXISTS ${MXE_OBJDUMP})
            message(FATAL_ERROR "objdump executable not found. please check MXE_OBJDUMP is set to correct cross compiled executable")
          endif()
          execute_process(COMMAND ${MXE_OBJDUMP} "-p" "${SEARCHDIR}/${infile}"  OUTPUT_VARIABLE dlldeps)
          string(REGEX MATCHALL "DLL.Name..[A-Za-z(0-9\\.0-9)+_\\-]*" OUT "${dlldeps}")
          string(REGEX REPLACE "DLL.Name.." "" OUT "${OUT}")
          foreach(o ${OUT})
            process_deps(${o})
          endforeach()
        endif()
      endif(NOT DLL_FOUND)
    endforeach()

    if(NOT DLL_FOUND)
      is_system_dll(iss "${infile}")
      if(NOT iss)
        set(notfound_dlls "${notfound_dlls};${infile}")
      endif()
    else(NOT DLL_FOUND)

      set( alldlls "${alldlls};${bn}" PARENT_SCOPE )
    endif(NOT DLL_FOUND)

    set(notfound_dlls "${notfound_dlls}" PARENT_SCOPE )
   endif()
endfunction()