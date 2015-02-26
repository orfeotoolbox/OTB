set(DOCUMENTATION "This module contains tools related to stereo-reconstruction.
Classes that allow block matching, fine registration, adherence correction,
elevation map construction are available.")

otb_module(OTBStereo
  DEPENDS
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBInterpolation
    OTBOSSIMAdapters
    OTBObjectList
    OTBStatistics
    OTBTransform

  TEST_DEPENDS
    OTBCommon
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
