function(install_translation_files)
  # get all translations already distributed with qt4/
  #
  # These files are the "qt_<localename>.qm" files
  # Qt distributes translation of common message strings used in Qt.
  #We just copy these files into package.
  
  file(GLOB QT_TRANSLATIONS_FILES ${QT_TRANSLATIONS_DIR}/qt_*)


  foreach(translation_item ${QT_TRANSLATIONS_FILES})

    # We need to remove the "qt_help_<localename>.qm" files from this list
    if(NOT ${translation_item} MATCHES "qt_help")
      install(FILES ${translation_item}
	DESTINATION "${PKG_STAGE_DIR}/lib/qt4/translations")
    endif()

  endforeach()
  
  # otb i18n directory is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations by default on linux.

  # To install otb i18n directory correctly, we need to find
  #Monteverdi_INSTALL_DATA_DIR. Value of this is a defined in  ConfigureMonteverdi.h
  # and default value is "share/OTB-6.1"
  #...
  # #define Monteverdi_INSTALL_DATA_DIR
  #...
  #Instead of parsing ConfigureMonteverdi.h,
  #we simply use default and add a check for this directory!

  #define Monteverdi_INSTALL_DATA_DIR "share/OTB-6.1"

  set(Monteverdi_I18N_SOURCE_DIR ${SUPERBUILD_INSTALL_DIR}/share/OTB-${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/i18n)
  set(Monteverdi_I18N_INSTALL_DIR ${PKG_STAGE_DIR}/share/OTB-${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/i18n)
  #translation of monteverdi specific strings
  if(NOT EXISTS "${Monteverdi_I18N_SOURCE_DIR}")
    message(FATAL_ERROR "Error ${Monteverdi_I18N_SOURCE_DIR} not exists")
  endif()
  
  file(GLOB APP_TS_FILES ${PACKAGE_OTB_SRC_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${Monteverdi_I18N_SOURCE_DIR}/${APP_TS_FILENAME}.qm
      DESTINATION ${Monteverdi_I18N_INSTALL_DIR}
      )
  endforeach()

endfunction()
