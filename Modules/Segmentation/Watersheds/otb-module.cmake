set(DOCUMENTATION "OTB implementation of Watersheds segmentation algorithm.")

otb_module(OTBWatersheds 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
