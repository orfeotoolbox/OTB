macro(superbuild_package)
  cmake_parse_arguments(PACKAGE  "" "OUTDIR;INSTALLDIR" "SEARCHDIRS;PEFILES" ${ARGN} )

  find_program(OBJDUMP_PROGRAM "objdump")

  include(GetPrerequisites)

  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/bin") #exe
  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/lib") #so
  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/lib/otb/applications") #otb apps

  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PACKAGE_INSTALLDIR}/${PACKAGE_OUTDIR}")

  #install share/gdal, otbcli* otbgui*
  install_common()

  list(APPEND PACKAGE_PEFILES ${PACKAGE_INSTALLDIR}/bin/otbApplicationLauncherCommandLine)
  
  list(APPEND PACKAGE_PEFILES ${PACKAGE_INSTALLDIR}/bin/otbTestDriver)

  if(OTB_USE_QT4)
    list(APPEND PACKAGE_PEFILES ${PACKAGE_INSTALLDIR}/bin/otbApplicationLauncherQt)
  endif()
  
  file(GLOB otbapps_list ${PACKAGE_INSTALLDIR}/lib/otb/applications/otbapp_*so) # /lib/otb
  list(APPEND PACKAGE_PEFILES ${otbapps_list})

  set(alldlls)
  set(notfound_dlls)
  foreach(infile ${PACKAGE_PEFILES})
    get_filename_component(bn ${infile} NAME)
    process_deps(${bn})
  endforeach()

  list(LENGTH notfound_dlls nos)
  if(${nos} GREATER 0)
    STRING(REPLACE ".so;" ".so," notfound ${notfound_dlls})
    message(FATAL_ERROR "Following dlls were not found: ${notfound_dlls}. Please consider adding their paths to SEARCHDIRS when calling superbuild_package macro.")
  endif()

  set(PKG_OUTPUT_DIR "${PACKAGE_INSTALLDIR}/${PACKAGE_OUTDIR}")
  configure_file(${SETUP_SCRIPT_SRC}
    ${CMAKE_BINARY_DIR}/pkgsetup @ONLY)

  file(COPY ${CMAKE_BINARY_DIR}/pkgsetup
    DESTINATION ${PACKAGE_OUTDIR}
    FILE_PERMISSIONS
      OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE)
  
endmacro(superbuild_package)

SET(SYSTEM_DLLS
  libm.so
  libc.so
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  )

## http://www.cmake.org/Wiki/CMakeMacroListOperations
macro(IS_SYSTEM_DLL matched value)
  set(${matched})
  foreach (pattern ${SYSTEM_DLLS})
    if(${value} MATCHES ${pattern})
      set(${matched} TRUE)
    endif()
  endforeach()
endmacro()

macro(list_contains var value)
  set(${var})
  foreach(value2 ${ARGN})
    if(${value} STREQUAL ${value2})
      set(${var} TRUE)
    endif()
  endforeach(value2)
endmacro()

macro(install_rpath_code src_filename)

endmacro()

function(process_deps infile)

  get_filename_component(bn ${infile} NAME)
  list_contains(contains "${bn}" "${alldlls}")
  if(NOT contains)
    set(DLL_FOUND FALSE)

    foreach(SEARCHDIR ${PACKAGE_SEARCHDIRS})
      if(NOT DLL_FOUND)
        if(EXISTS ${SEARCHDIR}/${infile})
          set(DLL_FOUND TRUE)
          message(STATUS "Processing ${SEARCHDIR}/${infile}")
          is_file_executable("${SEARCHDIR}/${infile}" is_executable)
          if(is_executable)
            install(FILES "${SEARCHDIR}/${infile}"
              DESTINATION ${PACKAGE_OUTDIR}/bin
              PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
            
          else(is_executable)
            if(${infile} MATCHES "otbapp_")
              install(FILES "${PACKAGE_INSTALLDIR}/lib/otb/applications/${infile}"
                DESTINATION ${PACKAGE_OUTDIR}/lib/otb/applications
                PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
            else()
            get_filename_component(bn_we ${infile} NAME_WE)
            file(GLOB sofiles "${SEARCHDIR}/${bn_we}*")
            foreach(sofile ${sofiles})
              install(FILES "${sofile}"
                DESTINATION ${PACKAGE_OUTDIR}/lib
                PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
            endforeach()
            
            endif()
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

function(install_common)
  set(OUT_DIR "${PACKAGE_OUTDIR}")
  set(BIN_DIR "${OUT_DIR}/bin")
  set(OTBAPPS_DIR "${OUT_DIR}/lib/otb")
  set(DATA_DIR "${OUT_DIR}/share")

  ####################### install GDAL data ###########################
  find_path(GDAL_DATA epsg.wkt ${PACKAGE_INSTALLDIR}/share/gdal)
  install(DIRECTORY ${GDAL_DATA}
    DESTINATION ${DATA_DIR})

  ####################### install patchelf #####################
  install(FILES ${PACKAGE_INSTALLDIR}/tools/patchelf
    DESTINATION ${OUT_DIR}/tools
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
    )

  ####################### install otbcli scripts ######################
  file(GLOB CLI_SCRIPTS ${PACKAGE_INSTALLDIR}/bin/otbcli*)
  foreach(CLI_SCRIPT ${CLI_SCRIPTS})
    install(FILES "${CLI_SCRIPT}"
      DESTINATION ${BIN_DIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
  endforeach()

  ####################### install otbgui scripts ######################
  if(OTB_USE_QT4)
    file(GLOB GUI_SCRIPTS ${PACKAGE_INSTALLDIR}/bin/otbgui*)
    foreach(GUI_SCRIPT ${GUI_SCRIPTS})
      install(FILES "${GUI_SCRIPT}"
        DESTINATION ${BIN_DIR}
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
    endforeach()
  endif()


endfunction()
