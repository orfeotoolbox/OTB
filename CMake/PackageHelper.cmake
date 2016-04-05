set(WINDOWS_SYSTEM_DLLS
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
  shfolder.dll
  secur32.dll
  wsock32.dll
  winspool.drv)

SET(UNIX_SYSTEM_DLLS
  libm.so
  libc.so
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  ld-linux-x86-64.so*
  libX11.so*
  libXext.so*
  libXau.so*
  libXdmcp.so*
  libXxf86vm.so*
  libdrm.so.2
  libGL.so*
  libGLU.so*
  )
set(SYSTEM_DLLS "${UNIX_SYSTEM_DLLS}")
if(WIN32 OR CMAKE_CROSSCOMPILING)
  set(SYSTEM_DLLS "${WINDOWS_SYSTEM_DLLS}")
endif()

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

# Get the translation files coming with Qt, and install them in the bundle
# They are loaded by Monteverdi.
function(get_qt_translation_files RESULT)
    # These files are the "qt_<localename>.qm" files
    # They are located in QT_TRANSLATIONS_DIR, which comes from FindQt4
    file(GLOB translation_files ${QT_TRANSLATIONS_DIR}/qt_*)

    # We need to remove the "qt_help_<localename>.qm" files from this list
    foreach(translation_item ${translation_files})
      if(${translation_item} MATCHES "qt_help")
        list(REMOVE_ITEM translation_files ${translation_item})
      endif()
    endforeach()

    set(${RESULT} ${translation_files} PARENT_SCOPE)
endfunction()

function(install_common include_mvd)

  #a convenient cmake var for storing <prefix>
#  set(PKG_STAGE_DIR "${stage_dir}")

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #root folder where otb applications are installed
  set(PKG_OTBLIBS_DIR "${PKG_STAGE_DIR}/lib/otb")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${DEPENDENCIES_INSTALL_DIR}/share)

  if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "you must set 'OTB_APPS_LIST' before calling this method")
  endif()

  # Just check if required variables are defined.
  foreach(req
      DEPENDENCIES_INSTALL_DIR
      OTB_APPLICATIONS_DIR
      PKG_STAGE_DIR
      CMAKE_INSTALL_PREFIX
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #one for debugging..
#  install(CODE "message(\"CMake/PackageHelper.cmake:install_common(${outdir})\n${vars}\n\")")

  ####################### install cli and gui scripts ###########################
  file(GLOB PKG_APP_SCRIPTS
    ${OTB_INSTALL_DIR}/bin/otbcli*
    ${OTB_INSTALL_DIR}/bin/otbgui*) #

  list(LENGTH PKG_APP_SCRIPTS PKG_APP_SCRIPTS_LENGTH)
  if (PKG_APP_SCRIPTS_LENGTH LESS 1)
    message(WARNING "PKG_APP_SCRIPTS is empty: ${PKG_APP_SCRIPTS}")
  endif()

  ##################### install cli and gui scripts #######################
  install(FILES ${PKG_APP_SCRIPTS}  DESTINATION ${PKG_STAGE_BIN_DIR})

  if(include_mvd)
    install_monteverdi_files()
  endif()

  ####################### install GDAL data ############################
  set(GDAL_DATA ${PKG_SHARE_SOURCE_DIR}/gdal)
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR "Cannot generate package without GDAL_DATA : ${GDAL_DATA} ${DEPENDENCIES_INSTALL_DIR}")
  endif()

  install(DIRECTORY ${GDAL_DATA} DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install GeoTIFF data ###########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/epsg_csv DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### install OSSIM data ###########################
  install(DIRECTORY ${PKG_SHARE_SOURCE_DIR}/ossim DESTINATION ${PKG_SHARE_DEST_DIR})

  ####################### Install otb applications ######################
  install(DIRECTORY "${OTB_APPLICATIONS_DIR}"  DESTINATION ${PKG_OTBLIBS_DIR})

endfunction()

function(install_monteverdi_files)

  #name/ext of qt's sqlite plugin. Varies with platform/OS
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(PKG_QTSQLITE_FILENAME "qsqlite4.dll")
  elseif(APPLE)
    set(PKG_QTSQLITE_FILENAME "qsqlite.dylib")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND NOT CMAKE_CROSSCOMPILING)
    set(PKG_QTSQLITE_FILENAME "libqsqlite.so")
  else()
    message(FATAL_ERROR "install_monteverdi_files: Unknown OS/Platform")
  endif()

  #root folder where qt plugins are installed
  set(PKG_QTPLUGINS_DIR "${PKG_STAGE_DIR}/lib/qt4/plugins")

  #qt4's distributes some translation of common message string used in Qt.
  #This are provided with any qt installation. We reuse them in otb package
  #so as not to reinvent the wheels.
  set(PKG_I18N_DIR "${PKG_STAGE_DIR}/lib/qt4/translations")

  #<prefix>/share for otb i18n directory. This is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations.
  set(PKG_OTB_I18N_DIR "${PKG_STAGE_DIR}/${Monteverdi_INSTALL_DATA_DIR}/i18n")

  # Just check if required variables are defined.
  foreach(req
      Monteverdi_SOURCE_DIR
      QT_PLUGINS_DIR
      PKG_STAGE_BIN_DIR
      PKG_QTSQLITE_FILENAME
      PKG_QTPLUGINS_DIR
      PKG_I18N_DIR
      PKG_OTB_I18N_DIR
      )
    if(NOT DEFINED ${req} OR "${${req}}" STREQUAL "")
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #message("CMake/PackageHelper.cmake:install_common(${stage_dir})\n${vars}\n")

  ##################### install mingw otbenv.cmd ##########################
  if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/otbenv.cmd)
    install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/otbenv.cmd
      DESTINATION ${PKG_STAGE_DIR})
  endif()

  ####################### install mingw qt.conf ##########################
  if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/qt.conf)
    install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/mingw/qt.conf
      DESTINATION ${PKG_STAGE_BIN_DIR})
  endif()

  ####################### install sqldriver plugin ########################
  install(FILES ${QT_PLUGINS_DIR}/sqldrivers/${PKG_QTSQLITE_FILENAME}
    DESTINATION ${PKG_QTPLUGINS_DIR}/sqldrivers)

  ####################### install translations ###########################
  #get all translations already distributed with qt4
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  #install all files in ${QT_TRANSLATIONS_FILES}
  install(FILES ${QT_TRANSLATIONS_FILES}  DESTINATION ${PKG_I18N_DIR})

  #translation of monteverdi specific strings
  file(GLOB APP_TS_FILES ${Monteverdi_SOURCE_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${Monteverdi_BINARY_DIR}/i18n/${APP_TS_FILENAME}.qm
      DESTINATION ${PKG_OTB_I18N_DIR})
  endforeach()

endfunction()

macro(clear_package_staging_directory)
  message(STATUS "Clearing package staging directory: ${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_INSTALL_PREFIX}/${PKG_STAGE_DIR}")
endmacro()

function(configure_package)

  set(EXE_EXT "")
  set(LIB_EXT "*so")
  set(SCR_EXT ".sh")
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(EXE_EXT ".exe")
    set(LIB_EXT "*dll")
    set(SCR_EXT ".bat")
  endif()

  foreach(EXE_FILE
      monteverdi
      mapla)
    if(EXISTS "${CMAKE_INSTALL_PREFIX}/bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${CMAKE_INSTALL_PREFIX}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
    if(DEFINED Monteverdi_SOURCE_DIR)
      if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${EXE_FILE}${SCR_EXT})
        install(FILES
          ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${EXE_FILE}${SCR_EXT}
          DESTINATION
          "${PKG_STAGE_DIR}/bin")
    endif()
  endif()

  endforeach()

  foreach(EXE_FILE otbApplicationLauncherQt
      iceViewer
      otbTestDriver
      otbApplicationLauncherCommandLine)
    if(EXISTS "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
      list(APPEND PKG_PEFILES
        "${OTB_INSTALL_DIR}/bin/${EXE_FILE}${EXE_EXT}")
    endif()
  endforeach()

  file(GLOB OTB_APPS_LIST ${OTB_APPLICATIONS_DIR}/otbapp_${LIB_EXT}) # /lib/otb

  set(include_mvd 0)
  if(DEFINED Monteverdi_SOURCE_DIR)
    set(include_mvd 1)
  endif()

  install_common(${include_mvd})

  list(APPEND PKG_PEFILES ${OTB_APPS_LIST})

  set(alldlls)
  set(notfound_dlls)
  foreach(infile ${PKG_PEFILES})
    get_filename_component(bn ${infile} NAME)
    process_deps(${bn})
  endforeach()

  list(LENGTH notfound_dlls nos)
  if(${nos} GREATER 0)
    STRING(REPLACE ".so;" ".so," notfound ${notfound_dlls})
    message(FATAL_ERROR "Following dlls were not found: ${notfound_dlls}. Please consider adding their paths to SEARCHDIRS when calling superbuild_package macro.")
  endif()

endfunction()