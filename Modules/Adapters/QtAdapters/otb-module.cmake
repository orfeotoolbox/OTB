set( DOCUMENTATION "Adapters for the Qt Library.")

otb_module( OTBQtAdapters
  ENABLE_SHARED
  DEPENDS
    OTBQt4

  # TEST_DEPENDS
  #   OTBCommon

  DESCRIPTION
    "${DOCUMENTATION}"
)
