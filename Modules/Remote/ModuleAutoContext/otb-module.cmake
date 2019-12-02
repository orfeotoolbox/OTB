set(DOCUMENTATION "Implementation of Auto Context based on vector data.")

otb_module(OTBAutoContext
  DEPENDS
  OTBCommon
  OTBApplicationEngine
  OTBGdalAdapters
  OTBGDAL
  OTBStatistics
  OTBConversion
  OTBSampling
  OTBSupervised
  OTBLearningBase
  
  DESCRIPTION "Auto Context")
