set(DOCUMENTATION "This module contains tools related to SAR calibration
(classes to compute the backscatter or the brigthness of a given pixel and so on).")

otb_module(OTBSARCalibration
  DEPENDS
    OTBCommon
    OTBITK
    OTBMetadata
    OTBOSSIMAdapters

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
