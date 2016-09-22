set( DOCUMENTATION
  "This module provides the Monteverdi program."
  )

otb_module( OTBMonteverdi
  ENABLE_SHARED
  DEPENDS
    OTBMonteverdiCore
    OTBMonteverdiGUI
    OTBQt4

  OPTIONAL_DEPENDS

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
  "${DOCUMENTATION}"
  )
