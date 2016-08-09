set(DOCUMENTATION
  "This module provides MonteverdiGui, the GUI library of the Monteverdi applications."
  )

otb_module( OTBMonteverdiGUI
  DEPENDS
    OTBCommon
    OTBIce
    OTBIOGDAL
    OTBMonteverdiCore
    OTBQwt

  OPTIONAL_DEPENDS
  
  DESCRIPTION
  "${DOCUMENTATION}"
  )
