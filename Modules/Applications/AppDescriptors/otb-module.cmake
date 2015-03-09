set(DOCUMENTATION "Descritors application( see also Descriptors module).")

otb_module(OTBAppDescriptors 
  DEPENDS
    OTBGdalAdapters
    OTBDescriptors
    OTBTransform
    OTBApplicationEngine
    OTBImageBase
    OTBGDAL
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
