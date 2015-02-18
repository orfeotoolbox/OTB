set(DOCUMENTATION "QT wrappers.")

otb_module(OTBQtWidget 
  DEPENDS
    OTBApplicationEngine
    OTBQt4
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
