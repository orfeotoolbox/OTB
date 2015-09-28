set(OTBMetadata_SRC
  otbWorldView2ImageMetadataInterfaceFactory.cxx
  otbWorldView2ImageMetadataInterface.cxx

  otbQuickBirdImageMetadataInterfaceFactory.cxx
  otbQuickBirdImageMetadataInterface.cxx

  otbImageMetadataInterfaceFactory.cxx
  otbImageMetadataInterfaceBase.cxx

  otbSpotImageMetadataInterfaceFactory.cxx
  otbSpotImageMetadataInterface.cxx

  otbDefaultImageMetadataInterfaceFactory.cxx
  otbSarDefaultImageMetadataInterfaceFactory.cxx
  otbOpticalDefaultImageMetadataInterfaceFactory.cxx

  otbFormosatImageMetadataInterfaceFactory.cxx
  otbFormosatImageMetadataInterface.cxx

  otbOpticalImageMetadataInterfaceFactory.cxx
  otbOpticalImageMetadataInterface.cxx

  otbSpot6ImageMetadataInterfaceFactory.cxx
  otbSpot6ImageMetadataInterface.cxx

  otbIkonosImageMetadataInterfaceFactory.cxx
  otbIkonosImageMetadataInterface.cxx

  otbPleiadesImageMetadataInterfaceFactory.cxx
  otbPleiadesImageMetadataInterface.cxx

  otbSarImageMetadataInterfaceFactory.cxx
  otbSarImageMetadataInterface.cxx

  otbTerraSarImageMetadataInterfaceFactory.cxx
  otbTerraSarImageMetadataInterface.cxx

  otbSentinel1ImageMetadataInterfaceFactory.cxx
  otbSentinel1ImageMetadataInterface.cxx

  otbCosmoSkymedImageMetadataInterfaceFactory.cxx
  otbCosmoSkymedImageMetadataInterface.cxx

  otbNoDataHelper.cxx
  )

add_library(OTBMetadata ${OTBMetadata_SRC})
target_link_libraries(OTBMetadata
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBMetadata)
