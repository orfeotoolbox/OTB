set(DOCUMENTATION "QT wrappers.")

otb_module(OTBQtWidget
ENABLE_SHARED
  DEPENDS
    OTBApplicationEngine
    OTBITK
    OTBQt4
    OTBQtAdapters

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
