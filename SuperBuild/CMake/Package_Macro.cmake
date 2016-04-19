macro(superbuild_package)
  cmake_parse_arguments(PKG  "" "STAGE_DIR;XDK" "SEARCHDIRS" ${ARGN} )

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

  #CMAKE_INSTALL_PREFIX -> SB_INSTALL_PREFIX
  #"${PKG_MXEROOT}/usr/x86_64-w64-mingw32.shared/bin")

  #a helper variable. rethink
  set(DEPENDENCIES_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
  set(OTB_APPLICATIONS_DIR "${OTB_INSTALL_DIR}/lib/otb/applications")

  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/bin") #exe
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib") #so
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib/otb") #mvd so
  list(APPEND PKG_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/lib/otb/applications") #otb apps

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
    set(PKGSETUP_IN_FILENAME osx_pkgsetup.in)
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

  if(APPLE)
    if( "${infile}" MATCHES "@rpath")
      string(REGEX REPLACE "@rpath." "" infile "${infile}")
    endif()
  endif()
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
            install(PROGRAMS "${SEARCHDIR}/${infile}"
              DESTINATION ${PKG_STAGE_DIR}/bin)
          else(is_executable)
            get_filename_component(bn_we ${infile} NAME_WE)
            file(GLOB sofiles "${SEARCHDIR}/${bn_we}*")
            foreach(sofile ${sofiles})
              get_filename_component(sofile_ext ${sofile} EXT)
              set(is_valid TRUE)
              if ("${sofile_ext}" MATCHES ".la"
                  OR "${sofile_ext}" MATCHES ".prl"
                  OR "${sofile_ext}" MATCHES ".a")
                set(is_valid FALSE)
              endif()

              if(is_valid)
                get_filename_component(basename_of_sofile ${sofile} NAME)
                is_file_a_symbolic_link("${sofile}" is_symlink linked_to_file)
                if(is_symlink)
                  # NOTE: $OUT_DIR is set actually in pkgsetup.in. So don't try
                  # any pre-mature optimization on that variable names
                  file(APPEND
                    ${CMAKE_BINARY_DIR}/make_symlinks
                    "ln -sf $OUT_DIR/lib/${linked_to_file} $OUT_DIR/lib/${basename_of_sofile}\n"
                    )
                  #message("${sofile} is a symlink to ${linked_to_file}")
                else() # is_symlink
                  if("${basename_of_sofile}" MATCHES "otbapp_")
                    if(NOT MAKE_XDK)
                      install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib/otb/applications)
                    endif()
                  else() #if(.. MATCHES "otbapp_")
                    #if we are making xdk. skill all those starting with libotb case insensitively
                    if(MAKE_XDK)
                      string(TOLOWER "${basename_of_sofile}" sofile_lower )
                      if(NOT "${sofile_lower}" MATCHES "libotb")
                        install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib)
                      endif()
                    else() #MAKE_XDK
                      #just install the so file to <staging-dir>/lib
                      install(FILES "${sofile}" DESTINATION ${PKG_STAGE_DIR}/lib)
                    endif() #MAKE_XDK

                    # Finally touch a file in temp directory for globbing later
                    # message("touching ${basename_of_sofile}")
                    execute_process(COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/temp_so_names_dir/${basename_of_sofile}")
                  endif() #if(.. MATCHES "otbapp_")
                endif() #is_symlink
              endif() #is_valid
            endforeach()
          endif(is_executable)
          execute_process(
            COMMAND ${LOADER_PROGRAM} ${LOADER_PROGRAM_ARGS} "${SEARCHDIR}/${infile}"
            RESULT_VARIABLE loader_rv
            OUTPUT_VARIABLE loader_ov
            ERROR_VARIABLE loader_ev
            )
          if(loader_rv)
            message(FATAL_ERROR "loader_ev=${loader_ev}\n PACKAGE-OTB: result_variable is '${loader_rv}'")
          endif()

          if(APPLE)
            string(REGEX REPLACE "[^\n]+cmd LC_LOAD_DYLIB\n[^\n]+\n[^\n]+name ([^\n]+).\\(offset[^\n]+\n" "rpath \\1\n" loader_ov "${loader_ov}")
            string(REGEX MATCHALL "rpath [^\n]+" loader_ov "${loader_ov}")
            string(REGEX REPLACE "rpath " "" needed_dlls "${loader_ov}")
          else()
            string(REGEX MATCHALL "NEEDED\\ *[A-Za-z(0-9\\.0-9)+_\\-]*" loader_ov "${loader_ov}")
            string(REGEX REPLACE "NEEDED" "" needed_dlls "${loader_ov}")
          endif()

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
