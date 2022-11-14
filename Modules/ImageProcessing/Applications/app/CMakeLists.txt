
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

  otb_create_application(
  NAME           Smoothing
  SOURCES        otbSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ContrastEnhancement
  SOURCES        otbContrastEnhancement.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           FastNLMeans
  SOURCES        otbFastNLMeans.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
    NAME           BandMath
    SOURCES        otbBandMath.cxx
    LINK_LIBRARIES ${${otb-module}_LIBRARIES})


  otb_create_application(
      NAME           BandMathX
      SOURCES        otbBandMathX.cxx
      LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
  NAME           RadiometricIndices
  SOURCES        otbRadiometricIndices.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LineSegmentDetection
  SOURCES        otbLineSegmentDetection.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           EdgeExtraction
  SOURCES        otbEdgeExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

  otb_create_application(
    NAME           BinaryMorphologicalOperation
    SOURCES        otbBinaryMorphologicalOperation.cxx
    LINK_LIBRARIES ${${otb-module}_LIBRARIES})

    otb_create_application(
      NAME           HaralickTextureExtraction
      SOURCES        otbHaralickTextureExtraction.cxx
      LINK_LIBRARIES ${${otb-module}_LIBRARIES})
    
    otb_create_application(
      NAME           SFSTextureExtraction
      SOURCES        otbSFSTextureExtraction.cxx
      LINK_LIBRARIES ${${otb-module}_LIBRARIES})
    
    otb_create_application(
      NAME           PantexTextureExtraction
      SOURCES        otbPantexTextureExtraction.cxx
      LINK_LIBRARIES ${${otb-module}_LIBRARIES})
    