set(DOCUMENTATION
  "This module provides MonteverdiCore, the core library of the Monteverdi applications."
  )

otb_module( OTBMonteverdiCore
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
