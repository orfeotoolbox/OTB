set(DOCUMENTATION "This module contains the Monteverdi Gui library.")

otb_module(OTBMonteverdiGui
  DEPENDS
    OTBCommon
    OTBIce
    OTBIOGDAL
    OTBMonteverdiCore
    OTBQwt
    
  OPTIONAL_DEPENDS

# TODO : add optional depends to a future Qt4OpenGL  
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
