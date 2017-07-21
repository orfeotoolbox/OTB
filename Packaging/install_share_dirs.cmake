function(install_share_dirs)

  #a convenient cmake var for storing <prefix>/bin
  set(PKG_STAGE_BIN_DIR "${PKG_STAGE_DIR}/bin")

  #<prefix>/share for gdal data files
  set(PKG_SHARE_DEST_DIR ${PKG_STAGE_DIR}/share)

  set(PKG_SHARE_SOURCE_DIR ${SUPERBUILD_INSTALL_DIR}/share)


  set(GDAL_DATA_DIR ${PKG_SHARE_SOURCE_DIR}/gdal)
  #MSVC install gdal-data in in a different directory. So we don't spoil it
  if(MSVC)
    set(GDAL_DATA_DIR ${SUPERBUILD_INSTALL_DIR}/data)
  endif()

  ####################### install GDAL data ############################
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

  
  install(FILES ${PKG_SHARE_SOURCE_DIR}/copyright/LICENSE DESTINATION ${PKG_STAGE_DIR})

  ####################### Install VERSION ##########################

  set(PKG_VERSION_FILE
    "${SUPERBUILD_INSTALL_DIR}/share/doc/${PKG_OTB_VERSION_MAJOR}.${PKG_OTB_VERSION_MINOR}/VERSION")
  if(EXISTS ${PKG_VERSION_FILE} )
    install(FILES ${PKG_VERSION_FILE} DESTINATION ${PKG_STAGE_DIR})
  endif()
 
endfunction()

