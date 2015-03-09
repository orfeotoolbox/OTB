set(OTBIOTileMap_SRC
  otbTileMapImageIOFactory.cxx
  otbTileMapImageIOHelper.cxx
  otbTileMapImageIO.cxx
  )

add_library(OTBIOTileMap ${OTBIOTileMap_SRC})
target_link_libraries(OTBIOTileMap 
  ${OTBIOGDAL_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBCurlAdapters_LIBRARIES}

  )

otb_module_target(OTBIOTileMap)
