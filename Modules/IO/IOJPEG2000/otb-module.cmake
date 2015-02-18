set(DOCUMENTATION "This module contains features to read and write JPEG2000
format images.")

otb_module(OTBIOJPEG2000 
  DEPENDS
    OTBTinyXML
    OTBImageBase
    OTBOSSIMAdapters
    OTBCommon
    OTBOpenJPEG
    OTBBoost
    OTBGDAL
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
