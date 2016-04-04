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


function(install_common outdir)

  #a convenient cmake var for storing <prefix>
  set(PKG_PREFIX_DIR "${outdir}")

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_BIN_DIR "${PKG_PREFIX_DIR}/bin")

  #name of qt's sqlite plugin. Varies with platform/OS
  set(PKG_QTSQLITE_FILENAME "qsqlite4.dll")

  #root folder where qt plugins are installed
  set(PKG_QTPLUGINS_DIR "${PKG_PREFIX_DIR}/lib/qt4/plugins")

  #root folder where otb applications are installed
  set(PKG_OTBLIBS_DIR "${PKG_PREFIX_DIR}/lib/otb")

  #qt4's distributes some translation of common message string used in Qt.
  #This are provided with any qt installation. We reuse them in otb package
  #so as not to reinvent the wheels.
  set(PKG_I18N_DIR "${PKG_PREFIX_DIR}/lib/qt4/translations")

  #<prefix>/share for otb i18n directory. This is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations.
  set(PKG_OTB_I18N_DIR "${PACKAGE_PREFIX_DIR}/${Monteverdi_INSTALL_DATA_DIR}/i18n")

  #<prefix>/share for gdal data files
  set(PKG_DATA_DIR "${PKG_PREFIX_DIR}/share")

  #place to install where qt.conf
  set(PKG_QTCONF_DIR  "${CMAKE_INSTALL_PREFIX}/${PKG_BIN_DIR}")

  #a convenient cmake var to store the otb's install prefix
  #guess install directory from OTB_MODULES_DIR
  set(OTB_INSTALL_DIR ${OTB_MODULES_DIR}/../../../..)

  # Just check if required variables are defined.
  foreach(req
      Monteverdi_SOURCE_DIR
      Monteverdi_BINARY_DIR
      MXE_BIN_DIR
      OTB_MODULES_DIR
      PACKAGE_PEFILES
      QT_PLUGINS_DIR
      CMAKE_INSTALL_PREFIX
      PKG_QTCONF_DIR
      OTB_INSTALL_DIR
      )
    if(NOT DEFINED ${req})
      message(FATAL_ERROR "you must set ${req} before calling this method")
    endif()
    set(vars "${vars}  ${req}=[${${req}}]\n")
  endforeach(req)

  #one for debugging..
  install(CODE "message(\"CMake/PackageHelper.cmake:install_common(${outdir})\n${vars}\n\")")

  ####################### install cli and gui scripts ###########################
  file(GLOB PKG_APP_SCRIPTS
    ${OTB_INSTALL_DIR}/bin/otbcli*
    ${OTB_INSTALL_DIR}/bin/otbgui*) #

  list(LENGTH PKG_APP_SCRIPTS PKG_APP_SCRIPTS_LENGTH)
  if (PKG_APP_SCRIPTS_LENGTH LESS 1)
    message(WARNING "PKG_APP_SCRIPTS is empty: ${PKG_APP_SCRIPTS}")
  endif()

  install(FILES ${PKG_APP_SCRIPTS}
    DESTINATION ${PKG_BIN_DIR})

  foreach(exe_file ${PACKAGE_PEFILES})
    #check ext is .exe
    get_filename_component(FILE_EXT ${exe_file} EXT)
    string(TOLOWER ${FILE_EXT} file_ext)
    if( ${file_ext} STREQUAL ".exe")
      get_filename_component(base_name ${exe_file} NAME_WE)
      #check if a .bat exists?
      if(EXISTS ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${base_name}.bat)
        install(FILES ${Monteverdi_SOURCE_DIR}/Packaging/Windows/${base_name}.bat
          DESTINATION ${PKG_BIN_DIR})
      endif()
    endif()
  endforeach()

  install(CODE "file(WRITE \"${CMAKE_INSTALL_PREFIX}/${PKG_PREFIX_DIR}/otbenv.cmd\"
    \"set CURRENT_SCRIPT_DIR=%~dp0
    set PATH=%PATH%;%CURRENT_SCRIPT_DIR%\\bin
    set GDAL_DATA=%CURRENT_SCRIPT_DIR%\\share\\gdal
    \")" )

  ####################### create and install qt.conf #######################
  install(CODE "file(WRITE \"${PKG_QTCONF_DIR}/qt.conf\"
                \"[Paths]
                Translations=../lib/qt4/translations
                Plugins=../lib/qt4/plugins
                \")" )

  ####################### install sqldriver plugin ########################
  install(FILES ${QT_PLUGINS_DIR}/sqldrivers/${PKG_QTSQLITE_FILENAME}
    DESTINATION ${PKG_QTPLUGINS_DIR}/sqldrivers)

  ####################### install translations ###########################
  #get all translations already distributed with qt4
  get_qt_translation_files(QT_TRANSLATIONS_FILES)

  #install all files in ${QT_TRANSLATIONS_FILES}
  install(FILES ${QT_TRANSLATIONS_FILES}
    DESTINATION ${PKG_I18N_DIR})

  #translation of monteverdi specific strings
  file(GLOB APP_TS_FILES ${Monteverdi_SOURCE_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${Monteverdi_BINARY_DIR}/i18n/${APP_TS_FILENAME}.qm
      DESTINATION ${PKG_OTB_I18N_DIR})
  endforeach()

  ####################### install GDAL data ############################
  set(GDAL_DATA ${MXE_BIN_DIR}/../share/gdal)
  if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
    message(FATAL_ERROR "Cannot generate package without GDAL_DATA : ${GDAL_DATA} ${MXE_BIN_DIR}")
  endif()

  install(DIRECTORY ${GDAL_DATA}
    DESTINATION ${PKG_DATA_DIR})

  ####################### Check otb applications ########################
  file(GLOB OTB_APPS_LIST ${OTB_MODULES_DIR}/../../../otb/applications/otbapp_*dll) # /lib/otb
  if(NOT OTB_APPS_LIST)
    message(FATAL_ERROR "No OTB-applications detected")
  endif()

  ####################### Install otb applications ######################
  install(DIRECTORY "${OTB_MODULES_DIR}/../../../otb/applications"
    DESTINATION ${PKG_OTBLIBS_DIR})

endfunction()
