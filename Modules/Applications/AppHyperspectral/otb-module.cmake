set(DOCUMENTATION "Unmixing application.")

otb_module(OTBAppHyperspectral 
  DEPENDS
    OTBStatistics
    OTBApplicationEngine
    OTBEndmembersExtraction
    OTBUnmixing
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
