set(DOCUMENTATION "This module contains the Mapla application.")

otb_module(OTBMapla
  DEPENDS
    OTBMonteverdiCore
    OTBMonteverdiGui
    OTBQt4
    
  OPTIONAL_DEPENDS 
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
