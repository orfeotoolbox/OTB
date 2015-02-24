set(DOCUMENTATION "This module contains tools related to stereo-reconstruction.
Classes that allow block matching, fine registration, adherence correction,
elevation map construction are available.")

otb_module(OTBStereo
  DEPENDS
    OTBStatistics
    OTBITK
    OTBITKPendingPatches
    OTBImageBase
    OTBTransform
    OTBCommon
    OTBInterpolation
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageManipulation

  DESCRIPTION
    "${DOCUMENTATION}"
)
