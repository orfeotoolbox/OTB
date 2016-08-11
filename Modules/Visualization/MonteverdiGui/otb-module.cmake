set(DOCUMENTATION
  "This module provides MonteverdiGui, the GUI library of the Monteverdi applications."
  )

otb_module( OTBMonteverdiGUI
  DEPENDS
    OTBCommon
    OTBIce
    OTBIOGDAL
    OTBMonteverdiCore
    OTBQt4
    OTBQwt

  OPTIONAL_DEPENDS
    OTBQtWidget

  DESCRIPTION
  "${DOCUMENTATION}"
  )
