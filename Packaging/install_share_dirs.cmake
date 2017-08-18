function(install_share_dirs)

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${SUPERBUILD_INSTALL_DIR}/share)


  ####################### install mvd i18N ############################
  # OTB i18n directory is different from qt's i18N directory
  #which is <prefix>/share/qt4/translations by default on linux.

  # To install otb i18n directory correctly, we need to find
  #Monteverdi_INSTALL_DATA_DIR. Value of this is a defined in  ConfigureMonteverdi.h
  # and default value is "share/OTB-X.Y"
  #...
  # #define Monteverdi_INSTALL_DATA_DIR
  #...
  #Instead of parsing ConfigureMonteverdi.h,
  #we simply use default and add a check for this directory!

  #define Monteverdi_INSTALL_DATA_DIR "share/OTB-X.Y"

  set(OTB_I18N_DIRNAME OTB-${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/i18n)  
  set(MVD_I18N_SOURCE_DIR ${PKG_SHARE_SOURCE_DIR}/${OTB_I18N_DIRNAME})
  if(NOT EXISTS "${MVD_I18N_SOURCE_DIR}")
    message(FATAL_ERROR "Error ${MVD_I18N_SOURCE_DIR} not exists")
  endif()
  set(MVD_I18N_DEST_DIR ${PKG_SHARE_DEST_DIR}/${OTB_I18N_DIRNAME})
  file(GLOB MVD_APP_TS_FILES ${PACKAGE_OTB_SRC_DIR}/i18n/*.ts) # qm files
  foreach(APP_TS_FILE ${APP_TS_FILES})
    get_filename_component(APP_TS_FILENAME ${APP_TS_FILE} NAME_WE)
    install(FILES ${MVD_I18N_SOURCE_DIR}/${APP_TS_FILENAME}.qm
      DESTINATION ${MVD_I18N_DEST_DIR}
      )
  endforeach()

  ####################### install GDAL data ############################
  set(GDAL_DATA_DIR ${PKG_SHARE_SOURCE_DIR}/gdal)
  #MSVC install gdal-data in in a different directory. So we don't spoil it
  if(MSVC)
    set(GDAL_DATA_DIR ${SUPERBUILD_INSTALL_DIR}/data)
  endif()
  if(NOT EXISTS "${GDAL_DATA_DIR}/epsg.wkt")
    message(FATAL_ERROR
      "Cannot generate package without GDAL_DATA : ${GDAL_DATA_DIR}")
  endif()
  install_without_message("${GDAL_DATA_DIR}" "share" )

  ####################### install GeoTIFF data ########################
  install_without_message("${PKG_SHARE_SOURCE_DIR}/epsg_csv" "share" )

  ####################### install OSSIM data ##########################
  install_without_message("${PKG_SHARE_SOURCE_DIR}/ossim" "share" )

  ####################### install proj share ##########################
  if(EXISTS ${PKG_SHARE_SOURCE_DIR}/proj)
    install_without_message("${PKG_SHARE_SOURCE_DIR}/proj" "share" )
  endif()
  
  ####################### Install copyrights ##########################
  #install license for packages
  install_without_message("${PKG_SHARE_SOURCE_DIR}/copyright" "share" )
  install(FILES ${PKG_SHARE_SOURCE_DIR}/copyright/LICENSE
    DESTINATION ${PKG_STAGE_DIR})

  ####################### Install VERSION ##########################
  set(PKG_VERSION_FILE
    "${SUPERBUILD_INSTALL_DIR}/share/doc/${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/VERSION")
  if(EXISTS ${PKG_VERSION_FILE} )
    install(FILES ${PKG_VERSION_FILE} DESTINATION ${PKG_STAGE_DIR})
  endif()
 
endfunction()

