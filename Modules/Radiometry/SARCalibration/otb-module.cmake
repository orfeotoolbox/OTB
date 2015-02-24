set(DOCUMENTATION "This module contains tools related to SAR calibration
(classes to compute the backscatter or the brigthness of a given pixel and so on).")

otb_module(OTBSARCalibration
  DEPENDS
    OTBITKPendingPatches
    OTBMetadata
    OTBITK
    OTBOSSIMAdapters
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBStatistics
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
