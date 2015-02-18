set(DOCUMENTATION "Adapters for the famous OpenCV library. This module contains
a generic machine learning framework for supervised classification.")

otb_module(OTBOpenCVAdapters 
  DEPENDS
    OTBOpenCV
    OTBBoost
    OTBCommon
  
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
