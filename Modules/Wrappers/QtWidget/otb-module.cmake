set(DOCUMENTATION "QT wrappers.")

otb_module(OTBQtWidget
  DEPENDS
    OTBQt4
    OTBApplicationEngine
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
