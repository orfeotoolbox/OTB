set(DOCUMENTATION "This module contains the Monteverdi Core library.")

otb_module(OTBMonteverdiCore
  DEPENDS
    OTBCarto
    OTBCommon
    OTBImageBase
    OTBImageIO
    OTBImageManipulation
    OTBIOGDAL
    OTBMetadata
    OTBObjectList
    OTBOSSIMAdapters
    OTBProjection
    OTBQt4
    OTBStatistics
    OTBTransform
    
  OPTIONAL_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
