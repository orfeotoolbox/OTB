macro(package_mingw)
  cmake_parse_arguments(PACKAGE  "" "PREFIX_DIR;ARCH;MXEROOT;NEEDS_OTB_APPS" "SEARCHDIRS;PEFILES" ${ARGN} )
  if("${PACKAGE_ARCH}" STREQUAL "x86")
    set(MXE_BIN_DIR "${PACKAGE_MXEROOT}/usr/i686-w64-mingw32.shared/bin")
    set(MXE_OBJDUMP "${PACKAGE_MXEROOT}/usr/bin/i686-w64-mingw32.shared-objdump")
  elseif("${PACKAGE_ARCH}" STREQUAL "x64")
    set(MXE_BIN_DIR "${PACKAGE_MXEROOT}/usr/x86_64-w64-mingw32.shared/bin")
    set(MXE_OBJDUMP "${PACKAGE_MXEROOT}/usr/bin/x86_64-w64-mingw32.shared-objdump")
  endif()

  file(GLOB MXE_GCC_LIB_DIR "${MXE_BIN_DIR}/gcc*")
  list(APPEND PACKAGE_SEARCHDIRS ${MXE_GCC_LIB_DIR})
  list(APPEND PACKAGE_SEARCHDIRS ${MXE_BIN_DIR})
  list(APPEND PACKAGE_SEARCHDIRS "${MXE_BIN_DIR}/../qt/bin") #Qt
  list(APPEND PACKAGE_SEARCHDIRS "${MXE_BIN_DIR}/../qt/lib") #Qwt
  list(APPEND PACKAGE_SEARCHDIRS "${CMAKE_INSTALL_PREFIX}/bin") #mvd

  install_common(${PACKAGE_PREFIX_DIR} ${PACKAGE_NEEDS_OTB_APPS})
  if(PACKAGE_NEEDS_OTB_APPS)
    list(APPEND PACKAGE_SEARCHDIRS "${OTB_MODULES_DIR}/../../../otb/applications") #otb apps
  endif()

    foreach(exe_file ${PACKAGE_PEFILES})
      get_filename_component(base_name ${exe_file} NAME_WE)
      install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${base_name}.bat
        DESTINATION ${PACKAGE_PREFIX_DIR}/bin)
  endforeach()


  #qt4 translations
  file(GLOB APP_TS_FILES ${Monteverdi_SOURCE_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${Monteverdi_BINARY_DIR}/i18n/${APP_TS_FILENAME}.qm
      DESTINATION ${PACKAGE_PREFIX_DIR}/${Monteverdi_INSTALL_DATA_DIR}/i18n)
  endforeach()

  #dependency resolution based on copydlldeps.sh from mxe by Timothy Gu
  if(PACKAGE_NEEDS_OTB_APPS)
    file(GLOB otbapps_list ${OTB_MODULES_DIR}/../../../otb/applications/otbapp_*dll) # /lib/otb
    list(APPEND PACKAGE_PEFILES ${otbapps_list})
  endif()

  set(alldlls)
  set(notfound_dlls)
  foreach(infile ${PACKAGE_PEFILES})
    get_filename_component(bn ${infile} NAME)
    process_deps(${bn})
  endforeach()

  list(LENGTH notfound_dlls nos)
  if(${nos} GREATER 0)
    string(REPLACE ".dll" ".dll," notfound ${notfound_dlls})
    message(FATAL_ERROR "Following dlls were not found: ${notfound}
                   Please consider adding their paths to SEARCHDIRS when calling package_mingw macro.")
  endif()

endmacro(package_mingw)

set(SYSTEM_DLLS
  msvc.*dll
  user32.dll
  gdi32.dll
  shell32.dll
  kernel32.dll
  advapi32.dll
  crypt32.dll
  ws2_32.dll
  wldap32.dll
  ole32.dll
  opengl32.dll
  glu32.dll
  comdlg32.dll
  imm32.dll
  oleaut32.dll
  comctl32.dll
  winmm.dll
  secur32.dll
  libeay32.dll
  shfolder.dll
  ssleay32.dll
  wsock32.dll
  winspool.drv)

## http://www.cmake.org/Wiki/CMakeMacroListOperations
macro(is_system_dll matched value)
  set(${matched})
  string(TOLOWER ${value} value_)
  foreach (pattern ${SYSTEM_DLLS})
    string(TOLOWER ${pattern} pattern_)
    if(${value_} MATCHES ${pattern_})
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
          if(NOT "${infile}" MATCHES "otbapp")
            install(FILES "${SEARCHDIR}/${infile}"
              DESTINATION ${PACKAGE_PREFIX_DIR}/bin)
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

function(install_common outdir need_otb_apps)
  set(APP_PREFIX_DIR "${outdir}")
  set(APP_BIN_DIR "${APP_PREFIX_DIR}/bin")
  set(APP_QTSQLITE_FILENAME "qsqlite4.dll")
  set(APP_QTPLUGINS_DIR "${APP_PREFIX_DIR}/lib/qt4/plugins")
  set(APP_OTBLIBS_DIR "${APP_PREFIX_DIR}/lib/otb")
  set(APP_I18N_DIR "${APP_PREFIX_DIR}/lib/qt4/translations")
  set(APP_DATA_DIR "${APP_PREFIX_DIR}/share")

  install(CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${APP_BIN_DIR}/qt.conf\"
                \"[Paths]
                  Translations=../lib/qt4/translations
                  Plugins=../lib/qt4/plugins
                \")" )

  install(FILES ${QT_PLUGINS_DIR}/sqldrivers/${APP_QTSQLITE_FILENAME}
    DESTINATION ${APP_QTPLUGINS_DIR}/sqldrivers)

  ####################### install translations #######################
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  install(FILES ${QT_TRANSLATIONS_FILES}
    DESTINATION ${APP_I18N_DIR})

  ####################### install GDAL data #######################

  set(GDAL_DATA ${MXE_BIN_DIR}/../share/gdal)
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR "Cannot generate package without GDAL_DATA : ${GDAL_DATA}")
  endif()

  install(DIRECTORY ${GDAL_DATA}
    DESTINATION ${APP_DATA_DIR})

  ####################### Check otb applications #######################
  if(need_otb_apps)
    file(GLOB OTB_APPS_LIST ${OTB_MODULES_DIR}/../../../otb/applications/otbapp_*dll) # /lib/otb
    if(NOT OTB_APPS_LIST)
      message(FATAL_ERROR "No OTB-applications detected")
    endif()

    ## otb apps dir /lib/otb/applications
    install(DIRECTORY "${OTB_MODULES_DIR}/../../../otb/applications"
      DESTINATION ${APP_OTBLIBS_DIR})
  endif()

endfunction()
