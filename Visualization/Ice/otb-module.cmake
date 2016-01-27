set(DOCUMENTATION "This module provides Ice, a ligthweight efficient visualization framework for remote sensing images.")

otb_module(OTBSupervised
  DEPENDS
    OTBCommon
    OTBGLEW
    OTBOpenGL
    
  OPTIONAL_DEPENDS
    
  DESCRIPTION
    "${DOCUMENTATION}"
)
