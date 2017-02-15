set(DOCUMENTATION "This module provides IceViewer, the ligthweight GLFW viewer based on Ice.")

otb_module(OTBIceViewer
ENABLE_SHARED
  DEPENDS
    OTBIce
    OTBMetadata
    OTBOSSIMAdapters
    OTBGlut
    OTBGLFW
    
  OPTIONAL_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
