set(DOCUMENTATION "This module provides IceViewer, the ligthweight GLFW viewer based on Ice.")

otb_module(OTBSupervised
  DEPENDS
    OTBCIce
    OTBGlut
    OTBGLFW
    
  OPTIONAL_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
