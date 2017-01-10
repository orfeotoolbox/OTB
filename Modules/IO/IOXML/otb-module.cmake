set(DOCUMENTATION "This module contains features to read and write Read a xml
files.")

otb_module(OTBIOXML 
  DEPENDS
    OTBBoostAdapters
    OTBITK
    OTBTinyXML

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
  )
