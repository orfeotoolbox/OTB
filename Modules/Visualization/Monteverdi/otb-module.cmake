set(
  DOCUMENTATION
  "This module provides the Monteverdi program."
  )

otb_module( OTBIce
  DEPENDS

  OTBCommon
  MonteverdiCore
  MonteverdiGui

  OPTIONAL_DEPENDS
  
  DESCRIPTION
  "${DOCUMENTATION}"
  )
