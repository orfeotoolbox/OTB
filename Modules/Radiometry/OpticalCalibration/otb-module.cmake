set(DOCUMENTATION "Digital number to luminance correction. luminance to
refletance image conversion. atmospheric correction for TOA (top of atmosphere) to TOC
(top of canopy) reflectance estimation. correction of the adjacency effects taking
into account the neighborhood contribution.")

otb_module(OTBOpticalCalibration
  DEPENDS
    OTB6S
    OTBBoost
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBMetadata
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
