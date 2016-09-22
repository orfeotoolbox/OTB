set(DOCUMENTATION
  "This module provides MonteverdiGui, the GUI library of the Monteverdi applications."
  )

otb_module( OTBMonteverdiGUI
  ENABLE_SHARED
  DEPENDS
    OTBCommon
    OTBIce
    OTBIOGDAL
    OTBMonteverdiCore
    OTBQt4
    OTBQwt

  OPTIONAL_DEPENDS
    OTBQtWidget

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
  "${DOCUMENTATION}"
  )
