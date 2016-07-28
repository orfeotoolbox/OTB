set(DOCUMENTATION "This module contains the Monteverdi application.")

otb_module(OTBMonteverdi
  DEPENDS
    OTBMonteverdiCore
    OTBMonteverdiGui
    OTBQt4
    
  OPTIONAL_DEPENDS

  DESCRIPTION
    "${DOCUMENTATION}"
)
