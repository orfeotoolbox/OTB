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
    OTBMathParser
    OTBCommon
    OTBImageManipulation
    OTBMorphologicalProfiles
    OTBLabelMap
    OTBProjection

  OPTIONAL_DEPENDS
    OTBEdisonMeanShift

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
