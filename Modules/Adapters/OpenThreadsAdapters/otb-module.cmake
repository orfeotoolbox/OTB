set(DOCUMENTATION "Adapters for the OpenThreads library (thread interface for
C++).")

otb_module(OTBOpenThreadsAdapters

  TEST_DEPENDS
    OTBTestKernel
    OTBITK

  DESCRIPTION
    "${DOCUMENTATION}"
)
