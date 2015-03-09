set(DOCUMENTATION "Adapters for the OpenThreads library (thread interface for
C++).")

otb_module(OTBOpenThreadsAdapters
  DEPENDS
    OTBOpenThreads

  TEST_DEPENDS
    OTBITK
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
