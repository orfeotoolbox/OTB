set(DOCUMENTATION "This module provides Ice, a ligthweight efficient visualization framework for remote sensing images.")

otb_module(OTBSupervised
  DEPENDS
    OTBCommon
    OTBStatistics
    OTBGdalAdapters
    OTBImageIO
    OTBTransform
    OTBImageManipulation
    OTBOSSIMAdapters
    OTBProjection
    OTBImageBase
    OTBITK
    OTBGLEW
    OTBOpenGL
    
  OPTIONAL_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
