set(OTBMetadata_SRC
  otbWorldView2ImageMetadataInterface.cxx
  otbQuickBirdImageMetadataInterface.cxx
  otbWorldView2ImageMetadataInterfaceFactory.cxx
  otbTerraSarImageMetadataInterface.cxx
  otbImageMetadataInterfaceBase.cxx
  otbImageMetadataInterfaceFactory.cxx
  otbFormosatImageMetadataInterface.cxx
  otbSpotImageMetadataInterfaceFactory.cxx
  otbDefaultImageMetadataInterfaceFactory.cxx
  otbSarDefaultImageMetadataInterfaceFactory.cxx
  otbOpticalDefaultImageMetadataInterfaceFactory.cxx
  otbSpotImageMetadataInterface.cxx
  otbFormosatImageMetadataInterfaceFactory.cxx
  otbOpticalImageMetadataInterfaceFactory.cxx
  otbSpot6ImageMetadataInterfaceFactory.cxx
  otbSarImageMetadataInterfaceFactory.cxx
  otbSpot6ImageMetadataInterface.cxx
  otbIkonosImageMetadataInterface.cxx
  otbPleiadesImageMetadataInterface.cxx
  otbQuickBirdImageMetadataInterfaceFactory.cxx
  otbOpticalImageMetadataInterface.cxx
  otbIkonosImageMetadataInterfaceFactory.cxx
  otbSarImageMetadataInterface.cxx
  otbPleiadesImageMetadataInterfaceFactory.cxx
  otbTerraSarImageMetadataInterfaceFactory.cxx
  otbNoDataHelper.cxx
  )

add_library(OTBMetadata ${OTBMetadata_SRC})
target_link_libraries(OTBMetadata 
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBMetadata)
