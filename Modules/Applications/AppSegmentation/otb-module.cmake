set(DOCUMENTATION "Segmentation application.")

otb_module(OTBAppSegmentation 
  DEPENDS
    OTBVectorDataBase
    OTBLabelMap
    OTBConversion
    OTBImageManipulation
    OTBImageIO
    OTBProjection
    OTBImageBase
    OTBWatersheds
    OTBGdalAdapters
    OTBSmoothing
    OTBMetrics
    OTBOGRProcessing
    OTBStatistics
    OTBMeanShift
    OTBApplicationEngine
    OTBCCOBIA
    OTBMathParser
    OTBMorphologicalProfiles
    OTBTransform
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
