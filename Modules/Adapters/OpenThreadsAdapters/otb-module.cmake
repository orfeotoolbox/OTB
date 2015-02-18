set(DOCUMENTATION "Adapters for the OpenThreads library (thread interface for
C++).")

otb_module(OTBOpenThreadsAdapters 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
