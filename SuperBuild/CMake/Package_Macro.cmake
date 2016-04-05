macro(superbuild_package)
  cmake_parse_arguments(PKG  "" "STAGE_DIR;XDK" "SEARCHDIRS" ${ARGN} )

  find_program(OBJDUMP_PROGRAM "objdump")

  include(GetPrerequisites)

  #CMAKE_INSTALL_PREFIX -> SB_INSTALL_PREFIX
  #"${PKG_MXEROOT}/usr/x86_64-w64-mingw32.shared/bin")

  #a helper variable. rethink
  set(DEPENDENCIES_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
  set(OTB_APPLICATIONS_DIR "${OTB_INSTALL_DIR}/lib/otb/applications")

  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/bin") #exe
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib") #so
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib/otb") #mvd so
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib/otb/applications") #otb apps

  clear_package_staging_directory()

  set(PKG_PEFILES)

  configure_package()

  ############# install client configure script ################
  configure_file(${PACKAGE_CMAKE_SOURCE_DIR}/pkgsetup.in
    ${CMAKE_BINARY_DIR}/pkgsetup @ONLY)

  install(FILES ${CMAKE_BINARY_DIR}/pkgsetup
    DESTINATION ${PKG_STAGE_DIR}
    PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE)

  ####################### install patchelf #####################
  install(FILES ${CMAKE_INSTALL_PREFIX}/tools/patchelf
    DESTINATION ${PKG_STAGE_DIR}/tools
    PERMISSIONS
    OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ)


  if(PKG_XDK)
    install_xdk_files()
  endif()

endmacro(superbuild_package)


function(process_deps infile)

  get_filename_component(bn ${infile} NAME)
  list_contains(contains "${bn}" "${alldlls}")
  if(NOT contains)
    set(DLL_FOUND FALSE)

    foreach(SEARCHDIR ${PKG_SEARCHDIRS})
      if(NOT DLL_FOUND)
        if(EXISTS ${SEARCHDIR}/${infile})
          set(DLL_FOUND TRUE)
          message(STATUS "Processing ${SEARCHDIR}/${infile}")
          is_file_executable("${SEARCHDIR}/${infile}" is_executable)
          if(is_executable)
            install(FILES "${SEARCHDIR}/${infile}"
              DESTINATION ${PKG_STAGE_DIR}/bin
              PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)

          else(is_executable)
            if(NOT MAKE_XDK)
              if(${infile} MATCHES "otbapp_")
                install(FILES "${CMAKE_INSTALL_PREFIX}/lib/otb/applications/${infile}"
                  DESTINATION ${PKG_STAGE_DIR}/lib/otb/applications
                  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
              endif()
            endif() # MAKE_XDK
            get_filename_component(bn_we ${infile} NAME_WE)
            file(GLOB sofiles "${SEARCHDIR}/${bn_we}*")
            foreach(sofile ${sofiles})
              if(MAKE_XDK)
                get_filename_component(bn_we_sofile ${sofile} NAME)
                string(TOLOWER "${bn_we_sofile}" sofile_lower )
                if(NOT "${sofile_lower}" MATCHES "otb")
                  install(FILES "${sofile}"
                    DESTINATION ${PKG_STAGE_DIR}/lib
                    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
                endif()
              else() #MAKE_XDK
                install(FILES "${sofile}"
                  DESTINATION ${PKG_STAGE_DIR}/lib
                  PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
              endif() #MAKE_XDK
            endforeach()
          endif(is_executable)

          if(NOT EXISTS ${OBJDUMP_PROGRAM})
            message(FATAL_ERROR "objdump executable not found. please check OBJDUMP_PROGRAM is set to correct cross compiled executable")
          endif()
          execute_process(COMMAND ${OBJDUMP_PROGRAM} "-p" "${SEARCHDIR}/${infile}"  OUTPUT_VARIABLE dump_out)
          string(REGEX MATCHALL "NEEDED\\ *[A-Za-z(0-9\\.0-9)+_\\-]*" needed_dlls "${dump_out}")
          string(REGEX REPLACE "NEEDED" "" needed_dlls "${needed_dlls}")

          foreach(needed_dll ${needed_dlls})
            string(STRIP ${needed_dll} needed_dll)
            process_deps(${needed_dll})
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
