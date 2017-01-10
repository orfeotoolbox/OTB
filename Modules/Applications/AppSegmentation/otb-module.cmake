set(DOCUMENTATION "Segmentation application.")

otb_module(OTBAppSegmentation
  DEPENDS
    OTBVectorDataBase
    OTBOGRProcessing
    OTBConversion
    OTBStatistics
    OTBImageIO
    OTBITK
    OTBCCOBIA
    OTBWatersheds
    OTBTransform
    OTBMetrics
    OTBSmoothing
    OTBMeanShift
    OTBGdalAdapters
    OTBApplicationEngine
    OTBImageBase
    OTBCommon
    OTBImageManipulation
    OTBMorphologicalProfiles
    OTBLabelMap
    OTBProjection

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
