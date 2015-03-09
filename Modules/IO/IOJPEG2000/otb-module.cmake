set(DOCUMENTATION "This module contains features to read and write JPEG2000
format images.")

otb_module(OTBIOJPEG2000
  DEPENDS
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBCommon
    OTBTinyXML
    OTBOpenJPEG
    OTBBoost
    OTBGDAL

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation

  DESCRIPTION
    "${DOCUMENTATION}"
)
