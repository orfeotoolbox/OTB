macro(superbuild_package)
  cmake_parse_arguments(PACKAGE  "" "OUTDIR;INSTALLDIR" "SEARCHDIRS;PEFILES" ${ARGN} )

  find_program(OBJDUMP_PROGRAM "objdump")
  set(PATCHELF_PROGRAM "/home/mrashad/build/patchelf-0.8/src/patchelf")

  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/bin") #exe
  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/lib") #so
  list(APPEND PACKAGE_SEARCHDIRS "${PACKAGE_INSTALLDIR}/lib/otb/applications") #otb apps

  install_common()

  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PACKAGE_INSTALLDIR}/${PACKAGE_OUTDIR}")

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
     message(FATAL_ERROR "Following dlls were not found: ${notfound_dlls}
Please consider adding their paths to SEARCHDIRS when calling superbuild_package macro.")
  endif()

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
          add_custom_target(RPATH_${bn}
            INSTALL
            POST_BUILD
            COMMAND ${PATCHELF_PROGRAM} "--set-rpath" "../lib" "${PACKAGE_OUTDIR}/lib/${infile}")

          file(GLOB sofiles "${SEARCHDIR}/${bn}*")
            foreach(sofile ${sofiles})
            install(FILES "${sofile}"
              DESTINATION ${PACKAGE_OUTDIR}/bin)
            endforeach()

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
  set(BIN_DIR "${OUTDIR}/bin")
  set(OTBLIBS_DIR "${OUTDIR}/lib/otb")
  set(DATA_DIR "${OUTDIR}/share")

  ####################### install GDAL data #######################
  find_path(GDAL_DATA epsg.wkt ${PACKAGE_INSTALLDIR}/share/gdal)
  install(DIRECTORY ${GDAL_DATA}
    DESTINATION ${DATA_DIR})

  ####################### Check otb applications #######################

  file(GLOB OTB_APPS_LIST ${PACKAGE_INSTALLDIR}/lib/otb/applications/otbapp_*so) # /lib/otb
  if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "No OTB-applications detected")
  endif()

  ####################### install otb applications #######################
  install(DIRECTORY "${PACKAGE_INSTALLDIR}/lib/otb/applications"
    DESTINATION ${OTBLIBS_DIR})

  ####################### install otbcli scripts #######################
  file(GLOB CLI_SCRIPTS ${SB_INSTALL_DIR}/bin/otbcli*)
  foreach(CLI_SCRIPT ${CLI_SCRIPTS})
    install(FILES "${CLI_SCRIPT}"
      DESTINATION ${BIN_DIR})
  endforeach()

  ####################### install otbgui scripts #######################
  if(OTB_USE_QT4)
    file(GLOB GUI_SCRIPTS ${SB_INSTALL_DIR}/bin/otbgui*)
    foreach(GUI_SCRIPT ${GUI_SCRIPTS})
      install(FILES "${GUI_SCRIPT}"
        DESTINATION ${BIN_DIR})
    endforeach()
  endif()


endfunction()
