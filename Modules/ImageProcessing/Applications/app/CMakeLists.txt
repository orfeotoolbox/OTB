
otb_create_application(
    NAME           Quicklook
    SOURCES        otbQuicklook.cxx
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
  NAME           PixelValue
  SOURCES        otbPixelValue.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           Rescale
  SOURCES        otbRescale.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           TileFusion
  SOURCES        otbTileFusion.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  OTB_CREATE_APPLICATION(
  NAME           Synthetize
  SOURCES        otbSynthetize.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           SplitImage
  SOURCES        otbSplitImage.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})