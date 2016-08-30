set(DOCUMENTATION
  "This module provides the Monteverdi Application Launcher (Mapla)  program."
  )

otb_module( OTBMapla
  DEPENDS
    OTBMonteverdiCore
    OTBMonteverdiGUI
    OTBQt4

  OPTIONAL_DEPENDS
  
  DESCRIPTION
  "${DOCUMENTATION}"
  )
