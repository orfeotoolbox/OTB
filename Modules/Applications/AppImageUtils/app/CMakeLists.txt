set(OTBAppImageUtils_LINK_LIBS
  ${OTBStatistics_LIBRARIES}
  ${OTBColorMap_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBCarto_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBCurlAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
)

otb_create_application(
  NAME           DownloadSRTMTiles
  SOURCES        otbDownloadSRTMTiles.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           BandMathX
  SOURCES        otbBandMathX.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Convert
  SOURCES        otbConvert.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           PixelValue
  SOURCES        otbPixelValue.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ExtractROI
  SOURCES        otbExtractROI.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Rescale
  SOURCES        otbRescale.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           TileFusion
  SOURCES        otbTileFusion.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ReadImageInfo
  SOURCES        otbReadImageInfo.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Quicklook
  SOURCES        otbQuicklook.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           DEMConvert
  SOURCES        otbDEMConvert.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})


otb_create_application(
  NAME           ConcatenateImages
  SOURCES        otbConcatenateImages.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MultiResolutionPyramid
  SOURCES        otbMultiResolutionPyramid.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ColorMapping
  SOURCES        otbColorMapping.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           CompareImages
  SOURCES        otbCompareImages.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           SplitImage
  SOURCES        otbSplitImage.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
