set(DOCUMENTATION "This module contains base classes that allow the
representation of mono- and multiband images.")

otb_module(OTBImageBase 
  DEPENDS
    OTBCommon
    OTBOSSIMAdapters
    OTBITKPendingPatches
    OTBMetadata
  TEST_DEPENDS
    OTBMoments
    OTBTestKernel
    OTBDescriptors
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
